#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "lookuptables.h"


class FileParser
{
    // parse individual line, return binary?
    // detect if C instruction or A instruction
    public:
        std::string currentLine;

        std::string processCurrentLine(std::string line) {
            currentLine = line;

            //A instruction
            if(currentLine[0] == '@'){
                std::cout << "A: " << currentLine << "\n";
                int aValue = std::stoi(currentLine.substr(1));
                //always safe because 32767 is max value that fits in 15 bits
                //write this line to the output file
                std::bitset bitset = std::bitset<16>(aValue);
                std::cout << "A: " << currentLine << "\n";
                std::cout << "A: " << bitset << "\n";
            } 
            //C instruction
            else 
            {
                std::cout << "C: " << currentLine << "\n";
                //basically, do the same as above but use the lookup tables to assemble the binary
                std::string dest;
                std::string remainder;

                auto equalsPos = currentLine.find('=');
                if(equalsPos != std::string::npos) {
                    dest = currentLine.substr(0, equalsPos);
                    remainder = currentLine.substr(equalsPos + 1);
                     std::cout << "dest: " << dest << std::endl;
                     std::cout << "remainder: " << remainder << std::endl;
                } else {
                    //there is no dest instruction
                    remainder = currentLine; // instruction is comp;jump
                }

                std::string comp;
                std::string jump;
                auto semiColPos = remainder.find(';');
                if(semiColPos != std::string::npos) {
                    comp = remainder.substr(0, semiColPos);
                    jump = remainder.substr(semiColPos + 1);
                    std::cout << "comp: " << comp << std::endl;
                    std::cout << "jump: " << jump << std::endl;
                }
            }
            return "";
        }
    
};

int main()
{
    FileParser fileParser;
    std::string line;
    std::ifstream assemblyFile("asmfiles/Add.asm");

    LookupTable table;
    if (assemblyFile.is_open())
    {
        while (std::getline(assemblyFile, line))
        {
            // if line is empty or starts with //, skip it
            if (line.empty())
            {
                continue;
            }
            if(line[0] == '/' && line[1] == '/')
            {
                continue;
            }
            // process line with file parser
            // std::cout << line << std::endl;
            fileParser.processCurrentLine(line);
        }
        // table.getLookUpTable();
    }
    else
    {
        std::cout << "Error: Could not open file" << std::endl;
    }
    return 0;
}