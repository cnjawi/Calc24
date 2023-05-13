#pragma once
#include <string>

void calculate(char*[6]);  //执行calc模块，计算输入值的24点结果

int* gen_exrcs();  //随机生成四个数字，确保有解
bool matched(const std::string&, int[4]);  //判断输入是否与题目一致
char find_illegal(const std::string&);  //判断有无非法字符
float mid_eval(std::string&);  //中缀表达式求值

constexpr char HELP[] = "cqlt option [args]\n\
options:\n\
  HELP\t---\t打印说明页\n\
  calc\t---\t计算24点\t[args] : 四个[1,13]间的整数\n\
  eval\t---\t四则运算求值\t[args] : 中缀表达式(一定要用引号括起来)\n\
  play\t---\t24点游戏\n";
