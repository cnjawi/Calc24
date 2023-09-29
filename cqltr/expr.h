#pragma once
#include <string>
#include <stdint.h>

/*表达式*/
struct expr_base {
    float val;      // 数值
    char opt;       // 运算符，包含 + - * / 以及表示数值节点的占位符&
    expr_base* next[2];  // 左右子节点

    expr_base();
    void eval();                      // 单层求值
    void reval();                     // 递归求值
    std::string str(char pre = '+', bool AfterSubOrDiv = false);  // 递归转化为中缀表达式
    void set(char, expr_base*, expr_base*);
};

struct expr : expr_base {
    uint8_t pos;    // 记录取值的位置，位操作

    expr();
    void set(char operation, expr* left, expr* right);
};

/*
把运算符映射到lambda表达式
可以使用 std::map 或 std::unordered_map，
但是就这一种情况而言
    字符    *   +   -   /
    ascii  42  43  45  47
字符的ascii值相近，可以直接减42作为数组索引
以取到对应的lambda函数，中间空位上随便写点什么即可
*/

/*
存放lmabda表达式的数组常量，
形参和返回值类型均为T
可以是任意数字类型
*/
template <typename T>
constexpr T(* const calc[6])(T, T) =
{
[](T x, T y) -> T { return x * y; },
[](T x, T y) -> T { return x + y; },
[](T, T) -> T { return 0; },
[](T x, T y) -> T { return x - y; },
[](T, T) -> T { return 0; },
[](T x, T y) -> T { return x / y; },
};  //在常量表达式中使用lambda是c++17后支持的特性


/*
运算符映射到优先级，同样用数组解决
    运算符   *   +   -   /
    优先级   2   1   1   2
中间的空位设为0即可
*/
constexpr const uint8_t priority[6]{ 2, 1, 0, 1, 0, 2 };