#include <sstream>
#include <stdint.h>
#include <string>

#include "expr.h"

expr_base::expr_base() : val(0.), opt('&'), next{nullptr,nullptr} {};

/*深度为1的单层求值*/
void expr_base::eval() {
    if (opt != '&') val = calc<float>[opt - 42](next[0]->val, next[1]->val);
}

/*递归求值 reCURSIVE evalUATION*/
void expr_base::reval() {
    if (opt != '&') {
        next[0]->reval();
        next[1]->reval();
        val = calc<float>[opt - 42](next[0]->val, next[1]->val);
    }
}

std::string expr_base::str(char pre, bool AfterSubOrDiv) {
    std::stringstream ss;  // stringstream 非常适合格式化输出一长串数据
    bool JudgePos = (opt == '-' || opt == '/');
    if (opt == '&') {
        // 当前节点表示数字时
        ss << val;
    }
    else if (priority[pre - 42] > priority[opt - 42] || AfterSubOrDiv) {
        // 父节点的运算符优先级更大 或 在减号或除号之后
        ss << '(' << next[0]->str(opt) << opt << next[1]->str(opt, JudgePos) << ')';
    }
    else {
        ss << next[0]->str(opt) << opt << next[1]->str(opt, JudgePos);
    }
    return ss.str();
}

void expr_base::set(char operation, expr_base* left, expr_base* right) {
    next[0] = left;
    next[1] = right;
    opt = operation;
    eval();
}

expr::expr() : pos(0) {}

void expr::set(char operation, expr* left, expr* right) {
    next[0] = left;
    next[1] = right;
    opt = operation;
    pos = left->pos | right->pos;
    eval();
}