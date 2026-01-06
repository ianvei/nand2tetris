#ifndef LOOKUP_TABLES_H
#define LOOKUP_TABLES_H

#include <unordered_map>
#include <string>

class LookupTable
{
public:
    LookupTable();
    void getLookUpTable();

private:
    std::unordered_map<std::string, int> _lookupTable;
    void initPreDefinedVariables();
};

#endif