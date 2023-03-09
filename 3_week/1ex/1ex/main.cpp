#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <algorithm>
#include "json.hpp"
#include <filesystem>
#include <format>

template<typename T>
void print_vector(std::vector<T> &vector){
    for (auto i: vector){
        std::cout << i << std::endl;
    }
    std::cout << std::endl;
}

struct Person{
    int age;
    double height, weight;
    std::string name;
    std::vector<int> marks;
};

std::vector<std::string> split(std::string str){
    std::vector<std::string> result = {""};
    int k = 0;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' '){
            k++;
            result.push_back("");
        }
        else {
            result[k].push_back(str[i]);
        }
    }
    return result;
}
std::vector<int> str_vector_to_int(std::vector<std::string> vec){
    std::vector<int> result;
    for (auto item: vec) {
        result.push_back(std::stoi(item));
    }
    return result;
}

std::istream& operator >> (std::istream& in, Person& p)
{
    std::string input;
    std::cout << "Enter your name" << std::endl;
    in >> p.name;
    std::cout << "Enter your age" << std::endl;
    in >> p.age;
    std::cout << "Enter your height" << std::endl;
    in >> input;
    p.height = std::stod(input);
    std::cout << "Enter your weight" << std::endl;
    in >> input;
    in.ignore();
    p.weight = std::stod(input);
    std::cout << "Enter your grades separated by a space" << std::endl;
    getline(in, input, '\n');
    p.marks = str_vector_to_int(split(input));
    return in;
}

nlohmann::json person_to_json(Person& p){
    nlohmann::json result;
    result["age"] = p.age;
    result["height"] = p.height;
    result["weight"] = p.weight;
    result["name"] = p.name;
    result["marks"] = p.marks;
    return result;
}

int main(int argc, const char * argv[]) {
    std::filesystem::create_directory("json_files");
    for (int i = 0; i < 3; i++) {
        std::cout << "Person " + std::to_string(i) << std::endl;
        Person p;
        std::cin >> p;
        std::ofstream test("json_files/json_" + std::to_string(i) + ".json");
        test << person_to_json(p);
        test.close();
    }
    
    return 0;
}
