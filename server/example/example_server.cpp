#include "rtsp/server.hpp"
#include "rtsp/header.hpp"

using namespace rtsp::server;

int main(int argc, char **argv) {
    app app;
    app.port(5004).start_streaming();
    return 0;
}

