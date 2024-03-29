# 24点计算器

24点游戏的规则是从一副去掉大小王的扑克牌中选取4张，得到对应的4个数字(JQK对应11, 12, 13)，然后用这四个数字进行加减乘除运算，得到24。在给定的4个数字之间，显然只存在数量有限的运算的组合，可以通过计算机把所有可能结果穷举出来。

## 算法

### 表达式树

穷举所有情况的前提是可以表示出一个情况。加减乘除都是二元运算，因此可以用二叉树表示一个四则运算。在 `cqltr/expr.cpp` 中实现了可以求值和转化为中缀表达式的对象 expr，只实现了默认构造函数，通过 set() 方法设值

### 穷举

这部分的代码存放在 `cqltr/cqltr.cpp` 中。

把一个表达式中包含的数字数量称为这个表达式的"长度"，如 `1` 的长度为 1，`1+2*2` 的长度为 3。

注意到所有长度为 2 的表达式都可以由长度为 1 的表达式合成；所有长度为 3 的表达式由长度分别为 1, 2 的表达式合成；而长度为 4 的表达式可以通过长度为 2 的表达式合成，也可以通过长度分别为 1, 3 的表达式合成。

因此，要穷举出24点所有可能，可以

* 将输入的四个长度为一的数字填入数组unary[4]
* 所有长度为二的情况可以通过unary中的元素组合得到，填入数组binary[36] (有36个情况)
* unary和binary排列的结果即为长度为三的所有情况，填入数组ternary[864]；
* 长度为四的算式有2+2或1+3两种可能，分别填入quad1[3780], quad2[20736]

注意到这个过程中会出现大量重复使用同一数字的表达式，接下来考虑如何解决数字重复问题。

扩充expr对象，记录表达式中所有数字的“位置”。比如输入 1 2 3 4，表达式 1+3 来自第一个数和第三个数，位置为 0101；表达式 1*4 来自第一个数和第四个数，位置为 1001。要想确定两个表达式是否重复使用了同一数字，只需比较它们位置的对应位上是否同时出现1。

一个表达式的位置具有4bit信息量，而计算机中一个变量最小也有1Byte，所以如果用大小为4的数组来记录，会造成很大的空间浪费。

可以用位运算来解决这个问题。每个位置可以看作一个二进制数，共有16中可能，长为4bit。用长为1Byte的uint8_t来保存位置，空间利用率从1/8上升到1/2。

判断重复的方法正好就是按位与运算的逻辑。将两个表达式合成新的表达式时，通过按位或运算组合两个位置。

这样只需在原有生成过程中增加一个判断语句即可。ternary数组大小减小到432，quad1减小到648，quad2减小到2592。

当然，这么做不能避免quad1和quad2中存在一致的结果，所以在输入前还需判断一下是否存在相同的结果。

## 功能

这部分代码存放在 `console/` 文件夹下。

除计算24点这个核心功能之外，还实现了一个简易的24点游戏。由程序随机生成确保有解的四个数字，用户输入计算式，程序判断输入的计算式是否为其中一解。

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

## 优化
本仓库迁移自原Calc24仓库，代码有大幅改动，主要有

* 将计算24点的核心功能独立为动态链接库，用C语言样式导出`const char* figure(int[4])`和`bool validate(int[4])`两个函数，分别用于求解和验证是否有解。支持通过python调用函数。
* 代码只用到了标准库，本身与平台无关。然而先前由于使用 Visual Studio 管理项目，只能放在 Windows 下编译。现在迁移到了cmake，实现源码跨平台。

另外用python写了一个调用编译完动态链接库的脚本，遍历所有24点可能的情况，并把结果存储在sqlite数据库中。
