#include <iostream>
#include <regex>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> find_matches(const std::string& text, std::regex pattern) {
    std::vector<std::string> matches;
    std::transform(std::sregex_iterator(std::begin(text), std::end(text), pattern), std::sregex_iterator(), std::back_inserter(matches), [](auto match){return match.str();});
    return matches;
}

int main() {
    std::fstream fin("text.txt", std::ios::in);
    std::stringstream ss;
    for(std::string tmp; std::getline(fin, tmp); ss << tmp << std::endl);
    auto text = ss.str();
    std::cout << text << std::endl;
    
    std::cout << "Found matches" << std::endl;
    
    std::regex pattern(R"([\w|\.|_]+@[\w|\.|_]+\.[\w|\.|_]+)");

    auto matches = find_matches(text, pattern);

    std::copy(std::begin(matches), std::end(matches), std::ostream_iterator<std::string>(std::cout, "\n"));
}
