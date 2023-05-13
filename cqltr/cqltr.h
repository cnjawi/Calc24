#pragma once

#include <string>

#ifndef CQLTR_EXPORTS
#define CQLTR_API __declspec(dllexport)
#else
#define CQLTR_API __declspec(dllimport)
#endif

extern "C" CQLTR_API const char* figure(int[4]);
extern "C" CQLTR_API bool validate(int[4]);