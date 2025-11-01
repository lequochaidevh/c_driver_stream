#include "GCodeCommand.h"
#include "../application.h"   // để có Robot2DOF
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>
static void moveLinear(Robot2DOF& robot, double targetX, double targetY, double feedrate, double dt);
static void moveArcCCW(Robot2DOF& robot, double cx, double cy, double targetX, double targetY, double feedrate, double dt);
static void dwell(double seconds);

// Thực thi toàn bộ chuỗi lệnh G-code
void ExecuteGCodeStep(Robot2DOF& robot, const std::vector<GCodeCommand>& cmds, double dt)
{
    // std::cout << "Excute called" << std::endl;
    for (auto& cmd : cmds)
    {
        if (cmd.type == "G0" || cmd.type == "G1") {
            moveLinear(robot, cmd.x, cmd.y, cmd.feedrate, dt);
            // break;
        }
        else if (cmd.type == "G3") {
            double cx = cmd.x - cmd.i;
            double cy = cmd.y - cmd.j;
            moveArcCCW(robot, cx, cy, cmd.x, cmd.y, cmd.feedrate, dt);
        }
        else if (cmd.type == "G4") {
            dwell(cmd.dwellTime);
        }
        else {
            std::cout << "⚠️ Unknown G-code: " << cmd.type << std::endl;
            break;
        }
    }
}

// ======== HÀM PHỤ TRỢ ========
// Di chuyển tuyến tính
static void moveLinear(Robot2DOF& robot, double targetX, double targetY, double feedrate, double dt)
{
    double x0 = robot.GetCurrentX();
    double y0 = robot.GetCurrentY();
    targetX /= 100;
    targetY /= 100;
    feedrate /= 100;
    double dx = targetX - x0;
    double dy = targetY - y0;
    double dist = std::sqrt(dx*dx + dy*dy);
    if (dist < 1e-8) return;

    int steps = std::max(1, (int)(dist / (feedrate * dt)));
    std::cout << "steps = " << steps << std::endl;
    for (int i = 1; i <= steps; i++) {
        double ratio = (double)i / steps;
        double nx = x0 + ratio * dx;
        double ny = y0 + ratio * dy;
        robot.MoveTo(nx, ny);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(dt*1000)));
    }
}

// Chuyển động tròn ngược kim đồng hồ (G3)
static void moveArcCCW(Robot2DOF& robot, double cx, double cy, double tx, double ty, double feedrate, double dt)
{
    double x0 = robot.GetCurrentX();
    double y0 = robot.GetCurrentY();
    cx /= 100;
    cy /= 100;
    tx /= 100;
    ty /= 100;
    feedrate /= 100;

    double r = std::sqrt((x0 - cx)*(x0 - cx) + (y0 - cy)*(y0 - cy));
    double startAngle = std::atan2(y0 - cy, x0 - cx);
    double endAngle   = std::atan2(ty - cy, tx - cx);
    // std::cout << "r = " << r << std::endl;
    // std::cout << "startAngle = " << startAngle << std::endl;
    // std::cout << "endAngle = " << endAngle << std::endl;
    if (abs(endAngle - startAngle) < 1e-8) endAngle += 2*M_PI;

    double arcLength = r * (endAngle - startAngle);
    // std::cout << "arcLength = " << arcLength << std::endl;
    int steps = std::max(1, (int)(arcLength / (feedrate * dt)));
    std::cout << "steps = " << steps << std::endl;
    for (int i = 1; i <= steps; i++) {
        double theta = startAngle + (endAngle - startAngle) * ((double)i / steps);
        double nx = cx + r * std::cos(theta);
        double ny = cy + r * std::sin(theta);
        robot.MoveTo(nx, ny);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(dt*1000)));
    }
}

// Dừng tạm thời G4 P[t]
static void dwell(double seconds)
{
    std::cout << "⏸️ Dwell for " << seconds << " s" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(seconds * 1000)));
}
