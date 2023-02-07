#include <iostream>
#include <chrono>
#include <math.h>

template <typename T>
class Timer {
public:
    Timer(){};
    
    void start(){
        start_time = std::chrono::steady_clock::now();
        started = true;
    }
    
    void pause(){
        sum_time += std::chrono::steady_clock::now() - start_time;
        started = false;
    }
    
    void print_pesult(){
        if (started){
            pause();
        }
        std::cout << std::chrono::duration_cast<T>(sum_time).count() << std::endl;
    }
    
private:
    bool started = false;
    std::chrono::steady_clock::time_point start_time;
    std::chrono::duration<double> sum_time = std::chrono::steady_clock::duration::zero();
};

int main(int argc, const char * argv[]) {
    Timer<std::chrono::nanoseconds> t;
    auto count = 0;
    t.start();
    for (auto i = 0; i < 100000000; i++) {
        count += sin(i) + cos(i);
    }
    t.pause();
    for (auto i = 0; i < 100000000; i++) {
        count += sin(i) + cos(i);
    }
//    t.start();
    for (auto i = 0; i < 100000000; i++) {
        count += sin(i) + cos(i);
    }
    t.print_pesult();
    return 0;
}
