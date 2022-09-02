#pragma once
#include "spdlog/sinks/stdout_color_sinks.h"
#include "utils.hpp"
#include <iostream>
#include <spdlog/spdlog.h>

// Base class for all compression backends
class CompressionBase
{

	public:
	virtual std::string_view get_string_view()
	{
		spdlog::error("Not yet implemented.");
		return nullptr;
	};
	virtual std::string& get_string()
	{
		spdlog::error("Not yet implemented.");
	};

	virtual void compress_string()
	{
		spdlog::error("Not yet implemented.");
	}
	virtual void decompress_string()
	{
		spdlog::error("Not yet implemented.");
	}

	// TODO: Implement compression for dictionaries so that parts can be compressed.
	virtual void compress_file()
	{
		spdlog::error("Not yet implemented.");
	}
	virtual void decompress_file()
	{
		spdlog::error("Not yet implemented.");
	}
};
