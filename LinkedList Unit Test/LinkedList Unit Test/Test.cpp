#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Graph Store/Graph Store/LinkedList.h"
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LinkedListUnitTest
{		
	TEST_CLASS(LinkedListTest)
	{
		typedef LinkedList<unsigned> List;
		typedef LinkedListIterator<unsigned> Iterator;

	public:

		void fillListFromTo(List& list, unsigned from, unsigned to) const
		{
			while(from <= to)
				list.addBack(from++);
		}

		bool areEqual(List& lhs, List& rhs) const
		{
			if (lhs.getSize() != rhs.getSize())
				return false;

			Iterator lhsIterator = lhs.getIteratorToFirst();
			Iterator rhsIterator = rhs.getIteratorToFirst();

			while (lhsIterator)
			{
				if (*lhsIterator != *rhsIterator)
					return false;

				++lhsIterator;
				++rhsIterator;
			}

			return true;
		}

		bool listConsistsOfAllInRange(List& list, unsigned start, unsigned end) const
		{
			unsigned rangeSize = end - start + 1;

			if (rangeSize != list.getSize())
				return false;

			Iterator iterator = list.getIteratorToFirst();

			for (unsigned i = start; i <= end; ++i)
			{
				if (i != *iterator)
					return false;

				++iterator;
			}

			return true;
		}

		List createListFromRange(unsigned start, unsigned end) const
		{
			List list;
			fillListFromTo(list, start, end);

			return list;
		}

		TEST_METHOD(defaultConstructor)
		{
			List list;

			Assert::IsTrue(list.isEmpty());
			Assert::AreEqual(0u, list.getSize());
		}

		TEST_METHOD(moveCtorFromEmpty)
		{
			List emptyList;

			List list(std::move(emptyList));

			Assert::IsTrue(list.isEmpty());
			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(moveCtorFromNonEmpty)
		{
			List movedFrom;
			fillListFromTo(movedFrom, 1, 10);

			List copy(movedFrom);
			List movedInto(std::move(movedFrom));

			Assert::IsTrue(movedFrom.isEmpty());
			Assert::IsTrue(areEqual(copy, movedInto));
		}

		TEST_METHOD(copyCtorFromEmpty)
		{
			const List emptyList;
			List copy(emptyList);

			Assert::IsTrue(copy.isEmpty());
			Assert::AreEqual(0u, copy.getSize());
		}

		TEST_METHOD(copyCtorFromNonEmpty)
		{
			List list = createListFromRange(1, 10);
			List copy(list);

			Assert::IsTrue(areEqual(list, copy));
		}

		TEST_METHOD(moveAssignmentEmptyList)
		{
			List emptyList;

			List emptyLhs;
			emptyLhs = std::move(emptyList);
			Assert::IsTrue(emptyLhs.isEmpty() && emptyList.isEmpty());

			List nonEmptyLhs = createListFromRange(1, 10);
			nonEmptyLhs = std::move(emptyList);
			Assert::IsTrue(emptyList.isEmpty() && nonEmptyLhs.isEmpty());
		}

		TEST_METHOD(moveAssignmentNonEmptyList)
		{
			List rhs = createListFromRange(1, 10);
			List copyOfRhs(rhs);

			List emptyLhs;
			emptyLhs = std::move(rhs);
			Assert::IsTrue(rhs.isEmpty());
			Assert::IsTrue(areEqual(copyOfRhs, emptyLhs));

			List nonEmptyLhs = createListFromRange(0, 5);
			nonEmptyLhs = std::move(copyOfRhs);
			Assert::IsTrue(copyOfRhs.isEmpty());
			Assert::IsTrue(areEqual(emptyLhs, nonEmptyLhs));
		}

		TEST_METHOD(copyAssignmentEmptyList)
		{
			const List emptyList;

			List emptyLhs;
			emptyLhs = emptyList;
			Assert::IsTrue(emptyLhs.isEmpty());

			List nonEmptyLhs = createListFromRange(0, 10);
			nonEmptyLhs = emptyList;
			Assert::IsTrue(nonEmptyLhs.isEmpty());
		}

		TEST_METHOD(copyAssignmentNonEmptyList)
		{
			List list = createListFromRange(0, 10);

			List emptyLhs;
			emptyLhs = list;
			Assert::IsTrue(areEqual(list, emptyLhs));

			List nonEmptyLhs = createListFromRange(10, 20);
			nonEmptyLhs = list;
			Assert::IsTrue(areEqual(list, nonEmptyLhs));
		}

		TEST_METHOD(copyAppendEmptyList)
		{
			const List emptyList;

			List emptyLhs;
			emptyLhs.appendList(emptyList);
			Assert::IsTrue(emptyLhs.isEmpty());

			List nonEmptyLhs = createListFromRange(1, 10);
			List copyOfNonEmptyLhs(nonEmptyLhs);

			nonEmptyLhs.appendList(emptyList);
			Assert::IsTrue(areEqual(copyOfNonEmptyLhs, nonEmptyLhs));
		}

		TEST_METHOD(copyAppendNonEmptyList)
		{
			List list = createListFromRange(11, 20);

			List emptyLhs;
			emptyLhs.appendList(list);
			Assert::IsTrue(areEqual(list, emptyLhs));

			List nonEmptyLhs = createListFromRange(1, 10);
			nonEmptyLhs.appendList(list);

			Assert::IsTrue(listConsistsOfAllInRange(nonEmptyLhs, 1, 20));
		}

		TEST_METHOD(moveAppendEmptyList)
		{
			List emptyList;

			List emptyLhs;
			emptyLhs.appendList(std::move(emptyList));
			Assert::IsTrue(emptyLhs.isEmpty() && emptyList.isEmpty());

			List nonEmptyLhs = createListFromRange(1, 10);

			nonEmptyLhs.appendList(std::move(emptyList));
			Assert::IsTrue(emptyList.isEmpty());
			Assert::IsTrue(listConsistsOfAllInRange(nonEmptyLhs, 1, 10));
		}

		TEST_METHOD(moveAppendNonEmptyToEmptyList)
		{
			List rhs = createListFromRange(1, 10);

			List lhs;
			lhs.appendList(std::move(rhs));

			Assert::IsTrue(rhs.isEmpty());
			Assert::IsTrue(listConsistsOfAllInRange(lhs, 1, 10));
		}

		TEST_METHOD(moveAppendNonEmptyToNonEmptyList)
		{
			List rhs = createListFromRange(11, 20);

			List lhs = createListFromRange(1, 10);
			lhs.appendList(std::move(rhs));

			Assert::IsTrue(rhs.isEmpty());
			Assert::IsTrue(listConsistsOfAllInRange(lhs, 1, 20));
		}
		
		TEST_METHOD(insertAfterInvalidIterator)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();

			fillListFromTo(list, 1, 5);

			size_t size = list.getSize();

			for (unsigned i = 6; i <= 10; ++i)
			{
				list.insertAfter(invalidIterator, i);
				Assert::AreEqual(++size, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 10));
		}

		TEST_METHOD(insertAfter)
		{
			List list;

			for (unsigned i = 0; i <= 6; i += 2)
				list.addBack(i);

			size_t size = list.getSize();

			Iterator iterator = list.getIteratorToFirst();

			for (unsigned i = 1; i <= 5; i += 2)
			{
				list.insertAfter(iterator, i);
				Assert::AreEqual(++size, list.getSize());

				for (int i = 1; i <= 2; ++i)
					++iterator;
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 0, 6));
		}

		TEST_METHOD(insertBeforeInvalidIterator)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();

			fillListFromTo(list, 3, 5);

			size_t size = list.getSize();
			
			for (unsigned i = 2; i >= 1; --i)
			{
				list.insertBefore(invalidIterator, i);
				Assert::AreEqual(++size, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 5));
		}

		TEST_METHOD(insertBefore)
		{
			List list;

			for (unsigned i = 1; i <= 5; i += 2)
				list.addBack(i);

			size_t size = list.getSize();
			Iterator iterator = list.getIteratorToFirst();

			for (unsigned i = 0; i <= 4; i += 2)
			{
				list.insertBefore(iterator, i);
				Assert::AreEqual(++size, list.getSize());
				++iterator;
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 0, 5));
		}

		TEST_METHOD(addFront)
		{
			List list;

			size_t size = 0;

			for (unsigned i = 10; i >= 1; --i)
			{
				list.addFront(i);
				Assert::AreEqual(++size, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 10));
		}

		TEST_METHOD(addBack)
		{
			List list;

			for (unsigned i = 1; i <= 10; ++i)
			{
				list.addBack(i);
				Assert::AreEqual(i, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 10));
		}

		TEST_METHOD(removeAtInvalidIterator)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();

			fillListFromTo(list, 1, 10);

			list.removeAt(invalidIterator);

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 10));
		}

		TEST_METHOD(removeAt)
		{
			List list = createListFromRange(1, 5);

			list.removeAt(list.getIteratorToFirst());
			list.removeAt(list.getIteratorToLast());

			Iterator iterator = list.getIteratorToFirst();
			list.removeAt(++iterator);

			Assert::AreEqual(2u, list.getSize());
			Assert::AreEqual(2u, *(list.getIteratorToFirst()));
			Assert::AreEqual(4u, *(list.getIteratorToLast()));
		}

		TEST_METHOD(removeAfterInvalidIterator)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();

			fillListFromTo(list, 1, 10);
			list.removeAfter(invalidIterator);

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 10));
		}

		TEST_METHOD(removeAfter)
		{
			List list = createListFromRange(1, 10);

			size_t size = list.getSize();

			Iterator iterator = list.getIteratorToFirst();

			for (int i = 1; i <= 9; ++i)
			{
				list.removeAfter(iterator);
				Assert::AreEqual(--size, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 1));
		}

		TEST_METHOD(removeBeforeInvalidIterator)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToLast();
			
			fillListFromTo(list, 1, 5);
			list.removeBefore(invalidIterator);

			Assert::IsTrue(listConsistsOfAllInRange(list, 1, 5));
		}

		TEST_METHOD(removeBefore)
		{
			List list = createListFromRange(1, 10);
			
			size_t size = list.getSize();

			Iterator last = list.getIteratorToLast();

			for (int i = 1; i <= 9; ++i)
			{
				list.removeBefore(last);
				Assert::AreEqual(--size, list.getSize());
			}

			Assert::IsTrue(listConsistsOfAllInRange(list, 10, 10));
		}

		TEST_METHOD(removeFirst)
		{
			List list = createListFromRange(1, 5);

			size_t size = list.getSize();

			for (unsigned i = 1; i <= 5; ++i)
			{
				Assert::AreEqual(size--, list.getSize());
				Assert::AreEqual(i, *(list.getIteratorToFirst()));
				list.removeFirst();
			}

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(removeFirstFromEmptyList)
		{
			List list;
			
			try
			{
				list.removeFirst();

				Assert::Fail(L"Removed the first element from an empty list!");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(removeLast)
		{
			List list = createListFromRange(1, 5);

			size_t size = list.getSize();

			for (unsigned i = 5; i >= 1; --i)
			{
				Assert::AreEqual(size--, list.getSize());
				Assert::AreEqual(i, (*list.getIteratorToLast()));
				list.removeLast();
			}

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(removeLastFromEmptyList) 
		{
			List list;

			try
			{
				list.removeLast();

				Assert::Fail(L"Removed the last element from an empty list!");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(search)
		{
			List list = createListFromRange(11, 20);
			
			for (unsigned i = 1; i <= 10; ++i)
			{	
				Iterator result = list.search(i);
				Assert::IsFalse(result);
			}

			for (unsigned i = 11; i <= 20; ++i)
			{
				Iterator result = list.search(i);
				Assert::AreEqual(i, *result);
			}
		}

		TEST_METHOD(getIteratorToFirst)
		{
			List list;

			Iterator invalidIterator = list.getIteratorToFirst();
			Assert::IsFalse(invalidIterator);

			fillListFromTo(list, 1, 5);

			Iterator first = list.getIteratorToFirst();
			Assert::AreEqual(1u, *first);
		}

		TEST_METHOD(getIteratorToLast)
		{
			List list;

			Iterator invalidIterator = list.getIteratorToLast();
			Assert::IsFalse(invalidIterator);

			fillListFromTo(list, 1, 5);

			Iterator last = list.getIteratorToLast();
			Assert::AreEqual(5u, *last);
		}

		TEST_METHOD(emptyAList)
		{
			List emptyList;
			emptyList.empty();
			Assert::IsTrue(emptyList.isEmpty());

			List nonEmptyList = createListFromRange(1, 5);
			nonEmptyList.empty();
			Assert::IsTrue(nonEmptyList.isEmpty());
		}

		TEST_METHOD(isEmpty)
		{
			List list;
			Assert::IsTrue(list.isEmpty());

			for (unsigned i = 1; i <= 10; ++i)
			{
				list.addBack(i);
				Assert::IsFalse(list.isEmpty());
			}
		}

		TEST_METHOD(getSize)
		{
			List list;
			Assert::AreEqual(0u, list.getSize());

			for (size_t i = 1; i <= 50; ++i)
			{
				list.addFront(i);
				Assert::AreEqual(i, list.getSize());
			}
		}
	};
}