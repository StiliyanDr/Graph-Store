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

		static const std::size_t RANGE_START = 1u;
		static const std::size_t RANGE_END = 5u;

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
				Assert::IsTrue(areEqual("Size must not exceed capacity!",
					                    e.what()));
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
			fillArrayWithNumbersFromTo(arrayToMove, RANGE_START, RANGE_END);
			auto capacity = arrayToMove.getCapacity();

			auto movedInto = Array(std::move(arrayToMove));
			
			Assert::IsTrue(hasNullForCapacityAndSize(arrayToMove),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, movedInto.getCapacity(),
				             L"The moved-into array should have the same capacity!");
			Assert::IsTrue(arrayConsistsOfNumbersInRange(movedInto,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testCopyConstructorFromEmptyArray)
		{
			auto emptyArray = Array{};

			auto copy = emptyArray;

			Assert::IsTrue(hasNullForCapacityAndSize(copy));
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyArray)
		{
			auto arrayToCopy =
				createArrayFromRange(RANGE_START, RANGE_END);

			auto copy = arrayToCopy;
			
			Assert::AreEqual(arrayToCopy.getCapacity(),
				             copy.getCapacity());
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
			auto lhs = createArrayFromRange(RANGE_START,
				                            RANGE_END);

			lhs = emptyArray;

			Assert::IsTrue(hasNullForCapacityAndSize(lhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyArray)
		{
			auto rhs = createArrayFromRange(RANGE_START,
				                            RANGE_END);
			auto lhs = Array{};

			lhs = rhs;

			Assert::AreEqual(rhs.getCapacity(), lhs.getCapacity());
			Assert::IsTrue(areEqual(rhs, lhs));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyArray)
		{
			auto rhs = createArrayFromRange(RANGE_START,
				                            RANGE_END);
			auto lhs = createArrayFromRange(RANGE_START + 1,
				                            RANGE_END + 1);
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
			auto lhs = createArrayFromRange(RANGE_START,
				                            RANGE_END);

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(lhs),
				           L"The moved-into array did not become empty!");
			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyArray)
		{
			auto rhs = createArrayFromRange(RANGE_START, RANGE_END);
			auto capacity = rhs.getCapacity();
			auto lhs = Array{};

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, lhs.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(lhs,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyArray)
		{
			auto rhs = createArrayFromRange(RANGE_START, RANGE_END);
			auto lhs = createArrayFromRange(RANGE_START + 1, RANGE_END + 1);
			auto capacity = rhs.getCapacity();

			lhs = std::move(rhs);

			Assert::IsTrue(hasNullForCapacityAndSize(rhs),
				           L"The moved-from array did not become empty!");
			Assert::AreEqual(capacity, lhs.getCapacity());
			Assert::IsTrue(arrayConsistsOfNumbersInRange(lhs,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testAddAppendsAnItemAtTheEnd)
		{
			auto arr = Array{};

			for (auto i = RANGE_START;
				 i <= RANGE_END;
				 ++i)
			{
				arr.add(i);
			}

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testAddIncrementsSize)
		{
			auto arr = Array{};

			for (auto i = 1u;
				 i <= 5u;
				 ++i)
			{
				arr.add(i);
				Assert::AreEqual(i, arr.getSize());
			}
		}

		TEST_METHOD(testAddingAnRvalueMovesTheObject)
		{
			const char word[] = "test";
			auto s = std::string{ word };
			auto arr = DynamicArray<std::string>{};

			arr.add(std::move(s));

			Assert::IsTrue(stringIsMovedFrom(s),
				           L"The rvalue object was not moved!");
			Assert::IsTrue(arr[0] == word);
		}

		TEST_METHOD(testRemoveAtShiftsLeftTheItemsAfterRemovedItem)
		{
			auto arr = createArrayFromRange(0, 5);

			arr.removeAt(0);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr, 1, 5));
		}

		TEST_METHOD(testRemoveAtDecrementsSize)
		{
			auto arr = createArrayFromRange(0, 6);

			arr.removeAt(0);

			Assert::AreEqual(6u, arr.getSize());
		}

		TEST_METHOD(testRemoveAtInvalidIndexThrowsException)
		{
			auto arr = Array{};

			try
			{
				arr.removeAt(0);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("Index out of range!", e.what()));
			}
		}

		TEST_METHOD(testRemoveLast)
		{
			auto arr = createArrayFromRange(RANGE_START,
				                            RANGE_END + 1);
			arr.removeLast();

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testRemoveLastFromEmptyArrayThrowsException)
		{
			auto emptyArray = Array{};

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

		TEST_METHOD(testAddAtIndexShiftsRightTheNextItems)
		{
			auto arr = createArrayFromRange(RANGE_START + 1,
				                            RANGE_END);

			arr.addAt(0, RANGE_START);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr,
				                                         RANGE_START,
				                                         RANGE_END));
		}

		TEST_METHOD(testAddAtIndexAfterLastElement)
		{
			auto arr = createArrayFromRange(RANGE_START,
				                            RANGE_END);

			arr.addAt(arr.getSize(), RANGE_END + 1);

			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr,
				                                         RANGE_START,
				                                         RANGE_END + 1));
		}

		TEST_METHOD(testAddAtIndexGreaterThanSizeThrowsException)
		{
			auto arr = Array{};

			try
			{
				arr.addAt(arr.getSize() + 1, 10);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("Index out of range!", e.what()));
			}
		}

		TEST_METHOD(testAddAtIncrementsSize)
		{
			auto arr = createArrayFromRange(1, 1);

			arr.addAt(0, 0);

			Assert::AreEqual(2u, arr.getSize());
		}

		TEST_METHOD(testAddAtWithAnRvalueMovesTheObject)
		{
			const char word[] = "test";
			auto s = std::string{ word };
			auto arr = DynamicArray<std::string>{};

			arr.addAt(0, std::move(s));

			Assert::IsTrue(stringIsMovedFrom(s),
				           L"The rvalue object was not moved!");
			Assert::IsTrue(arr[0] == word);
		}

		TEST_METHOD(testReserveExtendsTheArrayIfPassedGreaterCapacity)
		{
			auto arr = createArrayFromRange(RANGE_START,
				                            RANGE_END);
			auto desiredCapacity = arr.getCapacity() + 1;

			arr.reserve(desiredCapacity);

			Assert::IsTrue(arr.getCapacity() >= desiredCapacity);
			Assert::IsTrue(arrayConsistsOfNumbersInRange(arr,
				                                         RANGE_START,
				                                         RANGE_END),
				           L"Extending the array did not keep its elements the same!");
		}

		TEST_METHOD(testReserveDoesNothingIfCapacityIsAvailable)
		{
			auto capacity = 10u;
			auto arr = Array(capacity);

			arr.reserve(capacity / 2);

			Assert::AreEqual(capacity, arr.getCapacity());
		}

		TEST_METHOD(testEmpty)
		{
			auto arr = createArrayFromRange(RANGE_START,
				                            RANGE_END);
			arr.empty();

			Assert::IsTrue(hasNullForCapacityAndSize(arr));
		}

		TEST_METHOD(testSubscriptOperator)
		{
			auto arr = createArrayFromRange(0, 4);

			for (auto i = 0u; i <= 4u; ++i)
			{
				Assert::AreEqual(i, arr[i]);
			}
		}

		TEST_METHOD(testSubscriptOperatorWithInvalidIndexThrowsException)
		{
			auto arr = Array{};

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
