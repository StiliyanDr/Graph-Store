#include "stdafx.h"
#include "CppUnitTest.h"
#include "Dynamic Array/DynamicArray.h"
#include "Utility.h"
#include <assert.h>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DynamicArrayUnitTest
{		
	TEST_CLASS(DynamicArrayTest)
	{
		using Array = DynamicArray<unsigned>;

		static Array createArrayFromRange(unsigned start,
			                              unsigned end)
		{
			auto arr = Array{};
			fillArrayWithNumbersFromTo(arr, start, end);

			return arr;
		}

		static void fillArrayWithNumbersFromTo(Array &arr,
			                                   unsigned first,
			                                   unsigned last)
		{
			assert(arr.isEmpty());
			assert(first <= last);
			
			arr.reserve(last - first + 1);

			for (auto i = first; i <= last; ++i)
			{
				arr.add(i);
			}
		}

		static bool areEqual(const Array& lhs, const Array& rhs)
		{
			auto rhsIterator = rhs.getConstIterator();
			auto equalsCorrespondingItem =
				[&rhsIterator](auto lhsItem)
			{
				return lhsItem == *(rhsIterator++);
			};

			return lhs.getSize() == rhs.getSize()
				   && allOf(lhs.getConstIterator(),
					        equalsCorrespondingItem);
		}

		static bool arrayConsistsOfNumbersInRange(const Array& arr,
			                                      unsigned start,
			                                      unsigned end)
		{
			assert(start <= end);

			auto rangeSize = end - start + 1;
			auto equalsCorrespondingNumber =
				[&current = start](auto number)
			{
				return number == current++;
			};

			return arr.getSize() == rangeSize
				   && allOf(arr.getConstIterator(),
						    equalsCorrespondingNumber);
		}

		static bool hasNullForCapacityAndSize(const Array& arr)
		{
			return arr.getCapacity() == 0 && arr.getSize() == 0;
		}

		static bool stringIsMovedFrom(const std::string& s)
		{
			return s == "";
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return std::strcmp(lhs, rhs) == 0;
		}

	public:
		TEST_METHOD(testDefaultConstructorCreatesAnEmptyArray)
		{
			Array arr;

			Assert::IsTrue(hasNullForCapacityAndSize(arr));
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorFromCapacity)
		{
			std::size_t capacity = 10;

			Array arr(capacity);

			Assert::AreEqual(capacity, arr.getCapacity());
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorAllocatesCapacityAndGivesAccessToSizeItems)
		{
			std::size_t capacity = 10;
			std::size_t size = 5;

			Array arr(capacity, size);

			Assert::AreEqual(size, arr.getSize());
			Assert::AreEqual(capacity, arr.getCapacity());
		}

		TEST_METHOD(testCtorWithSizeExceedingCapacityThrowsException)
		{
			try
			{
				Array arr(10, 11);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (std::invalid_argument& e)
			{
				Assert::IsTrue(areEqual("Size must not exceed capacity!", e.what()));
			}
		}

		TEST_METHOD(testMoveConstructorFromEmptyArray)
		{
			Array arrayToMove;

			Array movedInto(std::move(arrayToMove));

			Assert::IsTrue(hasNullForCapacityAndSize(movedInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveConstructorFromNonEmptyArray)
		{
			Array arrayToMove;
			fillArrayWithNumbersFromTo(arrayToMove, 1, 5);
			std::size_t capacity = arrayToMove.getCapacity();

			Array movedInto(std::move(arrayToMove));
			
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, movedInto.getCapacity(), L"The moved-into array should have the same capacity!");
			Assert::IsTrue(arrayConsistsOfNumbersInRange(movedInto, 1, 5));
		}

		TEST_METHOD(testCopyConstructorFromEmptyArray)
		{
			Array arrayToCopy;

			Array copy(arrayToCopy);

			Assert::IsTrue(hasNullForCapacityAndSize(copy));
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);

			Array copy(arrayToCopy);
			
			Assert::AreEqual(arrayToCopy.getCapacity(), copy.getCapacity());
			Assert::IsTrue(areEqual(arrayToCopy, copy));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyArray)
		{
			Array arrayToCopy;
			Array arrayToChange;

			arrayToChange = arrayToCopy;

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToChange));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyArray)
		{	
			Array arrayToCopy;
			Array arrayToChange = createArrayFromRange(1, 5);

			arrayToChange = arrayToCopy;

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToChange));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);
			Array arrayToChange;

			arrayToChange = arrayToCopy;

			Assert::AreEqual(arrayToCopy.getCapacity(), arrayToChange.getCapacity());
			Assert::IsTrue(areEqual(arrayToCopy, arrayToChange));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyArray)
		{
			Array arrayToCopy = createArrayFromRange(1, 5);
			Array arrayToChange = createArrayFromRange(10, 15);

			arrayToChange = arrayToCopy;

			Assert::AreEqual(arrayToCopy.getCapacity(), arrayToChange.getCapacity());
			Assert::IsTrue(areEqual(arrayToCopy, arrayToChange));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyArray)
		{
			Array arrayToMove;
			Array arrayToMoveInto;

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMoveInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyArray)
		{
			Array arrayToMove;
			Array arrayToMoveInto = createArrayFromRange(1, 5);

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMoveInto), L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyArray)
		{
			Array arrayToMove = createArrayFromRange(1, 5);
			std::size_t movedArrayCapacity = arrayToMove.getCapacity();
			Array arrayToMoveInto;

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(movedArrayCapacity, arrayToMoveInto.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arrayToMoveInto, 1, 5));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyArray)
		{
			Array arrayToMove = createArrayFromRange(10, 15);
			Array arrayToMoveInto = createArrayFromRange(1, 5);
			std::size_t movedArrayCapacity = arrayToMove.getCapacity();

			arrayToMoveInto = std::move(arrayToMove);

			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove), L"The moved-from array did not become empty!");
			Assert::AreEqual(movedArrayCapacity, arrayToMoveInto.getCapacity());
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

		TEST_METHOD(testAddIncrementsSize)
		{
			Array arr;

			for (unsigned i = 1; i <= 5; ++i)
			{
				arr.add(i);
				Assert::AreEqual(i, arr.getSize());
			}
		}

		TEST_METHOD(testAddingAnRvalueMovesTheObject)
		{
			const char word[] = "test";
			std::string s = word;
			DynamicArray<std::string> arr;

			arr.add(std::move(s));

			Assert::IsTrue(stringIsMovedFrom(s));
			Assert::IsTrue(arr[0] == word);
		}

		TEST_METHOD(testRemoveAtShiftsLeftTheElementsAfterRemovedItem)
		{
			Array arr = createArrayFromRange(0, 5);

			arr.removeAt(0);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 5));
		}

		TEST_METHOD(testRemoveAtUpdatesSize)
		{
			Array arr = createArrayFromRange(0, 6);

			for (int i = 6; i >= 0; i -= 2)
			{
				arr.removeAt(i);
			}

			Assert::AreEqual(3u, arr.getSize());
		}

		TEST_METHOD(testRemoveAtInvalidIndexThrowsException)
		{
			Array arr = createArrayFromRange(0, 5);

			try
			{
				arr.removeAt(6);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("Index out of range!", e.what()));
			}
		}

		TEST_METHOD(testRemoveLast)
		{
			Array arr = createArrayFromRange(0, 5);

			arr.removeLast();

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 0, 4));
		}

		TEST_METHOD(testRemoveLastFromEmptyArrayThrowsException)
		{
			Array emptyArray;

			try
			{
				emptyArray.removeLast();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("There is no last element!", e.what()));
			}
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

			arr.addAt(arr.getSize(), 16);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 11, 16));
		}

		TEST_METHOD(testAddAtIndexGreaterThanSizeThrowsException)
		{
			Array arr;
			unsigned number = 10;

			try
			{
				arr.addAt(1, number);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("Index out of range!", e.what()));
			}
		}

		TEST_METHOD(testAddAtIncrementsSize)
		{
			Array arr = createArrayFromRange(0, 4);

			for (int i = 1; i <= 3; ++i)
			{
				arr.addAt(5, 100);
			}

			Assert::AreEqual(8u, arr.getSize());
		}

		TEST_METHOD(testAddAtWithAnRvalueMovesTheObject)
		{
			const char word[] = "test";
			std::string s = word;
			DynamicArray<std::string> arr;

			arr.addAt(0, std::move(s));

			Assert::IsTrue(stringIsMovedFrom(s));
			Assert::IsTrue(arr[0] == word);
		}

		TEST_METHOD(testReserveExtendsTheArrayIfPassedGreaterCapacity)
		{
			Array arr = createArrayFromRange(1, 10);
			std::size_t capacity = arr.getCapacity();

			arr.reserve(capacity + 1);

			Assert::AreEqual(capacity + 1, arr.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 10),
				L"Extending the array did not kepp its elements the same!");
		}

		TEST_METHOD(testReserveDoesNothingIfCapacityIsAvailable)
		{
			Array arr(100);

			arr.reserve(50);

			Assert::AreEqual(100u, arr.getCapacity());
		}

		TEST_METHOD(testEmpty)
		{
			Array arr = createArrayFromRange(1, 10);

			arr.empty();

			Assert::IsTrue(hasNullForCapacityAndSize(arr));
		}

		TEST_METHOD(testSubscriptOperator)
		{
			Array arr = createArrayFromRange(0, 4);

			for (unsigned i = 0; i <= 4; ++i)
			{
				Assert::AreEqual(i, arr[i]);
			}
		}

		TEST_METHOD(testSubscriptOperatorWithInvalidIndexThrowsException)
		{
			Array arr;

			try
			{
				arr[0];
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("Index out of range!", e.what()));
			}
		}
	};
}