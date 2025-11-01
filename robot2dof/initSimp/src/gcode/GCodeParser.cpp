#include "GCodeParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<GCodeCommand> GCodeParser::Parse(const std::string& path) {
    std::vector<GCodeCommand> cmds;
    std::ifstream f(path);
    if(!f.is_open()){
        std::cerr << "Cannot open GCode file: " << path << std::endl;
        return cmds;
    }

    std::string line;
    while(std::getline(f, line)) {
        if(line.empty() || line[0] == ';') continue; // comment

        std::istringstream iss(line);
        std::string token;
        GCodeCommand cmd;

        iss >> token;
        if(token == "G0") cmd.type = GCodeType::G0;
        else if(token == "G1") cmd.type = GCodeType::G1;
        else cmd.type = GCodeType::Unknown;

        while(iss >> token) {
            char c = token[0];
            double val = atof(token.substr(1).c_str());
            if(c == 'X') cmd.x = val;
            else if(c == 'Y') cmd.y = val;
            else if(c == 'F') cmd.f = val;
        }

        cmds.push_back(cmd);
    }
    return cmds;
}
