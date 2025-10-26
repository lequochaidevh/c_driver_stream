// src/main.cpp
#include <cmath>
#include <cstdio>
#include <vector>
#include <optional>
#include <GLFW/glfw3.h>


double target_theta1 = 0.0;
double target_theta2 = 0.0;
// ---------- Small math helpers ----------
struct Vec2 {
    double x=0, y=0;
    Vec2() {}
    Vec2(double _x,double _y):x(_x),y(_y){}
};
struct Mat4 {
    double m[4][4];
    Mat4(){ for(int i=0;i<4;i++) for(int j=0;j<4;j++) m[i][j] = (i==j?1.0:0.0); }
    static Mat4 translate(double tx, double ty, double tz=0){
        Mat4 T; T.m[0][3]=tx; T.m[1][3]=ty; T.m[2][3]=tz; return T;
    }
    static Mat4 rotZ(double theta){
        Mat4 R;
        double c = cos(theta), s = sin(theta);
        R.m[0][0]=c; R.m[0][1]=-s; R.m[1][0]=s; R.m[1][1]=c;
        return R;
    }
    Mat4 operator*(const Mat4& o) const {
        Mat4 r;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                r.m[i][j]=0;
                for(int k=0;k<4;k++) r.m[i][j]+=m[i][k]*o.m[k][j];
            }
        }
        return r;
    }
    Vec2 getXY() const { return Vec2(m[0][3], m[1][3]); }
};

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

// ---------- OpenGL drawing helpers ----------
void drawCircle(double cx, double cy, double r, int segs=24){
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(cx, cy);
    for(int i=0;i<=segs;i++){
        double a = (2.0*M_PI*i)/segs;
        glVertex2d(cx + cos(a)*r, cy + sin(a)*r);
    }
    glEnd();
}

void drawLine(double x1,double y1,double x2,double y2){
    glBegin(GL_LINES);
    glVertex2d(x1,y1);
    glVertex2d(x2,y2);
    glEnd();
}

// ---------- GLFW callbacks ----------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action != GLFW_PRESS) return;
    if(key==GLFW_KEY_R){ render_enabled = !render_enabled; printf("render=%d\n", render_enabled); }
    if(key==GLFW_KEY_S){ sim_enabled = !sim_enabled; printf("sim=%d\n", sim_enabled); }
    if(key==GLFW_KEY_1){ robot.theta1 += 0.1; }
    if(key==GLFW_KEY_2){ robot.theta1 -= 0.1; }
    if(key==GLFW_KEY_3){ robot.theta2 += 0.1; }
    if(key==GLFW_KEY_4){ robot.theta2 -= 0.1; }
    if(key==GLFW_KEY_I){
        // try IK to current target
        auto sols = robot.inverseKinematics(target_x, target_y);
        if(!sols.empty()){
            // choose first solution (could pick nearest)
            // robot.theta1 = sols[0].theta1; 
            // robot.theta2 = sols[0].theta2;
            
            target_theta1 = sols[0].theta1;
            target_theta2 = sols[0].theta2;
            printf("IK target set. theta1=%.3f theta2=%.3f\n", target_theta1, target_theta2);
            //printf("IK applied. theta1=%.3f theta2=%.3f\n", robot.theta1, robot.theta2);
        } else {
            printf("IK failed: target unreachable\n");
        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if(button==GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS){
        double mx, my;
        int w,h;
        glfwGetCursorPos(window, &mx, &my);
        glfwGetWindowSize(window, &w, &h);
        // map screen coords to world [-1,1] assuming ortho same as draw
        double wx = (mx / w) * 2.0 - 1.0;
        double wy = 1.0 - (my / h) * 2.0;
        // scale world to size 1.2 for view -> our robot lengths ~1.0, scale accordingly
        target_x = wx * 1.2;
        target_y = wy * 1.2;
        show_target = true;
        printf("Set target (%.3f, %.3f)\n", target_x, target_y);
    }
}

// ---------- Main ----------
int main(){
    if(!glfwInit()){
        fprintf(stderr,"GLFW init failed\n");
        return -1;
    }
    GLFWwindow* window = glfwCreateWindow(800,600,"Robot 2DOF Sim", NULL, NULL);
    if(!window){ glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // main loop
    double lastt = glfwGetTime();

    double speed = 1.0; // radian/giây (điều chỉnh tốc độ)
    while(!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        double dt = now - lastt; lastt = now;
        if(sim_enabled){
            time_acc += dt;
            // example: animate theta2 sinusoidally
            // robot.theta2 = 0.6 * sin(time_acc * 1.5);
            // robot.theta1 = 0.3 * sin(time_acc * 0.8);

            double alpha = 1.0 - exp(-4.0 * dt); // hệ số giảm dần
            robot.theta1 += alpha * (target_theta1 - robot.theta1);
            robot.theta2 += alpha * (target_theta2 - robot.theta2);
        }

        glViewport(0,0,800,600);
        glClearColor(0.95f,0.95f,0.95f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if(render_enabled){
            // simple ortho mapping: world coords in [-1.2,1.2]
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            double s=1.2;
            glOrtho(-s, s, -s, s, -1, 1);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            // Draw workspace circle
            glColor3f(0.9f,0.9f,0.9f);
            drawCircle(0,0, robot.l1+robot.l2);

            // Forward kinematics
            auto Ts = robot.forwardKinematics(robot.theta1, robot.theta2);
            Vec2 p0 = Ts[0].getXY(); // base (0,0)
            Vec2 p1 = Ts[1].getXY(); // joint1 end
            Vec2 p2 = Ts[2].getXY(); // end-eff

            // Draw links
            glLineWidth(6.0f);
            glColor3f(0.2f,0.2f,0.8f); drawLine(p0.x,p0.y,p1.x,p1.y);
            glColor3f(0.2f,0.8f,0.2f); drawLine(p1.x,p1.y,p2.x,p2.y);
            glLineWidth(1.0f);

            // Draw joints
            glColor3f(0.1f,0.1f,0.1f); drawCircle(p0.x,p0.y,0.03);
            drawCircle(p1.x,p1.y,0.03);
            // EE
            glColor3f(0.8f,0.1f,0.1f); drawCircle(p2.x,p2.y,0.03);

            // Draw target if present
            if(show_target){
                glColor3f(0.6f,0.0f,0.6f);
                drawCircle(target_x, target_y, 0.02);
            }

            // Optionally draw Jacobian vectors (visualization)
            double J[2][2];
            robot.jacobian(robot.theta1, robot.theta2, J);
            // draw small arrows representing columns of J at end-effector
            double scale = 0.15;
            glColor3f(0.0f,0.0f,0.0f);
            drawLine(p2.x, p2.y, p2.x + scale * J[0][0], p2.y + scale * J[1][0]);
            drawLine(p2.x, p2.y, p2.x + scale * J[0][1], p2.y + scale * J[1][1]);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
