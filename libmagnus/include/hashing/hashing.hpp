#pragma once
#include "utils.hpp"
#include <iomanip>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <vector>

class HashingBase
{
	public:
	HashingBase(){};
	~HashingBase(){};
	virtual std::string_view get_string_view()
	{
		spdlog::error("Not yet implemented.");
		return nullptr;
	};

	virtual std::string* get_string()
	{
		spdlog::error("Not yet implemented.");
		return nullptr;
	};

	virtual std::string hash()
	{
		spdlog::error("Not yet implemented.");
		return nullptr;
	}
};
