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
		const unsigned long long int TEST_RANGE = 100;
		const int EXTERNAL_INT_LOOP_STEP = 3848537;			// results 558 loops
		const int INTERNAL_INT_LOOP_STEP = 7110873;		// results in 302 loops	

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
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i >= 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s <= INT_MAX - 1; s += INTERNAL_INT_LOOP_STEP)
				{
					expectedValue = actionSet.contains((int)s);
					actualValue = actionSetExt.contains((int)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsAny)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				for (unsigned long long int s = TEST_RANGE; s != -1; s--)
				{
					expectedValue = actionSet.containsAny((BitSet)s);
					actualValue = actionSetExt.containsAny((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(containsNone)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				for (unsigned long long int s = TEST_RANGE; s != -1; s--)
				{
					expectedValue = actionSet.containsNone((BitSet)s);
					actualValue = actionSetExt.containsNone((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(isSubsetOf)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				for (unsigned long long int s = TEST_RANGE; s != -1; s--)
				{
					expectedValue = actionSet.isSubsetOf((BitSet)s);
					actualValue = actionSetExt.isSubsetOf((BitSetExt)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(getBit)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (int i = INT_MAX - 1; i >= 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s <= INT_MAX - 1; s += INTERNAL_INT_LOOP_STEP)
				{
					expectedValue = actionSet.getBit((int)s);
					actualValue = actionSetExt.getBit((int)s);

					Assert::AreEqual(expectedValue, actualValue, L"contains", LINE_INFO());
				}
			}
		}

		TEST_METHOD(addBit)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			Action expectedValue;
			ActionExt actualValue;

			for (int i = INT_MAX - 1; i >= 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				int s = i + 1;
				actionSet.add((int)s);
				actionSetExt.add((int)s);


				expectedValue = actionSet.popAction();
				actualValue = actionSetExt.popAction();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(subtractBit)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			Action expectedValue;
			ActionExt actualValue;

			for (int i = INT_MAX - 1; i > 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				int s = i - 1;
				actionSet.subtract((int)s);
				actionSetExt.subtract((int)s);

				expectedValue = actionSet.popAction();
				actualValue = actionSetExt.popAction();

				Assert::AreEqual(expectedValue, actualValue);
			}
		}

		TEST_METHOD(addSubtractBit)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			int expectedValue;
			int actualValue;

			for (int i = INT_MAX - 1; i >= 0; i -= EXTERNAL_INT_LOOP_STEP)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);
				for (int s = 0; s <= INT_MAX / 4; s += INTERNAL_INT_LOOP_STEP)
				{
					actionSet.add((int)s);
					actionSetExt.add((int)s);

					actionSet.subtract((int)s);
					actionSetExt.subtract((int)s);

					for (int j = 0; j < 64; j++)
					{
						expectedValue = actionSet[j];
						actualValue = actionSetExt[j];

						Assert::AreEqual(expectedValue, actualValue);
					}

				}
			}
		}

		TEST_METHOD(addBitSet)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE-1; i != -1; i--)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				actionSet.add((BitSet)BitSet(i + 1));
				actionSetExt.add((BitSetExt)BitSetExt(i + 1));

				for (int j = 0; j < 64; j++)
				{
					expectedValue = actionSet[j];
					actualValue = actionSetExt[j];

					Assert::AreEqual(expectedValue, actualValue);
				}

			}
		}

		TEST_METHOD(subtactBitSet)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				actionSet.subtract((BitSet)BitSet(i - 1));
				actionSetExt.subtract((BitSetExt)BitSetExt(i - 1));

				for (int j = 0; j < 64; j++)
				{
					expectedValue = actionSet[j];
					actualValue = actionSetExt[j];

					Assert::AreEqual(expectedValue, actualValue);
				}

			}
		}

		TEST_METHOD(addSubtractBitSet)
		{
			ActionSet actionSet = ActionSet(0ull);
			ActionSetExt actionSetExt = ActionSetExt(0ull);

			bool expectedValue;
			bool actualValue;

			for (unsigned long long int i = TEST_RANGE; i != -1; i--)
			{
				actionSet = ActionSet(i);
				actionSetExt = ActionSetExt(i);

				actionSet.add((BitSet)BitSet(i));
				actionSetExt.add((BitSetExt)BitSetExt(i));

				actionSet.subtract((BitSet)BitSet(i));
				actionSetExt.subtract((BitSetExt)BitSetExt(i));

				for (int j = 0; j < 64; j++)
				{
					expectedValue = actionSet[j];
					actualValue = actionSetExt[j];

					Assert::AreEqual(expectedValue, actualValue);
				}

			}
		}



	};
}