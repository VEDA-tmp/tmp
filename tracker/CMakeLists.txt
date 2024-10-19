# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(MyTVMProject)

find_package(OpenCV REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost COMPONENTS program_options filesystem REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Could not locate Boost")
endif()

#프로파일링 플래그 설정
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# TVM 경로 설정
set(TVM_PATH "TVM 경로")
set(TVM_INCLUDE_PATH ${TVM_PATH}/include)
set(DMLC_INCLUDE_PATH ${TVM_PATH}/3rdparty/dmlc-core/include)
set(DLPACK_INCLUDE_PATH ${TVM_PATH}/3rdparty/dlpack/include)
set(TVM_LIB_PATH ${TVM_PATH}/build)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)

set(SRCS
    src/hungarian.cpp
    src/kalman_filter.cpp
    src/main.cpp
    src/munkres.cpp
    src/track.cpp
    src/tracker.cpp
    src/tvm_wrapper.cpp
)

# 헤더 파일 경로 추가
include_directories(${TVM_INCLUDE_PATH})
include_directories(${DMLC_INCLUDE_PATH})
include_directories(${DLPACK_INCLUDE_PATH})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})

# TVM 라이브러리 경로 추가
link_directories(${TVM_LIB_PATH})

# 프로젝트 소스 파일 설정
add_executable(MyTVMProject ${SRCS})

# TVM 런타임 라이브러리 링크
target_link_libraries(MyTVMProject PRIVATE 
            tvm_runtime 
            ${OpenCV_LIBS} 
            ${Boost_LIBRARIES}
)
target_include_directories(MyTVMProject PRIVATE ${PROJECT_SOURCE_DIR}/include)
