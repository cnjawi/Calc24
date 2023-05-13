#include <iostream>
#include <string>

#include "utils.h"
#include "../cqltr/cqltr.h"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << HELP;
		return 0;
	}
	std::string option(argv[1]);
	if (option == "calc")
	{
		if (argc > 6)
		{
			std::cout << "Too Many Arguments\n";
			return -1;
		}
		else if (argc < 6)
		{
			std::cout << "Too Few Arguments\n";
			return -1;
		}
		calculate(argv);
	}
	else if (option == "play")
	{
		int* exrcs = gen_exrcs();
		std::cout << "可用符号: + - * / ( ), 不能输入空格\n输入 exit 退出, ans 获取答案\n\n";
		for (int i = 0; i < 4; i++) std::cout << exrcs[i] << ' ';
		std::cout << "\n";
		std::string exp;
		while (true)
		{
			std::cout << "\n>>> ";
			std::getline(std::cin, exp);
			if (exp == "exit")
			{
				std::cout << '\n';
				break;
			}
			else if (exp == "ans")
			{
				std::cout << figure(exrcs);
				continue;
			}
			char illegal;
			if ((illegal = find_illegal(exp)) != '\0')
			{
				std::cout << "illegal character \"" << illegal << "\" occurs\n";
				continue;
			}
			float r = mid_eval(exp);
			if (r > 23.99 && r < 24.01)
			{
				if (matched(exp, exrcs)) std::cout << "Correct\n";
				else std::cout << "Wrong, numbers do not match\n";
			}
			else std::cout << "Wrong, result is " << r << "\n";
		}
	}
	else if (option == "eval")
	{
		std::string exp(argv[2]);
		std::cout << mid_eval(exp) << '\n';
	}
	else std::cout << HELP;
	
	return 0;
}
