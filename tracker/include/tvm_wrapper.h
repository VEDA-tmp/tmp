#ifndef _TVM_Wrapper_
#define _TVM_Wrapper_

#include <tvm/runtime/module.h>
#include <tvm/runtime/packed_func.h>
#include <tvm/runtime/ndarray.h>
#include <opencv2/opencv.hpp>

#include "tracker.h"

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
        tvm::runtime::NDArray preprocess_frame(cv::Mat& frame,int _batch, int _input_w, int _input_h);
        

    private:
        tvm::runtime::Module loaded_lib;
        tvm::runtime::Module mod;
        DLDevice dev;

        std::string model_name;
};

#endif //_TVM_Wrapper_