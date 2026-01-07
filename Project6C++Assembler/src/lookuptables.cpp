#include <unordered_map>
#include <iostream>
#include <string>
#include "lookuptables.h"

LookupTable::LookupTable()
{
    initPreDefinedVariables();
}

void LookupTable::getLookUpTable()
{
    for (const auto &pair : _lookupTable)
    {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

void LookupTable::initPreDefinedVariables()
{
    // available registers start at 16
    R = 16;
    // pre-defined variables
    _lookupTable["SP"] = 0;
    _lookupTable["LCL"] = 1;
    _lookupTable["ARG"] = 2;
    _lookupTable["THIS"] = 3;
    _lookupTable["THAT"] = 4;

    // General purpose registers
    for (int i = 0; i <= 15; i++)
    {
        _lookupTable["R" + std::to_string(i)] = i;
    }

    // Memory-mapped I/O
    _lookupTable["SCREEN"] = 16384;
    _lookupTable["KBD"] = 24576;
}

bool LookupTable::checkExistingSymbol(const std::string &symbol)
{
    if (_lookupTable.find(symbol) != _lookupTable.end())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LookupTable::setNewLabel(const std::string &key, int val)
{
    _lookupTable[key] = val;
}

int LookupTable::getSymbolValue(const std::string &key)
{
    return _lookupTable[key];
}

int LookupTable::setSymbol(const std::string &key)
{
    _lookupTable[key] = R;
    int assigned = R;
    R++;
    return assigned;
}
