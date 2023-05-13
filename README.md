# 24点计算器

做一个24点计算器，也算是实现童年梦想了:-P 。

## 算法

24点的原始玩法是把一副扑克牌去掉大小王后，从中选取4张牌计算，JQK对应11, 12, 13，所以在 [1, 13] 区间范围内选取四个整数（可以重复）作为题目，目标是通过加减乘除四种运算得到 24 。显然每道题的排列组合方式都是有限的，可以通过计算机把所有可能结果穷举出来。

穷举所有情况的前提是可以表示出一个情况。加减乘除都是二元运算，因此可以用二叉树表示。在 `expr.h` 和 `expr.cpp` 中实现了可以求值和转化为中缀表达式的对象 expr，只实现了默认构造函数，通过 set() 方法设值

### 原始实现

非常暴力，不考虑数值重复问题。把算式包含的元素数量称为“深度”。

* 输入的四个数深度为一，填入数组unary[4]
* 所有深度为二的情况可以通过unary中的元素组合得到，填入数组binary[36] (有36个情况)
* unary和binary排列的结果即为深度为三的所有情况，填入数组ternary[864]；
* 深度为四的算式有2+2或1+3两种可能，分别填入quad1[3780], quad2[20736]

在quad1和quad2中找到所有值为24的exp对象，与single[4]比较以排除所有重复使用同一数的情况，拿到满足要求的结果

这个算法能实现的前提是算式深度只到4，且仅输入4个数。因为随着深度的增加，数组大小是爆炸式增长的。

不过所需内存固定，可以一次性申请好，而且在过程中不涉及判断，生成速度很快。

### 优化

保持原先迭代的思想不变，考虑如何解决数字重复问题。

扩充expr对象，记录表达式中所有数字的“位置”。比如输入 1 2 3 4，表达式 1+3 来自第一个数和第三个数，位置为 0101（反过来看）；表达式 1*4 来自第一个数和第四个数，位置为 1001。要想确定两个表达式是否重复使用了同一数字，只需比较它们位置的对应位上是否同时出现1。

一个表达式的位置具有4bit信息量，而计算机中一个变量最小也有1Byte，所以如果用大小为4的数组来记录，会造成很大的空间浪费。

可以用位运算来解决这个问题。每个位置可以看作一个二进制数，共有16中可能，长为4bit。用长为1Byte的uint8_t来保存位置，空间利用率从1/8上升到1/2。判断重复的方法正好就是按位与运算的逻辑。将两个表达式合成新的表达式时，通过按位或运算组合两个位置。

这样只需在原有生成过程中增加一个判断语句即可。ternary数组大小减小到432，quad1减小到648，quad2减小到2592。

## 功能

除计算24点这个核心功能之外，还实现了一个简易的24点游戏。由程序随机生成确保有解的四个数字，用户输入计算式，程序判断输入的计算式是否为其中一解。这里最大的难点就是中缀表达式的求值。这个函数单独放在`midexpr.cpp`中，实现过程中复用了`expr.cpp`中的两个常量，所以同时 include "expr.h"。

程序通过终端执行。用powershell打开程序所在目录，输入`.\cqlt help`打印帮助页，以下是运行示例

```powershell

PS C:\path\to\this\program> ./cqlt help
cqlt option [args]
options:
  help  ---     打印说明页
  calc  ---     计算24点        [args] : 四个[1,13]间的整数
  eval  ---     四则运算求值    [args] : 中缀表达式(一定要用引号括起来)
  play  ---     24点游戏

PS C:\path\to\this\program> ./cqlt calc 3 3 8 8
8/(3-(8/3))
Total: 1 

PS C:\path\to\this\program> ./cqlt play
可用符号: + - * / ( ), 不能输入空格
输入 exit 退出, ans 获取答案

2 7 7 8

>>> ans
2+7+7+8
2+8+7+7
7+2+7+8
7+7+2+8
7+8+2+7
8+2+7+7
8*(2+7/7)
8+7+2+7
Total: 8

>>> 2+7+7+8
Correct

>>> exit

PS C:\path\to\this\program> ./cqlt eval "2*(1+9/3)"  // eval 主要是用于调试代码的
8
```

## 改进
本仓库迁移自原Calc24仓库，代码改动很大，主要有

* 将计算24点的核心功能独立为动态链接库，用C语言样式导出`const char* figure(int[4])`和`bool validate(int[4])`两个函数，分别用于求解和验证是否有解。支持通过python调用函数。
* 命令行交互的几个模块的实现放到了对应的源文件中，通过 utils.h 统一调用

事实证明，gen_results.py运行的大部分时间都花在了输入输出上。直接调用dll遍历所有可能情况，运行时间不到1秒。而如果使用cqlt_old.exe，通过读取控制台输出的方式做通信，需要50多秒，动态链接库的优势可见一斑

### 目录结构

`Console/*` -- 命令行工具

`cqltr/*` -- 动态链接库
