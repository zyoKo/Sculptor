#include <SculptorPch.h>

#include "Memory.h"

#define _CRTDBG_MAP_ALLOC
#include <iostream>
#include <crtdbg.h>

namespace Sculptor::Core
{
	void Memory::CheckForLeaks()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtDumpMemoryLeaks();
	}

	void Memory::LeakCheckpointA()
	{
		_CrtMemCheckpoint(&firstState);
	}

	void Memory::LeakCheckpointB()
	{
		_CrtMemCheckpoint(&secondState);

		if (_CrtMemDifference(&diffState, &firstState, &secondState))
		{
			_CrtMemDumpStatistics(&diffState);
		}
	}
}
