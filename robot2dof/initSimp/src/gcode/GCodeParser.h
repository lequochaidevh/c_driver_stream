#pragma once
#include <vector>
#include <string>
#include "GCodeCommand.h"

class GCodeParser {
public:
    std::vector<GCodeCommand> Parse(const std::string& path);
};
