#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <unordered_set>
#include <array>
#include <limits>

std::ostream& operator<<(std::ostream& os, std::vector<int> v) {
    std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(os, " "));
    return os;
}

std::vector<int> get_3_max_numbers(const std::vector<int> vec){
    std::vector<int> maxs(3);
    std::fill(std::begin(maxs), std::end(maxs), std::numeric_limits<int>::min());
    for (auto item : vec) {
        if (item > maxs[0]) {
            maxs[2] = maxs[1];
            maxs[1] = maxs[0];
            maxs[0] = item;
        }
        else if (item > maxs[1]){
            maxs[2] = maxs[1];
            maxs[1] = item;
        }
        else if (item > maxs[2]){
            maxs[2] = item;
        }
    }
    return maxs;
}

int main(int argc, const char * argv[]) {
    // 1
    std::vector<int> P1(10);
    std::iota(std::begin(P1), std::end(P1), 1);
    // 2
    int input;
    std::cout << "введите 2 числа по очереди: " << std::endl;
    for (int i = 0; i < 2; i++) {
        std::cin >> input;
        P1.push_back(input);
    }
    // 3
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(std::begin(P1), std::end(P1), g);
    // 4
    std::unordered_set<int> temp (std::begin(P1), std::end(P1));
    P1.resize(std::size(temp));
    std::copy(std::begin(temp), std::end(temp), std::begin(P1));
    // 5
    std::cout << "количество нечетных в P1: " << std::count_if(std::begin(P1), std::end(P1), [](auto item){ return item % 2; }) << std::endl;
    // 6
    auto [min, max] = std::minmax_element(std::begin(P1), std::end(P1));
    std::cout << "минимальный и максимальный элемент соответственно: " << *min << "\t" << *max << std::endl;
    // 7
    std::cout << "одно из простый чисел: " << *std::find_if(std::begin(P1), std::end(P1), [](auto item){
        std::vector<int> ostatki(item - 2);
        std::iota(std::begin(ostatki), std::end(ostatki), 2);
        return std::all_of(std::begin(ostatki), std::end(ostatki), [item](auto elem){ return item % elem;});
    }) << std::endl;
    // 8
    std::for_each(std::begin(P1), std::end(P1), [](auto &item){ item = item * item;});
    // 9
    std::vector<int> P2(std::size(P1));
    std::generate(std::begin(P2), std::end(P2), [&g](){return g();});
    // 10
    std::cout << "сумма P2: " << std::accumulate(std::begin(P2), std::end(P2), 0LL) << std::endl;
    // 11
    std::for_each(std::begin(P2), std::begin(P2) + 2, [](auto &item){item = 1;});
    // 12
    std::vector<int> P3(std::size(P1));
    std::transform(std::begin(P1), std::end(P1), std::begin(P2), std::begin(P3), std::minus<int>());
    // 13
    std::replace_if(std::begin(P3), std::end(P3), [](auto item){return item < 0;}, 0LL);
    // 14
    P3.erase(std::remove_if(std::begin(P3), std::end(P3), [](auto item){return item == 0LL;}), std::end(P3));
    // 15
    std::reverse(std::begin(P3), std::end(P3));
    // 16
    std::cout << "3 максимальных числа: " << get_3_max_numbers(P3) << std::endl;
    // 17
    std::sort(std::begin(P1), std::end(P1));
    std::sort(std::begin(P2), std::end(P2));
    // 18
    std::vector<int> P4(std::size(P1) + std::size(P2));
    std::merge(std::begin(P1), std::end(P1), std::begin(P2), std::end(P2), std::begin(P4));
    // 19
    std::cout << "нижняя грань для вставки 1: " << std::distance(std::begin(P4), std::lower_bound(std::begin(P4), std::end(P4), 1)) << std::endl;
    std::cout << "верхняя грань для вставки 1: " << std::distance(std::begin(P4), std::upper_bound(std::begin(P4), std::end(P4), 1)) << std::endl;
    // 20
    std::cout << "P1: " << P1 << std::endl;
    std::cout << "P2: " << P2 << std::endl;
    std::cout << "P3: " << P3 << std::endl;
    std::cout << "P4: " << P4 << std::endl;
    return 0;
}
