#ifndef LOOKUP_TABLES_H
#define LOOKUP_TABLES_H

#include <unordered_map>
#include <string>

class LookupTable
{
public:
    LookupTable();
    void getLookUpTable();
    void setNewLabel(const std::string &key, int val);
    bool checkExistingSymbol(const std::string &symbol);
    int getSymbolValue(const std::string &key);
    int setSymbol(const std::string &key);

private:
    std::unordered_map<std::string, int> _lookupTable;
    void initPreDefinedVariables();
    int R;
};

#endif