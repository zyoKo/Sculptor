#include <SculptorPch.h>

#include "Core/Application/SculptorApplication.h"
#include "Core/Memory/Memory.h"

int main()
{
	Sculptor::Core::Memory::LeakCheckpointA();

	Sculptor::Core::SculptorApplication application;

	application.Sculpt();

	Sculptor::Core::Memory::LeakCheckpointB();

	//Sculptor::Core::Memory::CheckForLeaks();

	return 0;
}
