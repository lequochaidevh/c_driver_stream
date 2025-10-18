#include "exprtk_wrapper.h"
#include <iostream>

ExprEvaluator::ExprEvaluator(const std::string &expr_str) : compiled_(false), x_(0.0) {
    sym_table_.add_variable("x", x_);
    sym_table_.add_constants();
    expression_.register_symbol_table(sym_table_);

    if (!parser_.compile(expr_str, expression_)) {
        std::cerr << "ERROR: Can not parser the expression!\n";
        std::cerr << "Details: " << parser_.error() << "\n";
        compiled_ = false;
    } else {
        compiled_ = true;
    }
}

double ExprEvaluator::eval(double x) const {
    if (!compiled_) return 0.0;
    x_ = x;
    return expression_.value();
}
