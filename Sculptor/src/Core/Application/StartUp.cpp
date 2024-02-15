#include <SculptorPch.h>

#include "Core/Application/SculptorApplication.h"
#include "Core/RenderAPI/Memory/Memory.h"

int main()
{
	Sculptor::Core::Memory::LeakCheckpointA();

	Sculptor::Core::SculptorApplication application;

	application.Sculpt();

	Sculptor::Core::Memory::LeakCheckpointB();

	return 0;
}
