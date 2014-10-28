#include <cstring>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct string_comparison {
    typedef std::pair<int, std::string> p_type;
    bool operator () (const p_type& L, const p_type& R) {
        const std::string& right = R.second;
        const std::string& left = L.second;
        if(left.size()<right.size()) {
            return true;
        } else if( left.size()>right.size()) {
            return false;
        }
        return (std::strcmp(left.c_str(), right.c_str()) < 0);
    }
};

int main(void) {
    // these files have lists of words that we can use for testing
    const char names_file[] = "/usr/share/dict/propernames";
    const char words_file[] = "/usr/share/dict/words";

    std::vector<std::string> dictionary;

    // read words from file
    std::ifstream fid(words_file, std::ifstream::in);

    while( !fid.eof() ) {
        std::string line;
        std::getline(fid, line);
        // convert to lower case
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        dictionary.push_back(line);
    }

    // pair the strings with an index
    std::vector<std::pair<int, std::string>> keypairs;
    for(int i=0; i<dictionary.size(); ++i) {
        std::string str = dictionary[i];
        std::sort(str.begin(), str.end());
        keypairs.push_back(std::make_pair(i, str));
    }

    // sort the keypairs
    std::sort(keypairs.begin(), keypairs.end(), string_comparison());

    // search for duplicates
    std::vector<std::vector<int>> anagrams;
    std::vector<int> indexes(32);
    auto it = keypairs.begin();
    while(it!=keypairs.end()) {
        // resize indexes to 1 to keep previously allocated memory
        indexes.resize(1);
        indexes[0] = it->first;
        std::string& s = it->second;
        // find all matching keys
        while( (++it)!=keypairs.end() && it->second == s ) {
            indexes.push_back(it->first);
        }
        // save this set of indexes if there were matches
        if(indexes.size()>1) {
            anagrams.push_back(indexes);
        }
    }

    // print results to terminal
    for(auto const &set : anagrams) {
        for(auto i : set) {
            std::cout << dictionary[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

