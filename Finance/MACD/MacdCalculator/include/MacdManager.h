#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "MacdCalculator.h"

class MacdManager
{
private:
	enum class Period : int
	{
		Day,
		Week,
	};

	enum class Stock : int
	{
		SSE_600018,
	};

public:
	MacdManager();
	~MacdManager();
	void start();
	void demonstrateMACDCalculator(Stock stock, Period period);

private:
	static std::unordered_map<Period, std::string> dataMap_;
	static std::unordered_map<Stock, std::string> stockMap_;

};

