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
			auto arr = Array{};

			Assert::IsTrue(hasNullForCapacityAndSize(arr));
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorFromCapacity)
		{
			auto capacity = 10u;

			auto arr = Array(capacity);

			Assert::AreEqual(capacity, arr.getCapacity());
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(testCtorAllocatesCapacityAndGivesAccessToSizeItems)
		{
			auto capacity = 10u;
			auto size = 5u;

			auto arr = Array(capacity, size);

			Assert::AreEqual(size, arr.getSize());
			Assert::AreEqual(capacity, arr.getCapacity());
		}

		TEST_METHOD(testCtorThrowsExceptionWhenSizeExceedsCapacity)	
		{
			auto capacity = 5u;

			try
			{
				auto arr = Array(capacity, capacity + 1);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (std::invalid_argument& e)
			{
				Assert::IsTrue(areEqual("Size must not exceed capacity!", e.what()));
			}
		}

		TEST_METHOD(testMoveConstructorFromEmptyArray)
		{
			auto arrayToMove = Array{};

			auto movedInto = Array(std::move(arrayToMove));

			Assert::IsTrue(hasNullForCapacityAndSize(movedInto),
				           L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove),
				           L"The moved-from array did not become empty!");
		}
		
		TEST_METHOD(testMoveConstructorFromNonEmptyArray)
		{
			auto arrayToMove = Array{};
			fillArrayWithNumbersFromTo(arrayToMove, 1, 5);
			auto capacity = arrayToMove.getCapacity();

			auto movedInto = Array(std::move(arrayToMove));
			
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, movedInto.getCapacity(),
				             L"The moved-into array should have the same capacity!");
			Assert::IsTrue(arrayConsistsOfNumbersInRange(movedInto, 1, 5));
		}

		TEST_METHOD(testCopyConstructorFromEmptyArray)
		{
			auto emptyArray = Array{};

			auto copy = emptyArray;

			Assert::IsTrue(hasNullForCapacityAndSize(copy));
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyArray)
		{
			auto arrayToCopy = createArrayFromRange(1, 5);

			auto copy = arrayToCopy;
			
			Assert::AreEqual(arrayToCopy.getCapacity(), copy.getCapacity());
			Assert::IsTrue(areEqual(arrayToCopy, copy));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyArray)
		{
			auto rhs = Array{};
			auto lhs = Array{};

			lhs = rhs;

			Assert::IsTrue(hasNullForCapacityAndSize(lhs));
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyArray)
		{	
			auto emptyArray = Array{};
			auto lhs = createArrayFromRange(1, 5);

			lhs = emptyArray;

			Assert::IsTrue(hasNullForCapacityAndSize(lhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyArray)
		{
			auto rhs = createArrayFromRange(1, 5);
			auto lhs = Array{};

			lhs = rhs;

			Assert::AreEqual(rhs.getCapacity(), lhs.getCapacity());
			Assert::IsTrue(areEqual(rhs, lhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyArray)
		{
			auto rhs = createArrayFromRange(1, 5);
			auto lhs = createArrayFromRange(10, 15);

			lhs = rhs;

			Assert::AreEqual(rhs.getCapacity(), lhs.getCapacity());
			Assert::IsTrue(areEqual(rhs, lhs));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyArray)
		{
			auto rhs = Array{};
			auto lhs = Array{};

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(lhs),
				           L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyArray)
		{
			auto rhs = Array{};
			auto lhs = createArrayFromRange(1, 5);

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(lhs),
				           L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyArray)
		{
			auto rhs = createArrayFromRange(1, 5);
			auto capacity = rhs.getCapacity();
			auto lhs = Array{};

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, lhs.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(lhs, 1, 5));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyArray)
		{
			auto rhs = createArrayFromRange(10, 15);
			auto lhs = createArrayFromRange(1, 5);
			auto capacity = rhs.getCapacity();

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, lhs.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(lhs, 10, 15));
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
