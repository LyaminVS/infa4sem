#include <iostream>
#include <future>
#include <thread>
#include <numeric>
#include <vector>

template<typename T>
void print_vector(std::vector<T> &vector){
    for (auto i: vector){
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
}

template<class InputIt, class UnaryFunction>
constexpr UnaryFunction block_for_each(InputIt first, InputIt last, UnaryFunction f){
    std::for_each(first, last, f);
    return f;
}

template<class InputIt, class UnaryFunction>
constexpr UnaryFunction parallel_for_each(InputIt first, InputIt last, UnaryFunction f, int threat_number){
    std::vector<std::future<UnaryFunction>> futures;
    long int len = std::distance(first, last);
    long int one_block_len = len / threat_number;
    for (int i = 0; i < threat_number - 1; i++) {
        futures.push_back(std::async(std::launch::async, &block_for_each<InputIt, UnaryFunction>, std::next(first, one_block_len * i), std::next(first, one_block_len * (i + 1)), f));
    }
    std::for_each(std::next(first, (threat_number - 1) * one_block_len), last, f);
    return f;
}
    

int main(int argc, const char * argv[]) {
    std::vector<int> vec(1000);
    
    std::iota(std::begin(vec), std::end(vec), 1);
    parallel_for_each(std::begin(vec), std::end(vec), [](auto &elem){elem = elem * elem;}, 8);
    
    print_vector(vec);
    
    return 0;
}
