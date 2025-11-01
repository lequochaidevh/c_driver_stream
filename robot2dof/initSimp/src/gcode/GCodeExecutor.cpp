#include "GCodeCommand.h"
#include "../application.h"
#include <cmath>

static int currentCmd = 0;
static double moveProgress = 0.0;

void ExecuteGCodeStep(Robot2DOF& robot, const std::vector<GCodeCommand>& cmds, double dt) {
    if (currentCmd >= cmds.size()) return;

    const auto& cmd = cmds[currentCmd];

    static double startX, startY;
    static double endX, endY;
    static bool initialized = false;

    if (!initialized) {
        // Tính vị trí đầu
        auto fk = robot.forwardKinematics(robot.theta1, robot.theta2);
        startX = fk.back().getXY().x;
        startY = fk.back().getXY().y;
        endX = cmd.x;
        endY = cmd.y;
        moveProgress = 0.0;
        initialized = true;
    }

    // Tính vector di chuyển
    double dx = endX - startX;
    double dy = endY - startY;
    double dist = std::sqrt(dx*dx + dy*dy);

    double v = cmd.f; // tốc độ feedrate
    moveProgress += dt * v / dist;

    if (moveProgress >= 1.0) {
        moveProgress = 1.0;
        initialized = false;
        currentCmd++;
    }

    double curX = startX + dx * moveProgress;
    double curY = startY + dy * moveProgress;

    auto sols = robot.inverseKinematics(curX, curY);
    if (!sols.empty()) {
        robot.theta1 = sols[0].theta1;
        robot.theta2 = sols[0].theta2;
    }
}
