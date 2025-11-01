#pragma once
#include <vector>
#include <string>

enum class GCodeType { G0, G1, Unknown };
class Robot2DOF;
struct GCodeCommand {
    GCodeType type;
    double x = 0;
    double y = 0;
    double f = 1.0; // feedrate (speed)
};

void ExecuteGCodeStep(Robot2DOF& robot, const std::vector<GCodeCommand>& cmds, double dt);