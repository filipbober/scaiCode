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
		//const unsigned long long int TEST_RANGE = ULLONG_MAX;			// or const unsigned long long int = ~0;
		const unsigned long long int TEST_RANGE = 100;
		const int EXTERNAL_INT_LOOP_STEP = 3848537;			// results 558 loops
		const int INTERNAL_INT_LOOP_STEP = 7110873;		// results in 302 loops	
		const int MAX_DEFAULT_SET_SIZE = 32 - 1;
		

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
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			//for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				expectedValue = actionSet.isEmpty();
				actualValue = actionSetExt.isEmpty();

				Assert::AreEqual(expectedValue, actualValue, L"isEmpty", LINE_INFO());
			}
		}

		TEST_METHOD(containsBitSet)
		{
			// BitSet a
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
				{
					expectedValue = actionSet.contains((BuildOrderSearchDeprecated::BitSet)s);
					actualValue = actionSetExt.contains((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsInt)
		{
			// int bit
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
				{
					expectedValue = actionSet.contains((int)s);
					actualValue = actionSetExt.contains((int)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsAny)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{

				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				for (unsigned long long int s = TEST_RANGE; s != -1; s--)
				{
					expectedValue = actionSet.containsAny((BuildOrderSearchDeprecated::BitSet)s);
					actualValue = actionSetExt.containsAny((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsNone)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
				{
					expectedValue = actionSet.containsNone((BuildOrderSearchDeprecated::BitSet)s);
					actualValue = actionSetExt.containsNone((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(isSubsetOf)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
				{
					expectedValue = actionSet.isSubsetOf((BuildOrderSearchDeprecated::BitSet)s);
					actualValue = actionSetExt.isSubsetOf((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(getBit)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			int expectedValue;
			int actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
				{
					expectedValue = actionSet.getBit((int)s);
					actualValue = actionSetExt.getBit((int)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(addBit)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{				
				actionSet.add((int)s);
				actionSetExt.add((int)s);

				expectedValue = actionSet.contains((int)s);
				actualValue = actionSetExt.contains((int)s);

				Assert::AreEqual(expectedValue, actualValue);
			}

			//for (int i = 0; i < MAX_DEFAULT_SET_SIZE; i++)
			//{
			//	actionSet = BuildOrderSearchDeprecated::ActionSet(i);
			//	actionSetExt = ActionSetExt(i);
			//	for (int s = 0; s < INT_MAX - 2; s++)
			//	{
			//		actionSet.add((int)s);
			//		actionSetExt.add((int)s);


			//		expectedValue = actionSet.popAction();
			//		actualValue = actionSetExt.popAction();

			//		Assert::AreEqual(expectedValue, actualValue);
			//	}
			//}
		}

		TEST_METHOD(subtractBit)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{
				actionSet.subtract((int)s);
				actionSetExt.subtract((int)s);

				expectedValue = actionSet.contains((int)s);
				actualValue = actionSetExt.contains((int)s);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(addSubtractBit)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{
				actionSet.add((int)s);
				actionSetExt.add((int)s);

				expectedValue = actionSet.contains((int)s);
				actualValue = actionSetExt.contains((int)s);

				Assert::AreEqual(expectedValue, actualValue);

				actionSet.subtract((int)s);
				actionSetExt.subtract((int)s);

				expectedValue = actionSet.contains((int)s);
				actualValue = actionSetExt.contains((int)s);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(addBitSet)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{
				actionSet.add((int)s);
				actionSetExt.add((int)s);

				expectedValue = actionSet.contains((BuildOrderSearchDeprecated::BitSet)s);
				actualValue = actionSetExt.contains((BitSetExt)s);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(subtactBitSet)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{
				actionSet.subtract((BuildOrderSearchDeprecated::BitSet)s);
				actionSetExt.subtract((BitSetExt)s);

				expectedValue = actionSet.contains((BuildOrderSearchDeprecated::BitSet)s);
				actualValue = actionSetExt.contains((BitSetExt)s);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(addSubtractBitSet)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int s = 0; s < MAX_DEFAULT_SET_SIZE; s++)
			{
				actionSet.add((BuildOrderSearchDeprecated::BitSet)s);
				actionSetExt.add((BitSetExt)s);

				expectedValue = actionSet.contains((BuildOrderSearchDeprecated::BitSet)s);
				actualValue = actionSetExt.contains((BitSetExt)s);

				Assert::AreEqual(expectedValue, actualValue);

				actionSet.subtract((BuildOrderSearchDeprecated::BitSet)s);
				actionSetExt.subtract((BitSetExt)s);

				expectedValue = actionSet.contains((BuildOrderSearchDeprecated::BitSet)s);
				actualValue = actionSetExt.contains((BitSetExt)s);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(popAction)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			unsigned char expectedValue;
			unsigned char actualValue;

			//for (unsigned long long int i = TEST_RANGE; i != 0; i--)
			for (int i = 1; i < MAX_DEFAULT_SET_SIZE; i++)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				expectedValue = actionSet.popAction();
				actualValue = actionSetExt.popAction();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(nextAction)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			unsigned char expectedValue;
			unsigned char actualValue;

			for (unsigned long long int i = TEST_RANGE; i != 0; i--)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				int trailingZerosExt = actionSetExt.countTrailingZeros(i);
				int trailingZerosD = actionSet.countTrailingZeros(i);

				expectedValue = actionSet.nextAction();
				actualValue = actionSetExt.nextAction();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(countTrailingZeros)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			int expectedValue;
			int actualValue;

			//for (unsigned long long int i = TEST_RANGE; i != 0; i--)
			for (int i = 1; i < MAX_DEFAULT_SET_SIZE; i++)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				expectedValue = actionSet.countTrailingZeros(i);
				actualValue = actionSetExt.countTrailingZeros(i);								

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(countLeadingZeros)
		{
			// BITSET_SIZE must be set to 64 for test to pass (otherwise there are more leading zeros
			// due to larger bitset size
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			int expectedValue;
			int actualValue;

			//for (unsigned long long int i = TEST_RANGE; i != 0; i--)
			for (int i = 1; i < MAX_DEFAULT_SET_SIZE; i++)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				expectedValue = actionSet.countLeadingZeros(i);
				actualValue = actionSetExt.countLeadingZeros(i);

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(numActions)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			int expectedValue;
			int actualValue;

			for (int s = INT_MAX - 1; s > 0; s -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(s);
				actionSetExt = ActionSetExt(s);

				///////////////// Doesn't work, because set does not change; setExt does.




				//BitSetExt t(s);
				//int count(0);

				//while (!t.isEmpty())
				//{
				//	t.popAction();
				//	// popAction
				//	// -- doesn't delete
				//	//int nextAction = t.countTrailingZeros(s);		// s incorrect
				//	//t.subtract(nextAction);

				//	//

				//	++count;
				//}
				//int a = count;





				expectedValue = actionSet.numActions();
				actualValue = actionSetExt.numActions();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(randomAction)
		{
			BuildOrderSearchDeprecated::ActionSet actionSet = BuildOrderSearchDeprecated::ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			unsigned char expectedValue;
			unsigned char actualValue;

			for (unsigned long long int i = TEST_RANGE; i != 1; i--)
			{
				actionSet = BuildOrderSearchDeprecated::ActionSet(i);
				actionSetExt = ActionSetExt(i);

				unsigned char action = actionSet.randomAction();
				unsigned char actionExt = actionSetExt.randomAction();

				actionSetExt.add(actionExt);
				actionSet.add(action);

				expectedValue = actionSet.popAction();
				actualValue = actionSetExt.popAction();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}





	};
}