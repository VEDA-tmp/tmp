#include "server.hpp"

namespace rtsp {
    namespace server {
        // 프레임 접근을 보호하는 뮤텍스
        std::mutex frame_mutex;
        // 최신 프레임을 저장하는 변수
        cv::Mat global_frame;

        void build_rtp_header(RtpHeader *hdr, unsigned seq_num, unsigned int timestamp, unsigned int ssrc) {
            hdr->vpxcc = 0x80;
            hdr->mpayload = 96;
            hdr->sequence_number = htons(seq_num);
            hdr->timestamp = htonl(timestamp);
            hdr->ssrc = htonl(ssrc);
        }

        void capture_start_thread(const int device_id) {
            cv::VideoCapture cap(device_id);

            if(!cap.isOpened()) {
                std::cerr << "Error : Unabled to open the camera stream" << std::endl;
                return;
            }

            while (true) {
                cv::Mat frame;
                cap >> frame;

                if(frame.empty()) {
                    std::cerr << "Error : frame is empty" << std::endl;
                    break;
                }
                {
                    std::lock_guard<std::mutex> lock(frame_mutex);
                    global_frame = frame.clone();
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(30)); // 30 FPS
            }
            cap.release();
        }

        void rtp_streaming_thread(int sockfd, struct sockaddr_in cliaddr) {
            unsigned short sequence_number = 0;
            unsigned int timestamp = 0;
            unsigned int ssrc = 12345;
            socklen_t len = sizeof(cliaddr);
            const size_t max_packet_size = 1000 - RTP_HEADER_SIZE;

            while (true) {
                cv::Mat frame;
                {
                    std::lock_guard<std::mutex> lock(frame_mutex);
                    if (global_frame.empty()) {
                        continue;
                    }
                    frame = global_frame.clone();
                }

                std::vector<uchar> buf;
                if (!cv::imencode(".jpg", frame, buf)) {
                    std::cerr << "Error encoding frame to JPEG" << std::endl;
                    continue;
                } 

                size_t total_size = buf.size();
                size_t offset = 0;


                // RTP 패킷 나눠서 전송
                while (offset < total_size) {
                    // 패킷에 담을 데이터
                    size_t packet_size = std::min(max_packet_size, total_size - offset);
                    char rtp_packet[RTP_BUFFER_SIZE]; // 우선 4096 고정 추후에 변경 ( 네트워크, 프레임 고려 )

                    
                    rtsp::server::RtpHeader rtp_hdr;
                    build_rtp_header(&rtp_hdr, sequence_number++, timestamp, ssrc);
                    timestamp += 160; // RTP timestamp 증가 (프레임 속도에 맞게 조정 가능)

                    // RTP 마커 비트 설정 (최종 패킷에만 설정)
                    if (offset + packet_size >= total_size) {
                        rtp_hdr.mpayload |= 0x80; 
                    }

                    // RTP 헤더 복사
                    memcpy(rtp_packet, &rtp_hdr, RTP_HEADER_SIZE);
                    
                    // RTP 패킷에 데이터 복사
                    memcpy(rtp_packet + RTP_HEADER_SIZE, buf.data() + offset, packet_size);

                    if (sendto(sockfd, rtp_packet, packet_size + RTP_HEADER_SIZE, 0, (struct sockaddr*)&cliaddr, len) < 0) {
                        perror("Error sending RTP packet");
                        break;
                    }

                    // 다음 패킷으로 offset 이동 
                    offset += packet_size;
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        }
    }

    
}
