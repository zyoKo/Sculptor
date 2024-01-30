#pragma once

namespace Sculptor::Core
{
	class Memory
	{
	public:
		static void CheckForLeaks();

		static void LeakCheckpointA();

		static void LeakCheckpointB();

	private:
		inline static _CrtMemState firstState;

		inline static _CrtMemState secondState;

		inline static _CrtMemState diffState;
	};
}
