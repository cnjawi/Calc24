#include <cstdlib>
#include <iostream>

#include "utils.h"
#include "../cqltr/cqltr.h"

void calculate(char* argv[6])
{
	int exrcs[4], tmp;
	for (int i = 0; i < 4; i++)
    {
		tmp = atoi(argv[i + 2]);
		if (tmp == 0)
		{
			std::cout << "None-numerical Character or 0 Was Given\n";
			return;
		}
		else
		{
			exrcs[i] = tmp;
		}
	}
	std::cout << figure(exrcs);
}
