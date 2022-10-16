#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <numeric>


int main(int argc, char** argv) {
    if (argc == 3) {
        std::ifstream dictionaryFile(argv[1]);
        std::ifstream inputFile(argv[2]);
        std::string word;
        int num;

        std::vector<std::pair<std::string, int>> dictionary; //make dictionary
        while (dictionaryFile >> word, dictionaryFile >> num) {
            word.pop_back();
            dictionary.emplace_back(word, num);
        }
        dictionaryFile.close();


        std::vector<std::vector<std::pair<std::string, int>>> result(1); // create result without permutations
        std::string line;
        while (line.empty()) {
            if (!std::getline(inputFile, line)) { std::cout << "File is empty!"; return 0; }
        }
        std::stringstream sLine(line);
        while (sLine >> word) {
            std::vector<std::vector<std::pair<std::string, int>>> curVariants;
            for (auto &&item: dictionary) {
                if (std::is_permutation(item.first.begin(), item.first.end(), word.begin())
                && word.size() == item.first.size()) { // important!
                    for (auto&& text: result) {
                        curVariants.push_back(text);
                        curVariants.back().push_back(item);
                    }
                }
            }
            if (curVariants.empty()) {
                std::cout << "No words for anagram '" << word << "'" << std::endl;
                return 0;
            }
            result = curVariants;
        }


        auto&& compareItems = [](std::vector<std::pair<std::string, int>>& item1, std::vector<std::pair<std::string, int>>& item2) {
            return std::accumulate(item1.begin(),
                            item1.end(), 0,
                            [](int previous, const std::pair<const std::string, int>& p)
                            { return previous + p.second; }) >=
            std::accumulate(item2.begin(),item2.end(), 0,
                            [](int previous, const std::pair<const std::string, int>& p)
                            { return previous + p.second; });}; //sum of weights of item1 >= sum of weights of item2
        std::sort(result.begin(), result.end(), compareItems); //sort result by weights


        for (auto&& item: result) { // print result with all permutations
            std::sort(item.begin(), item.end());
            do {
                std::for_each(item.begin(),
                              item.end(),
                              [](std::pair<std::string, int>& pair){ std::cout << pair.first << " "; });
                const std::size_t sum = std::accumulate(item.begin(),
                                                        item.end(), 0,
                                                           [](int previous, const std::pair<const std::string, int>& p)
                                                           { return previous + p.second; });
                std::cout << sum << std::endl;
            } while(std::next_permutation(item.begin(),item.end()));
        }
    }
    else {
        std::cout << "Must be 2 arguments: input and output files";
    }
    return 0;
}