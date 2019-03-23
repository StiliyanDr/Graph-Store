#include "Application\Application.h"
#include <iostream>

int main(int argc, const char* argv[])
{	
	if (argc != 2)
	{
		std::cerr << "Expected one argument: <directory>!\nUsage: "
				  << argv[0] << '\n';
		return 1;
	}

	Application::instance().runIn(argv[1]);

    return 0;
}
