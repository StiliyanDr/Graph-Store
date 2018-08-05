#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Linked List/LinkedList.h"
#include <utility>
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LinkedListUnitTest
{		
	TEST_CLASS(LinkedListTest)
	{
		typedef LinkedList<unsigned> List;
		typedef LinkedListIterator<unsigned> Iterator;

	public:

		void fillListWithNumbersFromTo(List& list, unsigned from, unsigned to)
		{
			for (unsigned number = from; number <= to; ++number)
			{
				list.addBack(number);
			}
		}

		List createListFromRange(unsigned start, unsigned end)
		{
			List list;
			fillListWithNumbersFromTo(list, start, end);

			return list;
		}

		bool areEqual(List& expected, List& actual)
		{
			return listsHaveSameSize(expected, actual) && listsHaveSameElements(expected, actual);
		}

		bool listsHaveSameElements(List& lhs, List& rhs)
		{
			assert(listsHaveSameSize(lhs, rhs));

			Iterator lhsIterator = lhs.getIteratorToFirst();
			Iterator rhsIterator = rhs.getIteratorToFirst();

			while (lhsIterator)
			{
				if (*lhsIterator != *rhsIterator)
				{
					return false;
				}

				++lhsIterator;
				++rhsIterator;
			}

			return true;
		}

		bool listsHaveSameSize(const List& lhs, const List& rhs)
		{
			return lhs.getSize() == rhs.getSize();
		}

		bool listConsistsOfNumbersInRange(List& list, unsigned start, unsigned end)
		{
			if (end - start + 1 != list.getSize())
			{
				return false;
			}

			Iterator iterator = list.getIteratorToFirst();

			for (unsigned number = start; number <= end; ++number)
			{
				if (number != *iterator)
				{
					return false;
				}

				++iterator;
			}

			return true;
		}

		TEST_METHOD(testDefaultConstructorCreatesAnEmptyList)
		{
			List list;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testMoveConstructorFromEmptyList)
		{
			List emptyList;

			List list(std::move(emptyList));

			Assert::IsTrue(list.isEmpty());
			Assert::IsTrue(emptyList.isEmpty());
		}

		TEST_METHOD(testMoveConstructorFromNonEmptyList)
		{
			List listToMove;
			fillListWithNumbersFromTo(listToMove, 1, 10);
	
			List list(std::move(listToMove));

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testCopyConstructorFromEmptyList)
		{
			List emptyList;
			List copy(emptyList);

			Assert::IsTrue(copy.isEmpty());
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyList)
		{
			List list = createListFromRange(1, 10);
			
			List copy(list);

			Assert::IsTrue(areEqual(list, copy));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyList)
		{
			List listToMove;
			List list;

			list = std::move(listToMove);

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyList)
		{
			List listToMove;
			List list = createListFromRange(1, 10);
			
			list = std::move(listToMove);
			
			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyList)
		{
			List listToMove = createListFromRange(1, 10);
			List list;

			list = std::move(listToMove);

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyList)
		{
			List listToMove = createListFromRange(1, 10);
			List list = createListFromRange(11, 20);

			list = std::move(listToMove);

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyList)
		{
			const List emptyList;
			List list;

			list = emptyList;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyList)
		{
			const List emptyList;
			List list = createListFromRange(1, 10);

			list = emptyList;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyList)
		{
			List listToCopy = createListFromRange(1, 10);
			List list;

			list = listToCopy;

			Assert::IsTrue(areEqual(listToCopy, list));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyList)
		{
			List listToCopy = createListFromRange(1, 10);
			List list = createListFromRange(20, 30);

			list = listToCopy;

			Assert::IsTrue(areEqual(listToCopy, list));
		}

		TEST_METHOD(testCopyAppendEmptyToEmptyList)
		{
			List listToAppend;
			List list;

			list.appendList(listToAppend);

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testCopyAppendEmptyToNonEmptyList)
		{
			List listToAppend;
			List list = createListFromRange(1, 10);

			list.appendList(listToAppend);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testCopyAppendNonEmptyToEmptyList)
		{
			List listToAppend = createListFromRange(1, 10);
			List list;

			list.appendList(listToAppend);

			Assert::IsTrue(areEqual(listToAppend, list));
		}

		TEST_METHOD(testCopyAppendNonEmptyToNonEmptyList)
		{
			List listToAppend = createListFromRange(11, 20);
			List list = createListFromRange(1, 10);

			list.appendList(listToAppend);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 20));
		}

		TEST_METHOD(testMoveAppendEmptyToEmptyList)
		{
			List listToAppend;
			List list;

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testMoveAppendEmptyToNonEmptyList)
		{
			List listToAppend;
			List list = createListFromRange(1, 10);

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testMoveAppendNonEmptyToEmptyList)
		{
			List listToAppend = createListFromRange(1, 10);
			List list;

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testMoveAppendNonEmptyToNonEmptyList)
		{
			List listToAppend = createListFromRange(11, 20);
			List list = createListFromRange(1, 10);

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 20));
		}
		
		TEST_METHOD(testInsertAfterInvalidIteratorInsertsBack)
		{
			List list;
			Iterator iterator = list.getIteratorToFirst();
			list = createListFromRange(1, 4);
			
			list.insertAfter(iterator, 5);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testInsertAfterLastItem)
		{
			List list = createListFromRange(1, 4);
			Iterator iteratorToLast = list.getIteratorToLast();

			list.insertAfter(iteratorToLast, 5);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testInsertAfterMiddleItem)
		{
			List list = createListFromRange(1, 1);
			list.addBack(3);
			Iterator iterator = list.getIteratorToFirst();

			list.insertAfter(iterator, 2);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 3));
		}

		TEST_METHOD(testInsertBeforeInvalidIteratorInsertsAtFront)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(2, 5);

			list.insertBefore(invalidIterator, 1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testInsertBeforeFirstInList)
		{
			List list = createListFromRange(2, 5);
			Iterator iteratorToFirst = list.getIteratorToFirst();

			list.insertBefore(iteratorToFirst, 1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testInsertBeforeMiddleItem)
		{
			List list = createListFromRange(1, 1);
			list.addBack(3);
			Iterator iterator = list.getIteratorToLast();

			list.insertBefore(iterator, 2);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 3));
		}

		TEST_METHOD(testAddFrontInEmptyList)
		{
			List list;

			list.addFront(1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 1));
		}

		TEST_METHOD(testAddFrontInNonEmptyList)
		{
			List list = createListFromRange(2, 5);

			list.addFront(1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testAddBack)
		{
			List list;

			for (unsigned number = 1; number <= 5; ++number)
			{
				list.addBack(number);
			}

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testRemoveAtInvalidIteratorDoesNothing)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(1, 10);

			list.removeAt(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 10));
		}

		TEST_METHOD(testRemoveAtFrontOfList)
		{
			List list = createListFromRange(1, 5);

			list.removeAt(list.getIteratorToFirst());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 2, 5));
		}

		TEST_METHOD(testRemoveAtBackOfList)
		{
			List list = createListFromRange(1, 5);

			list.removeAt(list.getIteratorToLast());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 4));
		}

		TEST_METHOD(testRemoveAtMiddleOfList)
		{
			List list = createListFromRange(1, 2);
			list.insertAfter(list.getIteratorToFirst(), 100);
			Iterator iterator = list.getIteratorToFirst();
			++iterator;

			list.removeAt(iterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 2));
		}

		TEST_METHOD(testRemoveAfterInvalidIteratorDoesNothing)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(1, 5);

			list.removeAfter(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testRemoveAfterLastDoesNothing)
		{
			List list = createListFromRange(1, 5);

			list.removeAfter(list.getIteratorToLast());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testRemoveAfterMiddleItem)
		{
			List list = createListFromRange(1, 2);
			list.insertAfter(list.getIteratorToFirst(), 100);

			list.removeAfter(list.getIteratorToFirst());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 2));
		}

		TEST_METHOD(testRemoveBeforeInvalidIteratorDoesNothing)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(1, 5);

			list.removeBefore(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testRemoveBeforeFirstItemDoesNothing)
		{
			List list = createListFromRange(1, 5);

			list.removeBefore(list.getIteratorToFirst());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 5));
		}

		TEST_METHOD(testRemoveBeforeMiddleItem)
		{
			List list = createListFromRange(1, 2);
			list.insertAfter(list.getIteratorToFirst(), 100);

			list.removeBefore(list.getIteratorToLast());

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 2));
		}

		TEST_METHOD(testRemoveFirst)
		{
			List list = createListFromRange(1, 5);

			list.removeFirst();

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 2, 5));
		}

		TEST_METHOD(testRemoveFirstInOneElementList)
		{
			List list = createListFromRange(1, 1);

			list.removeFirst();

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testRemoveLast)
		{
			List list = createListFromRange(1, 5);

			list.removeLast();

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 4));
		}

		TEST_METHOD(testRemoveLastInOneElementList)
		{
			List list = createListFromRange(1, 1);

			list.removeLast();

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testEmptyANonEmptyList)
		{
			List list = createListFromRange(1, 5);

			list.empty();

			Assert::IsTrue(list.isEmpty());
		}
	};
}