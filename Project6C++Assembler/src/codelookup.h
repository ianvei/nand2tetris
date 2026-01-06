#ifndef CODE_LOOKUP_H
#define CODE_LOOKUP_H

#include <unordered_map>
#include <string>
#include <bitset>

class CodeLookup {
    public:
        static std::bitset<7> comp(const std::string& key);
        static std::bitset<3> dest(const std::string& key);
        static std::bitset<3> jump(const std::string& key);


    private:
        static const std::unordered_map<std::string, std::bitset<7>> compTable;
        static const std::unordered_map<std::string, std::bitset<3>> destTable;
        static const std::unordered_map<std::string, std::bitset<3>> jumpTable;
};

#endif