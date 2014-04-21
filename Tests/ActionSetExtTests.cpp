#include "stdafx.h"
#include "CppUnitTest.h"


#include "../StarcraftBuildOrderSearch/Source/starcraftsearch/Common.h"
#include "../StarcraftBuildOrderSearch/Source/starcraftsearch/ActionSet.hpp"
#include "../StarcraftBuildOrderSearch/Source/starcraftsearch/ActionSetExt.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace BuildOrderSearch;

namespace ActionSetTestUnitTests
{
	TEST_CLASS(ActionSetExtTest)
	{
	private:
		//const unsigned long long int TEST_RANGE = ULLONG_MAX;
		const unsigned long long int TEST_RANGE = 10;

	public:
		//TEST_MODULE_INITIALIZE(ModuleStartup)
		//{
		//	Logger::WriteMessage("The test is starting");
		//}

		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
		}

		TEST_METHOD(isEmpty)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				expectedValue = actionSet.isEmpty();
				actualValue = actionSetExt.isEmpty();

				Assert::AreEqual(expectedValue, actualValue, L"isEmpty", LINE_INFO());
			}
		}

		TEST_METHOD(containsBitSet)
		{
			// BitSet a
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (unsigned long long int s = TEST_RANGE; s != -1; s--)
				{
					expectedValue = actionSet.contains((BitSet)s);
					actualValue = actionSetExt.contains((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsInt)
		{
			// int bit
			// TODO: check if this function is called at all
		}

	};
}