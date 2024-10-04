#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>


//시간을 측정할 수 있도록 하는 클래스입니다,
//클래스를 호출하고 out of scope가 되면 자동으로 시간을 측정할 수 있도록 했습니다.
class Timer {
    public:
        Timer() : m_start(std::chrono::high_resolution_clock::now()) {}

        ~Timer() {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
            std::cout << "Elapsed time : " << duration << " ms" << std::endl;
        }

        long long elapsedMilliseconds() const {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
        }

    
    private:
        std::chrono::high_resolution_clock::time_point m_start;
};

#endif