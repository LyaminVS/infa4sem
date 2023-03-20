#include <thread>
#include <numeric>
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <filesystem>
#include <fstream>

using steady_clock_t = std::chrono::steady_clock;

template <typename T>
class Timer {
public:
    Timer(std::ofstream& stream): begin(steady_clock_t::now()), stream(stream) {}
    
    ~Timer() {
        stream << std::chrono::duration_cast<T>(steady_clock_t::now() - begin).count() << std::endl;
    }
private:
    steady_clock_t::time_point begin;
    std::string message;
    std::ofstream& stream;
};

template<typename Iter, typename T>
void accumulate_block(Iter begin, Iter end, T init, T& result) {
    result = std::accumulate(begin, end, init);
}

template<typename Iter, typename T>
T parallel_accumulate(Iter begin, Iter end, T init, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<T> results(num_threads - 1);
    auto block_size = std::distance(begin, end) / num_threads;
    for (auto i = 0u; i + 1 < num_threads; i++) {
        threads.push_back(std::thread(
            accumulate_block<Iter, T>,
            std::next(begin, i * block_size),
            std::next(begin, (i + 1) * block_size),
            0,
            std::ref(results[i]))
        );
    }
    T last_result;
    accumulate_block(std::next(begin, (num_threads - 1) * block_size),
            end, init, last_result);
    std::for_each(std::begin(threads), std::end(threads), std::mem_fn(&std::thread::join));
    return std::accumulate(std::begin(results), std::end(results), last_result);
}

void get_times_for_range_of_threats(int min, int max, int size, std::filesystem::path path){
    std::ofstream file;
    file.open(path);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution d(1, 10);
    for (int i = min; i < max + 1; i++) {
        std::cin >> size;
        std::vector<int> randoms;
        std::generate_n(std::back_inserter(randoms), size, [&gen, &d](){return d(gen);});
        {
            Timer<std::chrono::milliseconds> t(file);
            auto result = parallel_accumulate(randoms.begin(), randoms.end(), 0, i);

        }
    }
    file.close();
}

int main(int argc, const char * argv[]) {
    // запускать из юпитера для построения графика
    int min_threads = std::stoi(argv[1]);
    int max_threads = std::stoi(argv[2]);
    int size = std::stoi(argv[3]);
    std::filesystem::path path = argv[4];
    get_times_for_range_of_threats(min_threads, max_threads, size, path);
}
