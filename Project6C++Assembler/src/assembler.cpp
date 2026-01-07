#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "lookuptables.h"
#include "codelookup.h"


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
                std::cout << bitset << std::endl;
            } 
            //C instruction
            else 
            {
                std::cout << "C: " << currentLine << "\n";
                //basically, do the same as above but use the lookup tables to assemble the binary
                std::string dest;
                std::string remainder;  
                std::bitset<16> newInstruction;

                //set first 3 bits to 111
                newInstruction |= (std::bitset<16>(0b111) << 13);

                auto equalsPos = currentLine.find('=');
                if(equalsPos != std::string::npos) {
                    dest = currentLine.substr(0, equalsPos);
                    // std::cout << CodeLookup::dest(dest) << std::endl;
                    auto destInstruction = CodeLookup::dest(dest);
                    remainder = currentLine.substr(equalsPos + 1);
                    //  std::cout << "dest: " << dest << std::endl;
                    newInstruction |= (std::bitset<16>(destInstruction.to_ulong()) << 3);
                    //  std::cout << "remainder: " << remainder << std::endl;
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

                    auto compInstruction = CodeLookup::comp(comp);
                    auto jumpInstruction = CodeLookup::jump(jump);
                    newInstruction |= (std::bitset<16>(compInstruction.to_ulong()) << 6);
                    newInstruction |= std::bitset<16>(jumpInstruction.to_ulong());
                    // std::cout << "comp: " << comp << std::endl;
                    // std::cout << "jump: " << jump << std::endl;
                    // std::cout << CodeLookup::comp(comp) << std::endl;
                    // std::cout << CodeLookup::jump(jump) << std::endl;

                } else {
                    comp = remainder.substr(0, semiColPos);
                    auto compInstruction = CodeLookup::comp(comp);
                    newInstruction |= (std::bitset<16>(compInstruction.to_ulong()) << 6);
                    const std::bitset<3> noJumpConst = 000;
                    newInstruction |= std::bitset<16>(noJumpConst.to_ulong());
                    std::cout << "comp: " << comp << std::endl;
                    std::cout << CodeLookup::comp(comp) << std::endl;
                }
                std::cout << newInstruction << std::endl;
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