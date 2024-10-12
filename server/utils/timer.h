#include <chrono>
#include <iostream>
#include <string>


class TimerBaseClock {
    public:
        explicit TimerBaseClock(const std::string& task_name) : task_name_(task_name), start_time_(std::chrono::high_resolution_clock::now()) {
            std::cout << "Timer Clock started time : " << task_name_ << std::endl;
        }

        ~TimerBaseClock() {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_).count();
            std::cout << "Stopwatch stopped for : " << task_name_ << ". Elapsed time : " << duration << " ms " << std::endl;
        }

        long long elapsed() const {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<std::chrono::microseconds>(now - start_time_).count();
        }

    

    private:
        std::string task_name_;
        std::chrono::high_resolution_clock::time_point start_time_;
};