#pragma once
#include "exprtk.hpp"
#include <functional>
#include <string>

class ExprEvaluator {
public:
    ExprEvaluator(const std::string &expr_str);
    double eval(double x) const;
    bool valid() const { return compiled_; }

private:
    mutable double x_;
    bool compiled_;
    exprtk::symbol_table<double> sym_table_;
    exprtk::expression<double> expression_;
    exprtk::parser<double> parser_;
};
