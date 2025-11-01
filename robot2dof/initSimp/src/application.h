#pragma once

#include <cmath>
#include <cstdio>
#include <vector>
#include <optional>

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