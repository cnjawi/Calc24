#include <algorithm>
#include <ctime>
#include <string>
#include <stack>

#include "utils.h"
#include "../cqltr/cqltr.h"

int* gen_exrcs()
{
	srand((unsigned int)time(nullptr));
	int* exrcs = new int[4];
	do
	{
		for (int i = 0; i < 4; i++) exrcs[i] = rand() % 13 + 1;
	} while (!validate(exrcs));
	std::sort(exrcs, exrcs + 4);
	return exrcs;
}

bool matched(const std::string& s, int exrcs[4])
{
	int arr[16], idx = 0, i = 0, j, val = 0;
	while (i < s.size())
	{
		if (isdigit(s[i]))
		{
			j = i;
			while (isdigit(s[j]))
			{
				val = val * 10 + s[j] - '0';
				j += 1;
			}
			arr[idx] = val;
			val = 0;
			idx += 1;
			i = j;
		}
		else
		{
			i += 1;
		}
	}
	if (idx != 4)
	{
		return false;
	}
	std::sort(arr, arr + 4);
	for (int i = 0; i < 4; i++)
	{
		if (exrcs[i] != arr[i]) return false;
	}
	return true;
}

char find_illegal(const std::string& s)
{
	for (char e : s)
	{
		if (isdigit(e) || e == '+' || e == '-' || e == '*' || e == '/' || e == '(' || e == ')')
		{
			continue;
		}
		return e;
	}
	return '\0';
}
