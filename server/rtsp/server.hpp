#ifndef RTSP_SEVER_HPP
#define RTSP_SEVER_HPP

#include <opencv2/opencv.hpp>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>

#include "header.hpp"

// #define RTP_PORT 5004
#define RTP_HEADER_SIZE 12
#define RTP_BUFFER_SIZE 4096


namespace rtsp {
namespace server {

    void build_rtp_header(RtpHeader *hdr, unsigned seq_num, unsigned int timestamp, unsigned int ssrc);
    void capture_start_thread(const int device_id);
    void rtp_streaming_thread(int sockfd, struct sockaddr_in cliaddr);
    
    struct app {
        app& port(uint16_t p) {
            port_num = p;
            return *this;
        }

        void handle_client(int sockfd) {
            struct sockaddr_in cliaddr;
            socklen_t len = sizeof(cliaddr);
            char mesg[1000];

            std::cout << "Waiting for client connection..." << std::endl;

            // 클라이언트가 접속할 때까지 대기
            int n = recvfrom(sockfd, mesg, sizeof(mesg), 0, (struct sockaddr *)&cliaddr, &len);
            if (n < 0) {
                perror("Error receiving from client");
                return;
            }

            std::cout << "Client connected. Streaming video..." << std::endl;

            // 클라이언트에 RTP 패킷을 전송하는 스레드 시작
            std::thread rtp_thread(rtsp::server::rtp_streaming_thread, sockfd, cliaddr);
            rtp_thread.join();  // RTP 스트리밍이 끝날 때까지 대기
        }

        void start_streaming() {
            sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
            if(sock_fd < 0) {
                std::cerr << "Failed to create Socket" << std::endl;
                return;
            }

            memset(&servaddr, 0, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
            servaddr.sin_port = htons(port_num);

            if(bind(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
                std::cerr << "Failed to bind socket" << std::endl;
                return;
            }

            camera_thread.reset(new std::thread(capture_start_thread, 0));

            handle_client(sock_fd);

            camera_thread->join();
            close(sock_fd);

        }

        uint16_t port_num = 5004;
        int sock_fd;
        struct sockaddr_in servaddr;
        std::unique_ptr<std::thread> camera_thread;
    };
}; // namespace server
} // namesapce rtsp

#endif //RTSP_SERVER_HPP