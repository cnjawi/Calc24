#include <sstream>
#include <string>

#include "expr.h"

expr::expr() : val(0.), opt('&'), pos(0), next{nullptr,nullptr} {};

/*深度为1的单层求值*/
void expr::eval() {
    if (opt != '&') val = calc<float>[opt - 42](next[0]->val, next[1]->val);
}

/*递归求值 reCURSIVE evalUATION*/
void expr::reval() {
    if (opt != '&') {
        next[0]->reval();
        next[1]->reval();
        val = calc<float>[opt - 42](next[0]->val, next[1]->val);
    }
}

std::string expr::str(char pre, bool AfterSubOrDiv) {
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

void expr::set(char operation, expr* left, expr* right) {
    next[0] = left;
    next[1] = right;
    opt = operation;
    pos = left->pos | right->pos;
    eval();
}