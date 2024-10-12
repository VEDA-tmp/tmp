#ifndef RTSP_HEADER_HPP
#define RTSP_HEADER_HPP


namespace rtsp {
    namespace server {
        /*
            vpxcc : version + padding + extension + crsc count
            mpayload : 패킷 내에서 프레임 경계와 같은 중요한 이벤트들을 표시 PayloadType 필드의 확장을 위해 무시되기도 함
            sequence number : 패킷이 늘어날때마다 1씩 증가 tcp와 마찬가지로 패킷 손실 여부 확인
            timestamp : rtp 패킷의 첫 번쨰 바이트의 샘플링을 표시
            ssrc : 동기화 소스로 랜덤 넘버로 결정
        */
        struct RtpHeader {
            unsigned char vpxcc;
            unsigned char mpayload;
            unsigned short sequence_number;
            unsigned int timestamp;
            unsigned int ssrc;
        };

        

    }
}

#endif