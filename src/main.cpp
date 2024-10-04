#include "timer.h"
#include "tvm_wrapper.h"

int main() {
    const std::string model_path = "SO 파일 경로";

    TVMWrapper tw(model_path, "yolov5");
    
    tw.detect();
}