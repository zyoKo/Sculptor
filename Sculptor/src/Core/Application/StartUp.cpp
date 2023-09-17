#include <SculptorPch.h>

#include "Core/Application/SculptorApplication.h"

int main()
{
	Sculptor::Core::SculptorApplication application;

	try
	{
		application.SculptApplication();
	}
	catch( const std::exception& exception)
	{
		std::cerr << exception.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}