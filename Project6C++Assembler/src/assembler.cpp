#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "lookuptables.h"
#include "codelookup.h"
#include <cctype>
#include <algorithm>

class FileParser
{
public:
    FileParser(LookupTable &LookupTable) : table(LookupTable) {}
    std::string currentLine;
    void processSymbols(std::string &line, int pcLine)
    {
        // A instruction
        currentLine = line;
        // jump label
        if (currentLine[0] == '(')
        {
            // set the string inside the () to the map, with pcLine as the value
            std::string label = currentLine.substr(1, currentLine.size() - 2);
            if (!table.checkExistingSymbol(label))
            {
                table.setNewLabel(label, pcLine);
            }
        }
    }

    bool isNumericAInstruction(const std::string &line)
    {
        if (line.size() <= 1 || line[0] != '@')
        {
            return false;
        }

        const std::string value = line.substr(1);

        return std::all_of(
            value.begin(),
            value.end(),
            [](unsigned char c)
            {
                return std::isdigit(c);
            });
    }

    void processCurrentLine(std::string line, std::ostream &out)
    {
        currentLine = line;

        // A instruction
        if (currentLine[0] == '@' || currentLine[0] == '(')
        {
            if (currentLine[0] == '(')
            {
                return;
            }

            // variable - check if currentLine[0] is @ (a instruction)
            //  and currentLine[1] is NOT an integer.
            if (currentLine[0] == '@' && currentLine.size() > 1 && !isNumericAInstruction(currentLine))
            {
                const std::string symbol = currentLine.substr(1);
                if (!table.checkExistingSymbol(symbol))
                {
                    const int symbolAddress = table.setSymbol(symbol);
                    // DO SOMETHING WITH ADDRESS
                    std::bitset bitset = std::bitset<16>(symbolAddress);
                    std::cout << bitset << std::endl;
                    out << bitset << '\n';
                }
                else
                {
                    const int symbolAddress = table.getSymbolValue(symbol);
                    // DO SOMETHING WITH ADDRESS
                    std::bitset bitset = std::bitset<16>(symbolAddress);
                    std::cout << bitset << std::endl;
                    out << bitset << '\n';
                }
                // lookup symbol in symbol table
                // if not there, assign it an address
            }
            else
            {
                int aValue = std::stoi(currentLine.substr(1));
                // always safe because 32767 is max value that fits in 15 bits
                // write this line to the output file
                std::bitset bitset = std::bitset<16>(aValue);
                std::cout << bitset << std::endl;
                out << bitset << '\n';
            }
        }
        // C instruction
        else
        {
            // basically, do the same as above but use the lookup tables to assemble the binary
            std::string dest;
            std::string remainder;
            std::bitset<16> newInstruction;

            // set first 3 bits to 111
            newInstruction |= (std::bitset<16>(0b111) << 13);

            auto equalsPos = currentLine.find('=');
            if (equalsPos != std::string::npos)
            {
                dest = currentLine.substr(0, equalsPos);
                auto destInstruction = CodeLookup::dest(dest);
                remainder = currentLine.substr(equalsPos + 1);
                newInstruction |= (std::bitset<16>(destInstruction.to_ulong()) << 3);
            }
            else
            {
                // there is no dest instruction
                remainder = currentLine; // instruction is comp;jump
            }

            std::string comp;
            std::string jump;
            auto semiColPos = remainder.find(';');
            if (semiColPos != std::string::npos)
            {
                comp = remainder.substr(0, semiColPos);
                jump = remainder.substr(semiColPos + 1);
                auto compInstruction = CodeLookup::comp(comp);
                auto jumpInstruction = CodeLookup::jump(jump);
                newInstruction |= (std::bitset<16>(compInstruction.to_ulong()) << 6);
                newInstruction |= std::bitset<16>(jumpInstruction.to_ulong());
            }
            else
            {
                comp = remainder.substr(0, semiColPos);
                auto compInstruction = CodeLookup::comp(comp);
                newInstruction |= (std::bitset<16>(compInstruction.to_ulong()) << 6);
                // last 3 bits always 0 if no jump condition
                const std::bitset<3> noJumpConst = 000;
                newInstruction |= std::bitset<16>(noJumpConst.to_ulong());
            }
            std::cout << newInstruction << std::endl;
            out << newInstruction << '\n';
        }
    }

private:
    LookupTable &table;
};

std::string trimInlineComments(std::string string)
{
    auto commentStartPos = string.find("//");
    std::string cleanString = string;
    if (commentStartPos != std::string::npos)
    {
        cleanString = string.substr(0, commentStartPos);
    }

    return cleanString;
}

void ltrim(std::string &s)
{
    const auto pos = s.find_first_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
    {
        s.clear(); // string was all whitespace
    }
    else
    {
        s.erase(0, pos);
    }
}

void rtrim(std::string &s)
{
    const auto pos = s.find_last_not_of(" \t\n\r\f\v");
    if (pos == std::string::npos)
        s.clear();
    else
        s.erase(pos + 1);
}

int main()
{
    std::string line;
    std::ifstream assemblyFile("asmfiles/Pong.asm");
    int pcLine = 0;
    LookupTable table;
    FileParser fileParser(table);
    std::ofstream outFile("asmfiles/Pong.hack");
    if (!outFile)
    {
        std::cout << "Error: Could not open output file" << std::endl;
        return 1;
    }
    if (assemblyFile.is_open())
    {
        while (std::getline(assemblyFile, line))
        {
            ltrim(line);
            if (line.empty())
            {
                continue;
            }
            if (line[0] == '/' && line[1] == '/')
            {
                continue;
            }
            line = trimInlineComments(line);
            ltrim(line);
            rtrim(line);
            fileParser.processSymbols(line, pcLine);
            if (line[0] != '(')
            {
                pcLine++;
            }
        }
        assemblyFile.clear();
        assemblyFile.seekg(0, std::ios::beg);
        while (std::getline(assemblyFile, line))
        {
            ltrim(line);
            // if line is empty or starts with //, skip it
            if (line.empty())
            {
                continue;
            }
            if (line[0] == '/' && line[1] == '/')
            {
                continue;
            }
            // process line with file parser
            line = trimInlineComments(line);
            ltrim(line);
            rtrim(line);
            fileParser.processCurrentLine(line, outFile);
        }
    }
    else
    {
        std::cout << "Error: Could not open file" << std::endl;
    }
    return 0;
}