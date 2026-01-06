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
    // pre-defined variables
    _lookupTable["SP"] = 0;
    _lookupTable["LCL"] = 1;
    _lookupTable["ARG"] = 2;
    _lookupTable["THIS"] = 3;
    _lookupTable["THAT"] = 4;

    // General purpose registers
    for (int i = 0; i < 15; i++)
    {
        _lookupTable["R" + std::to_string(i)] = i;
    }

    // Memory-mapped I/O
    _lookupTable["SCREEN"] = 16384;
    _lookupTable["KBD"] = 24576;
}