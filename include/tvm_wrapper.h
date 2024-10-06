#ifndef _TVM_Wrapper_
#define _TVM_Wrapper_

//module은 TVM 모델 자체를 불러오는 라이브러리입니다.
#include <tvm/runtime/module.h>
//so 파일내의 바이너리 파일로 저장된 파일의 funcion을 불러올 수 있도록 하는 라이브러리입니다.
#include <tvm/runtime/packed_func.h>
//tvm에서 사용하는 tensor 자료구조입니다.
#include <tvm/runtime/ndarray.h>
#include <opencv2/opencv.hpp>

#include "tracker.h"


// 예시 코코 데이터 셋 설정
const static std::vector<std::string> COCO_CLASS = {
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat", "traffic light", 
        "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", 
        "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", 
        "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard", 
        "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", 
        "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa", 
        "pottedplant", "bed", "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard", 
        "cell phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", 
        "teddy bear", "hair drier", "toothbrush"
};

//TVM 클래스를 호출할떄 model 이름과 연산 디바이스를 미리 설정하도록 했습니다.  
class TVMWrapper {
    public:
        TVMWrapper(const std::string& model_path, const std::string _model_name, const std::string device = "cpu") {
            loaded_lib = tvm::runtime::Module::LoadFromFile(model_path);

            if(device == "gpu")  {
                dev = {kDLCUDA, 0};
            } else if (device == "cpu") {
                dev = {kDLCPU,  0};
            }
                
            this->mod = loaded_lib.GetFunction("default")(dev);
            this->model_name = _model_name;
        };

        void detect();
        void detect_video(const std::string video_path);
        tvm::runtime::NDArray preprocess_frame(cv::Mat& frame,int _batch, int _input_w, int _input_h);

    private:
        tvm::runtime::Module loaded_lib;
        tvm::runtime::Module mod;
        DLDevice dev;

        std::string model_name;
};

#endif //_TVM_Wrapper_