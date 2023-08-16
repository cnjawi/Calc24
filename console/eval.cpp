#include <string>
#include <stack>

#include "utils.h"
#include "expr.h"

// 用calc和priority的中缀表达式求值

float apply(std::stack<float>& values, std::stack<char>& ops) {
	float val2 = values.top();
	values.pop();
	float val1 = values.top();
	values.pop();
	char op = ops.top();
	ops.pop();
	return calc<float>[op - 42](val1, val2);
}

float mid_eval(std::string& exp) {
	std::stack<float> values;
	std::stack<char> ops;

	int i = 0;
	while (i < exp.size()) {
		if (exp[i] == '(') {  //遇到左括号直接进栈
			ops.push(exp[i]);
		}
		else if (isdigit(exp[i])) {
			float val = 0;

			while (i < exp.size() && isdigit(exp[i])) {
				val = val * 10 + exp[i] - '0';
				i++;
			} // 处理多位数字的情况

			values.push(val);
			i--;
		}
		else if (exp[i] == ')') {  // 计算括号内的内容
			while (!ops.empty() && ops.top() != '(') {
				values.push(apply(values, ops));
			}
			if (!ops.empty()) ops.pop();
		}
		else {
			while (!ops.empty() && priority[ops.top() - 42] >= priority[exp[i] - 42]) {
				values.push(apply(values, ops));
			}
			ops.push(exp[i]);
		}
		++i;
	}
	while (!ops.empty()) {
		values.push(apply(values, ops));
	}
	return values.top();
}
