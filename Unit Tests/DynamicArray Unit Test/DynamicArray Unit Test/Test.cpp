#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Dynamic Array/DynamicArray.h"
#include <utility>
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DynamicArrayUnitTest
{		
	TEST_CLASS(DynamicArrayTest)
	{
		typedef DynamicArray<unsigned> Array;

	public:

		bool hasNullForCountAndSize(const Array& arr)
		{
			return (arr.getSize() == 0 && arr.getCount() == 0);
		}

		void fillArrayWithNumbersFromTo(Array &arr, unsigned firstNumber, unsigned lastNumber)
		{
			for (unsigned number = firstNumber; number <= lastNumber; ++number)
			{
				arr.add(number);
			}
		}

		Array createArrayFromRange(unsigned firstNumber, unsigned lastNumber)
		{
			assert(firstNumber <= lastNumber);

			Array arr(lastNumber - firstNumber + 1);
			fillArrayWithNumbersFromTo(arr, firstNumber, lastNumber);

			return arr;
		}

		bool areEqual(const Array& lhs, const Array& rhs)
		{
			const size_t lhsCount = lhs.getCount();

			if (lhsCount != rhs.getCount())
			{
				return false;
			}

			for (size_t i = 0; i < lhsCount; ++i)
			{
				if (lhs[i] != rhs[i])
				{
					return false;
				}
			}

			return true;
		}

		bool arrayConsistsOfNumbersInRange(const Array& arr, unsigned firstNumber, unsigned lastNumber)
		{
			assert(firstNumber <= lastNumber);
			unsigned rangeSize = lastNumber - firstNumber + 1;

			if (rangeSize != arr.getCount())
			{
				return false;
			}

			unsigned i = 0;

			for (unsigned number = firstNumber; number <= lastNumber; ++number)
			{
				if (arr[i++] != number)
				{
					return false;
				}
			}

			return true;
		}

		TEST_METHOD(testDefaultConstructorCreatesAnEmptyArray)
		{
			Array arr;

			Assert::IsTrue(hasNullForCountAndSize(arr));
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorFromSizeAllocatesSizeItems)
		{
			size_t size = 10;

			Array arr(size);

			Assert::AreEqual(size, arr.getSize());
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorAllocatesSizeAndGivesAccessToCountItems)
		{
			size_t size = 10;
			size_t count = 5;

			Array arr(size, count);

			Assert::AreEqual(size, arr.getSize());
			Assert::AreEqual(count, arr.getCount());
		}

		TEST_METHOD(testMoveConstructorFromEmptyArray)
		{
			Array arrayToMove;

			Array movedInto(std::move(arrayToMove));

			Assert::IsTrue(hasNullForCountAndSize(movedInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveConstructorFromNonEmptyArray)
		{
			Array arrayToMove;
			fillArrayWithNumbersFromTo(arrayToMove, 1, 5);
			size_t size = arrayToMove.getSize();

			Array movedInto(std::move(arrayToMove));
			
			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(size, movedInto.getSize(), L"The moved-into array should have the same size!");
			Assert::IsTrue(arrayConsistsOfNumbersInRange(movedInto, 1, 5));
		}

		TEST_METHOD(testCopyConstructorFromEmptyArray)
		{
			Array arrayToCopy;

			Array copy(arrayToCopy);

			Assert::IsTrue(hasNullForCountAndSize(copy));
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);

			Array copy(arrayToCopy);
			
			Assert::AreEqual(arrayToCopy.getSize(), copy.getSize());
			Assert::IsTrue(areEqual(arrayToCopy, copy));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyArray)
		{
			Array arrayToCopy;
			Array arrayToChange;

			arrayToChange = arrayToCopy;

			Assert::IsTrue(hasNullForCountAndSize(arrayToChange));
		}
			
		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyArray)
		{	
			Array arrayToCopy;
			Array arrayToChange = createArrayFromRange(1, 5);

			arrayToChange = arrayToCopy;

			Assert::IsTrue(hasNullForCountAndSize(arrayToChange));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);
			Array arrayToChange;

			arrayToChange = arrayToCopy;

			Assert::AreEqual(arrayToCopy.getSize(), arrayToChange.getSize());
			Assert::IsTrue(areEqual(arrayToCopy, arrayToChange));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);
			Array arrayToChange = createArrayFromRange(10, 15);

			arrayToChange = arrayToCopy;

			Assert::AreEqual(arrayToCopy.getSize(), arrayToChange.getSize());
			Assert::IsTrue(areEqual(arrayToCopy, arrayToChange));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyArray)
		{
			Array arrayToMove;
			Array arrayToMoveInto;

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCountAndSize(arrayToMoveInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyArray)
		{
			Array arrayToMove;
			Array arrayToMoveInto = createArrayFromRange(1, 5);

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCountAndSize(arrayToMoveInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyArray)
		{
			Array arrayToMove = createArrayFromRange(1, 5);
			size_t movedArraySize = arrayToMove.getSize();
			Array arrayToMoveInto;

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(movedArraySize, arrayToMoveInto.getSize());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arrayToMoveInto, 1, 5));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyArray)
		{
			Array arrayToMove = createArrayFromRange(10, 15);
			Array arrayToMoveInto = createArrayFromRange(1, 5);
			size_t movedArraySize = arrayToMove.getSize();

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCountAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(movedArraySize, arrayToMoveInto.getSize());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arrayToMoveInto, 10, 15));
		}

		TEST_METHOD(testAddAppendsTheItemToTheArray)
		{
			Array arr;

			for (unsigned number = 1; number <= 10; ++number)
			{
				arr.add(number);
			}

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 10));
		}

		TEST_METHOD(testAddIncrementsCount)
		{
			Array arr;

			for (unsigned i = 1; i <= 5; ++i)
			{
				arr.add(i);
				Assert::AreEqual(i, arr.getCount());
			}
		}

		TEST_METHOD(testRemoveShiftsLeftTheElementsAfterRemovedItem)
		{
			Array arr = createArrayFromRange(0, 5);

			arr.removeAt(0);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 5));
		}

		TEST_METHOD(testRemoveUpdatesTheElementsCount)
		{
			Array arr = createArrayFromRange(0, 6);

			for (int index = 6; index >= 0; index -= 2)
			{
				arr.removeAt(index);
			}

			Assert::AreEqual(3u, arr.getCount());
		}

		TEST_METHOD(testAddAtIndexShiftsRightTheNextElements)
		{
			Array arr = createArrayFromRange(11, 15);

			arr.addAt(0, 10);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 10, 15));
		}

		TEST_METHOD(testAddAtIndexAfterLastElement)
		{
			Array arr = createArrayFromRange(11, 15);

			arr.addAt(arr.getCount(), 16);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 11, 16));
		}

		TEST_METHOD(testAddAtIncrementsElementsCount)
		{
			Array arr = createArrayFromRange(0, 4);

			for (int i = 1; i <= 3; ++i)
			{
				arr.addAt(5, 100);
			}

			Assert::AreEqual(8u, arr.getCount());
		}

		TEST_METHOD(testEnsureSizeExtendsTheArrayIfPassedAGreaterSize)
		{
			Array arr = createArrayFromRange(1, 10);
			size_t size = arr.getSize();

			arr.ensureSize(size + 1);

			Assert::AreEqual(size + 1, arr.getSize());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 10),
				L"Extending the array did not kepp its elements the same!");
		}

		TEST_METHOD(testEnsureSizeDoesNothingIfPassedNotGreaterSize)
		{
			Array arr(100);

			arr.ensureSize(50);

			Assert::AreEqual(100u, arr.getSize());
		}

		TEST_METHOD(testEmpty)
		{
			Array arr = createArrayFromRange(1, 10);

			arr.empty();

			Assert::IsTrue(hasNullForCountAndSize(arr));
		}

		TEST_METHOD(testSubscriptOperator)
		{
			Array arr = createArrayFromRange(0, 4);

			for (unsigned i = 0; i <= 4; ++i)
			{
				Assert::AreEqual(i, arr[i]);
			}
		}
	};
}