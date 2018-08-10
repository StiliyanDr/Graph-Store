#pragma once
#include <exception>
#include <iostream>

namespace Logger
{
	void logError(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
