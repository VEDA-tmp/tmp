#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <vector>

#define RTP_PORT 5004
#define RTP_HEADER_SIZE 12
#define BUFFER_SIZE 65536  // 충분히 큰 버퍼 크기로 설정 (패킷 크기 합산을 위한 공간)

struct rtp_header {
    unsigned char vpxcc;
    unsigned char mpt;
    unsigned short sequence_number;
    unsigned int timestamp;
    unsigned int ssrc;
};

void print_rtp_header(struct rtp_header *header) {
    printf("RTP Version: %d\n", header->vpxcc >> 6);
    printf("Payload Type: %d\n", header->mpt & 0x7F);
    printf("Sequence Number: %d\n", ntohs(header->sequence_number));
    printf("Timestamp: %u\n", ntohl(header->timestamp));
    printf("SSRC: %u\n", ntohl(header->ssrc));
}

int main() {
    int sockfd, n;
    struct sockaddr_in servaddr;
    socklen_t len;
    char buffer[BUFFER_SIZE];
    struct rtp_header rtp_hdr;

    // OpenCV 윈도우 생성
    cv::namedWindow("RTP Video Stream", cv::WINDOW_AUTOSIZE);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(RTP_PORT);

    // 서버에 초기 메시지 전송
    sendto(sockfd, "Hello", 5, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

    std::vector<uchar> jpeg_buffer;

    while (true) {
        len = sizeof(servaddr);
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);

        if (n < RTP_HEADER_SIZE) {
            printf("Received packet is too small to contain RTP header\n");
            continue;
        }

        // RTP 헤더 처리
        memcpy(&rtp_hdr, buffer, RTP_HEADER_SIZE);
        print_rtp_header(&rtp_hdr);

        // RTP 헤더 이후 데이터 (JPEG 데이터 부분)
        size_t payload_size = n - RTP_HEADER_SIZE;
        jpeg_buffer.insert(jpeg_buffer.end(), buffer + RTP_HEADER_SIZE, buffer + RTP_HEADER_SIZE + payload_size);

        // 마지막 패킷 여부 확인 (마커 비트 확인)
        if (rtp_hdr.mpt & 0x80) {
            // 마커 비트가 설정된 경우, JPEG 데이터를 완성했음
            std::cout << "Received complete frame, decoding..." << std::endl;

            // OpenCV를 사용하여 JPEG 데이터를 디코딩
            cv::Mat frame = cv::imdecode(jpeg_buffer, cv::IMREAD_COLOR);

            if (frame.empty()) {
                std::cerr << "Error decoding frame" << std::endl;
            } else {
                // 디코딩한 프레임을 윈도우에 표시
                cv::imshow("RTP Video Stream", frame);
            }

            // 'q' 키를 누르면 스트리밍 종료
            if (cv::waitKey(1) == 'q') {
                break;
            }

            // JPEG 버퍼를 비움 (다음 프레임을 위해)
            jpeg_buffer.clear();
        }
    }

    close(sockfd);
    cv::destroyAllWindows();  // 윈도우 닫기

    return 0;
}
