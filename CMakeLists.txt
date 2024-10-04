cmake_minimum_required(VERSION 3.13)
project(tvm_tutorial)

set(OpenCV_DIR "OPEN_CV 소스 경로")

find_package(OpenCV REQUIRED)

# TVM 경로 설정
set(TVM_PATH "tvm 소스 경로")  
include_directories("${TVM_PATH}/include")
include_directories("${TVM_PATH}/3rdparty/dmlc-core/include")
include_directories("${TVM_PATH}/3rdparty/dlpack/include")
link_directories("${TVM_PATH}/build")

# C++17 이상을 사용할 수 있도록 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENV{TVM_LOG_DEBUG} 1)

set(SOURCE
    src/main.cpp
    src/heatmap_generator.cpp
    src/tvm_wrapper.cpp
)

add_executable(tvm_tutorial ${SOURCE})

# 시스템 라이브러리 및 TVM 라이브러리 링크
target_link_libraries(tvm_tutorial tvm ${OpenCV_LIBS})
target_include_directories(tvm_tutorial PRIVATE ${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})
