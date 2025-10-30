
#include "application.h"
#include "window/window.h"

double target_theta1 = 0.0;
double target_theta2 = 0.0;

// ---------- Robot class ----------
struct IKResult {
    double theta1, theta2;
};

class Robot2DOF {
public:
    double l1, l2;
    double theta1, theta2; // current joint angles (rad)

    Robot2DOF(double _l1, double _l2):l1(_l1), l2(_l2), theta1(0), theta2(0){}

    // Forward kinematics: return transforms [T0, T1, T2(end-eff)]
    std::vector<Mat4> forwardKinematics(double th1, double th2) const {
        Mat4 T0 = Mat4(); // base
        Mat4 T1 = Mat4::rotZ(th1) * Mat4::translate(l1,0,0);
        Mat4 T2 = Mat4::rotZ(th2) * Mat4::translate(l2,0,0);
        Mat4 T01 = T0 * Mat4::rotZ(th1) * Mat4::translate(l1,0,0);
        Mat4 T02 = T01 * Mat4::rotZ(th2) * Mat4::translate(l2,0,0);
        return {T0, T01, T02};
    }

    // Convenience: get end-effector pos
    Vec2 endEffector(double th1, double th2) const {
        double x = l1 * cos(th1) + l2 * cos(th1 + th2);
        double y = l1 * sin(th1) + l2 * sin(th1 + th2);
        return Vec2(x,y);
    }

    // Jacobian 2x2 (filled into array J[2][2])
    void jacobian(double th1, double th2, double J[2][2]) const {
        double s1 = sin(th1), c1 = cos(th1);
        double s12 = sin(th1+th2), c12 = cos(th1+th2);
        J[0][0] = - l1*s1 - l2*s12;
        J[0][1] = - l2*s12;
        J[1][0] =   l1*c1 + l2*c12;
        J[1][1] =   l2*c12;
    }

    // Inverse kinematics (returns two solutions possibly). We return
    // vector with 0, 1 or 2 solutions (theta1, theta2).
    std::vector<IKResult> inverseKinematics(double x, double y) const {
        std::vector<IKResult> sols;
        double r2 = x*x + y*y;
        double cos_th2 = (r2 - l1*l1 - l2*l2) / (2.0 * l1 * l2);
        if (cos_th2 < -1.0 - 1e-12 || cos_th2 > 1.0 + 1e-12) return sols;
        if (cos_th2 > 1.0) cos_th2 = 1.0;
        if (cos_th2 < -1.0) cos_th2 = -1.0;
        double sin_th2_pos = sqrt(std::max(0.0, 1.0 - cos_th2*cos_th2));
        double sin_th2_neg = -sin_th2_pos;

        auto make_sol = [&](double s2, double c2){
            double th2 = atan2(s2, c2);
            double k1 = l1 + l2*c2;
            double k2 = l2*s2;
            double th1 = atan2(y,x) - atan2(k2, k1);
            // normalize angles to (-pi, pi]
            return IKResult{th1, th2};
        };

        sols.push_back(make_sol(sin_th2_pos, cos_th2));
        if (fabs(sin_th2_pos - sin_th2_neg) > 1e-9) // distinct
            sols.push_back(make_sol(sin_th2_neg, cos_th2));
        return sols;
    }
};

// ---------- Globals for simple app ----------
static Robot2DOF robot(0.6, 0.4);
static bool render_enabled = true;
static bool sim_enabled = false;
static double time_acc = 0.0;
static double target_x = 0.6, target_y = 0.0;
static bool show_target = false;

// ---------- Main ----------
int main(){
    WindowConfigure_t cfg{"Robot2DOF Sim", 800, 600};
    auto window = NativeWindow::Create(EWindowSpec::GLFW, cfg);
    // ---------- callbacks ----------
    window->SetKeyCallback([](int key, int action) {
        if (action != KEY_PRESS) return;

        switch (key) {
            case KEY_R:
                render_enabled = !render_enabled;
                printf("render=%d\n", render_enabled);
                break;
            case KEY_S:
                sim_enabled = !sim_enabled;
                printf("sim=%d\n", sim_enabled);
                break;
            case KEY_1: robot.theta1 += 0.1; break;
            case KEY_2: robot.theta1 -= 0.1; break;
            case KEY_3: robot.theta2 += 0.1; break;
            case KEY_4: robot.theta2 -= 0.1; break;
            case KEY_I: {
                auto sols = robot.inverseKinematics(target_x, target_y);
                if (!sols.empty()) {
                    target_theta1 = sols[0].theta1;
                    target_theta2 = sols[0].theta2;
                    printf("IK target set. theta1=%.3f theta2=%.3f\n",
                           target_theta1, target_theta2);
                } else {
                    printf("IK failed: target unreachable\n");
                }
                break;
            }
        }
    });

    window->SetMouseClickCallback([](double wx, double wy) {
        target_x = wx;
        target_y = wy;
        show_target = true;
        printf("Set target (%.3f, %.3f)\n", wx, wy);
    });

    // ---------- Simulation loop ----------
    double lastt = window->GetTime();
    while (!window->ShouldClose()) {
        double now = window->GetTime();
        double dt = now - lastt;
        lastt = now;

        if (sim_enabled) {
            time_acc += dt;
            // 
            double alpha = 1.0 - exp(-4.0 * dt); //speed
            robot.theta1 += alpha * (target_theta1 - robot.theta1);
            robot.theta2 += alpha * (target_theta2 - robot.theta2);
        }

        // --- Clear screen ---
        glViewport(0, 0, 800, 600);
        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (render_enabled) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            double s = 1.2;
            glOrtho(-s, s, -s, s, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Draw workspace circle
            glColor3f(0.9f, 0.9f, 0.9f);
            window->DrawCircle(0, 0, robot.l1 + robot.l2);

            // Forward kinematics
            auto Ts = robot.forwardKinematics(robot.theta1, robot.theta2);
            Vec2 p0 = Ts[0].getXY();
            Vec2 p1 = Ts[1].getXY();
            Vec2 p2 = Ts[2].getXY();

            // Draw links
            glLineWidth(6.0f);
            glColor3f(0.2f, 0.2f, 0.8f);
            window->DrawLine(p0.x, p0.y, p1.x, p1.y);
            glColor3f(0.2f, 0.8f, 0.2f);
            window->DrawLine(p1.x, p1.y, p2.x, p2.y);
            glLineWidth(1.0f);

            // Draw joints
            glColor3f(0.1f, 0.1f, 0.1f);
            window->DrawCircle(p0.x, p0.y, 0.03);
            window->DrawCircle(p1.x, p1.y, 0.03);
            // EE
            glColor3f(0.8f, 0.1f, 0.1f);
            window->DrawCircle(p2.x, p2.y, 0.03);

            // Draw target if present
            if (show_target) {
                glColor3f(0.6f, 0.0f, 0.6f);
                window->DrawCircle(target_x, target_y, 0.02);
            }

            // Draw Jacobian vectors
            double J[2][2];
            robot.jacobian(robot.theta1, robot.theta2, J);
            double scale = 0.15;
            glColor3f(0.0f, 0.0f, 0.0f);
            window->DrawLine(p2.x, p2.y, p2.x + scale * J[0][0], p2.y + scale * J[1][0]);
            window->DrawLine(p2.x, p2.y, p2.x + scale * J[0][1], p2.y + scale * J[1][1]);
        }

        window->SwapBuffers();
        window->PollEvents();
    }

    return 0;
}
