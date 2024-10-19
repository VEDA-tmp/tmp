#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include "tvm_wrapper.h"

int main() {
    const std::string model_path = "모델 경로;

    TVMWrapper tw(model_path, "yolov5");

    // 키보드 입력을 감지하는 스레드 시작
    tw.detect();

    return 0;
}
