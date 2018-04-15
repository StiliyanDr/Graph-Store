#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Graph Store/Graph Store/DynamicArray.h"

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

		void fillArrayFromTo(Array &arr, unsigned from, unsigned to)
		{
			for (unsigned i = from; i <= to; ++i)
			{
				arr.add(i);
			}
		}

		bool areEqual(const Array& lhs, const Array& rhs)
		{
			const size_t lhsCount = lhs.getCount();

			if (lhsCount != rhs.getCount())
				return false;

			for (size_t i = 0; i < lhsCount; ++i)
			{
				if (lhs[i] != rhs[i])
					return false;
			}

			return true;
		}

		bool arrayConsistsOfAllInRange(const Array& arr, unsigned start, unsigned end)
		{
			unsigned rangeSize = end - start + 1;

			if (rangeSize != arr.getCount())
				return false;

			for (unsigned i = 0; i < rangeSize; ++i)
			{
				if (arr[i] != start++)
					return false;
			}

			return true;
		}

		TEST_METHOD(defaultConstructor)
		{
			Array arr;

			Assert::AreEqual(0u, arr.getSize());
			Assert::AreEqual(0u, arr.getCount());
			Assert::IsTrue(arr.isEmpty());
		}

		TEST_METHOD(constructor)
		{
			for (size_t size = 0; size <= 10; ++size)
			{
				for (size_t count = 0; count <= size; ++count)
				{
					Array arr(size, count);

					Assert::AreEqual(size, arr.getSize());
					Assert::AreEqual(count, arr.getCount());
					Assert::IsTrue(!arr.isEmpty() || count == 0);
				}
			}
		}

		TEST_METHOD(invalidCtorArguments)
		{
			for (size_t size = 0; size <= 10; ++size)
			{
				try
				{
					Array arr(size, size + 1);

					Assert::Fail(L"Invalid array created!");
				}
				catch (...)
				{
				}
			}
		}

		TEST_METHOD(moveCtorFromEmpty)
		{
			Array emptyArray;
			Array movedInto(std::move(emptyArray));

			Assert::IsTrue(hasNullForCountAndSize(movedInto));
			Assert::IsTrue(hasNullForCountAndSize(emptyArray));
		}

		TEST_METHOD(moveCtorFromNonEmpty)
		{
			Array notEmpty(10);
			fillArrayFromTo(notEmpty, 1, 5);

			Array movedInto(std::move(notEmpty));
			Assert::IsTrue(hasNullForCountAndSize(notEmpty));

			Assert::AreEqual(5u, movedInto.getCount());
			Assert::AreEqual(10u, movedInto.getSize());

			Assert::IsTrue(arrayConsistsOfAllInRange(movedInto, 1, 5));
		}

		TEST_METHOD(copyConstructor)
		{
			const Array empty;

			Array emptyCopy(empty);
			Assert::IsTrue(hasNullForCountAndSize(emptyCopy));

			Array notEmpty(10);
			fillArrayFromTo(notEmpty, 1, 5);

			Array notEmptyCopy(notEmpty);
			
			Assert::AreEqual(notEmpty.getCount(), notEmptyCopy.getCount());
			Assert::AreEqual(notEmpty.getSize(), notEmptyCopy.getSize());
			Assert::IsTrue(areEqual(notEmpty, notEmptyCopy));
		}

		TEST_METHOD(copyAssignmentFromEmpty)
		{
			const Array empty;

			Array emptyLhs; 
			emptyLhs = empty;
			Assert::IsTrue(hasNullForCountAndSize(emptyLhs));

			Array notEmpty(10, 10);
			notEmpty = empty;
			Assert::IsTrue(hasNullForCountAndSize(notEmpty));
		}

		TEST_METHOD(copyAssignmentFromNonEmpty)
		{
			const Array rhs(10, 10);

			Array emptyLhs;
			emptyLhs = rhs;
			Assert::AreEqual(10u, emptyLhs.getCount());
			Assert::AreEqual(10u, emptyLhs.getSize());
			Assert::IsTrue(areEqual(rhs, emptyLhs));

			Array notEmptyLhs(4, 4);
			notEmptyLhs = rhs;
			Assert::AreEqual(10u, notEmptyLhs.getCount());
			Assert::AreEqual(10u, notEmptyLhs.getSize());
			Assert::IsTrue(areEqual(rhs, notEmptyLhs));
		}

		TEST_METHOD(moveAssignmentEmpty)
		{
			Array empty;

			Array emptyLhs;
			emptyLhs = std::move(empty);
			Assert::IsTrue(hasNullForCountAndSize(emptyLhs));
			Assert::IsTrue(hasNullForCountAndSize(empty));
			
			Array notEmptyLhs(10, 10);
			notEmptyLhs = std::move(empty);
			Assert::IsTrue(hasNullForCountAndSize(notEmptyLhs));
			Assert::IsTrue(hasNullForCountAndSize(empty));
		}

		TEST_METHOD(moveAssignmentNonEmpty)
		{
			Array rhs(10);
			fillArrayFromTo(rhs, 1, 5);
			Array copyOfRhs(rhs);

			Array emptyLhs;
			emptyLhs = std::move(rhs);
			Assert::AreEqual(10u, emptyLhs.getSize());
			Assert::IsTrue(areEqual(copyOfRhs, emptyLhs));
			Assert::IsTrue(hasNullForCountAndSize(rhs));

			Array notEmptyLhs(4, 4);
			notEmptyLhs = std::move(copyOfRhs);
			Assert::IsTrue(hasNullForCountAndSize(copyOfRhs));
			Assert::AreEqual(10u, notEmptyLhs.getSize());
			Assert::IsTrue(areEqual(emptyLhs, notEmptyLhs));
		}

		TEST_METHOD(add)
		{
			Array arr;

			for (unsigned i = 1; i <= 30; ++i)
			{
				arr.add(i);
				Assert::AreEqual(i, arr.getCount());
			}

			for (unsigned i = 0; i < 30; ++i)
				Assert::AreEqual(i + 1, arr[i]);
		}

		TEST_METHOD(removeAtInvalidIndex)
		{
			Array arr;

			try
			{
				arr.remove(arr.getCount());

				Assert::Fail(L"Removed an element at invalid index!");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(remove)
		{
			Array arr;
			fillArrayFromTo(arr, 0, 6);

			for (int i = 5; i >= 1; i -= 2)
				arr.remove(i);

			Assert::AreEqual(4u, arr.getCount());

			for (unsigned i = 0; i <= 3; ++i)
				Assert::AreEqual(2 * i, arr[i]);
		}

		TEST_METHOD(removeLastElement)
		{
			Array arr(10);
			fillArrayFromTo(arr, 1, 10);

			for (unsigned i = arr.getCount() - 1; i > 0; --i)
			{
				arr.remove(i);
				Assert::AreEqual(i, arr.getCount());
				Assert::AreEqual(i, arr[arr.getCount() - 1]);
			}
		}

		TEST_METHOD(search)
		{
			Array arr;
			fillArrayFromTo(arr, 0, 20);

			for (long i = 0; i <= 20; ++i)
				Assert::AreEqual(i, arr.search(i));

			for (unsigned i = 21; i <= 30; ++i)
				Assert::AreEqual(-1L, arr.search(i));
		}

		TEST_METHOD(addAt)
		{
			Array arr(20);

			for (unsigned i = 0; i <= 6; i += 2)
				arr.add(i);

			for (unsigned i = 1; i <= 5; i += 2)
				arr.addAt(i, i);

			Assert::IsTrue(arrayConsistsOfAllInRange(arr, 0, 6));
		}

		TEST_METHOD(addAtEnd)
		{
			Array arr(5);

			for (size_t i = 1; i <= 10; ++i)
			{
				arr.addAt(arr.getCount(), i);
				Assert::AreEqual(i, arr.getCount());
				Assert::AreEqual(i, arr[arr.getCount() - 1]);
			}
		}

		TEST_METHOD(ensureSize)
		{
			Array arr(10, 5);

			for (size_t i = 0; i <= 10; ++i)
			{
				arr.ensureSize(i);
				Assert::AreEqual(10u, arr.getSize());
			}

			for (size_t i = 11; i <= 20; ++i)
			{
				arr.ensureSize(i);
				Assert::AreEqual(i, arr.getSize());
				Assert::AreEqual(5u, arr.getCount());
			}
		}

		TEST_METHOD(empty)
		{
			for (size_t i = 0; i <= 10; ++i)
			{
				Array arr(i, i);
				arr.empty();

				Assert::AreEqual(0u, arr.getSize());
				Assert::AreEqual(0u, arr.getCount());
				Assert::IsTrue(arr.isEmpty());
			}
		}

		TEST_METHOD(isEmpty)
		{
			Array arr;
			Assert::IsTrue(arr.isEmpty());

			for (unsigned i = 1; i <= 5; ++i)
			{
				arr.add(i);
				Assert::IsFalse(arr.isEmpty());
			}
		}

		TEST_METHOD(subscriptOperator)
		{
			Array arr;
			fillArrayFromTo(arr, 0, 10);

			for (unsigned i = 0; i <= 10; ++i)
				Assert::AreEqual(i, arr[i]);

			try
			{
				arr[arr.getCount()];

				Assert::Fail(L"Accessed an element at invalid index!");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(elementAddOperator)
		{
			Array arr(10);

			for (unsigned i = 1; i <= 10; ++i)
			{
				arr += i;
				Assert::AreEqual(i, arr.getCount());
			}

			Assert::IsTrue(arrayConsistsOfAllInRange(arr, 1, 10));
		}

		TEST_METHOD(addWholeArrayOperator)
		{
			Array lhs, rhs;

			fillArrayFromTo(lhs, 1, 10);
			fillArrayFromTo(rhs, 11, 20);

			lhs += rhs;

			Assert::IsTrue(arrayConsistsOfAllInRange(lhs, 1, 20));
		}
	};
}