#include "stdafx.h"
#include "CppUnitTest.h"
#include "Linked List/LinkedList.h"
#include "Iterator/Iterator.h"
#include <utility>
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LinkedListUnitTest
{		
	TEST_CLASS(LinkedListTest)
	{
		using List = LinkedList<unsigned>;
		using Iterator = LinkedList<unsigned>::Iterator;
		using ConstIterator = LinkedList<unsigned>::ConstIterator;

		static const unsigned RANGE_START = 1;
		static const unsigned RANGE_END = 5;

		static List createListFromRange(unsigned start,
			                            unsigned end)
		{
			List list;
			fillListWithNumbersFromTo(list, start, end);

			return list;
		}

		static void fillListWithNumbersFromTo(List& list,
			                                  unsigned from,
			                                  unsigned to)
		{
			for (unsigned i = from; i <= to; ++i)
			{
				list.addBack(i);
			}
		}

		static bool listConsistsOfNumbersInRange(const List& list,
			                                     unsigned start,
			                                     unsigned end)
		{
			if (end - start + 1 != list.getSize())
			{
				return false;
			}

			List range = createListFromRange(start, end);

			return areEqual(range, list);
		}

		static bool areEqual(const List& expected, const List& actual)
		{
			return listsHaveSameSize(expected, actual)
				   && listsHaveSameElements(expected, actual);
		}

		static bool listsHaveSameSize(const List& lhs, const List& rhs)
		{
			return lhs.getSize() == rhs.getSize();
		}

		static bool listsHaveSameElements(const List& lhs,
			                              const List& rhs)
		{
			assert(listsHaveSameSize(lhs, rhs));

			ConstIterator lhsIterator = lhs.getConstIterator();
			ConstIterator rhsIterator = rhs.getConstIterator();

			auto matchesCorrespondingElement =
				[&rhsIterator](auto lhsElement)
			{
				return lhsElement == *(rhsIterator++);
			};

			return allOf(lhsIterator, matchesCorrespondingElement);
		}

	public:
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
			fillListWithNumbersFromTo(listToMove,
				                      RANGE_START,
				                      RANGE_END);

			List list(std::move(listToMove));

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testCopyConstructorFromEmptyList)
		{
			List emptyList;

			List copy(emptyList);

			Assert::IsTrue(copy.isEmpty());
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);
			
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
			List list =
				createListFromRange(RANGE_START, RANGE_END);
			
			list = std::move(listToMove);
			
			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyList)
		{
			List listToMove =
				createListFromRange(RANGE_START, RANGE_END);
			List list;

			list = std::move(listToMove);

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyList)
		{
			List listToMove =
				createListFromRange(RANGE_START, RANGE_END);
			List list =
				createListFromRange(RANGE_START + 1,
				                    RANGE_END + 1);

			list = std::move(listToMove);

			Assert::IsTrue(listToMove.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
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
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list = emptyList;

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyList)
		{
			List listToCopy =
				createListFromRange(RANGE_START, RANGE_END);
			List list;

			list = listToCopy;

			Assert::IsTrue(areEqual(listToCopy, list));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyList)
		{
			List listToCopy =
				createListFromRange(RANGE_START, RANGE_END);
			List list =
				createListFromRange(RANGE_START + 1,
					                RANGE_END + 1);

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
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.appendList(listToAppend);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testCopyAppendNonEmptyToEmptyList)
		{
			List listToAppend =
				createListFromRange(RANGE_START, RANGE_END);
			List list;

			list.appendList(listToAppend);

			Assert::IsTrue(areEqual(listToAppend, list));
		}

		TEST_METHOD(testCopyAppendNonEmptyToNonEmptyList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);
			unsigned SUFFIX_RANGE_SIZE = 5;
			List listToAppend =
				createListFromRange(RANGE_END + 1,
					                RANGE_END + SUFFIX_RANGE_SIZE);

			list.appendList(listToAppend);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END + SUFFIX_RANGE_SIZE));
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
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testMoveAppendNonEmptyToEmptyList)
		{
			List listToAppend =
				createListFromRange(RANGE_START, RANGE_END);
			List list;

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testMoveAppendNonEmptyToNonEmptyList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);
			unsigned SUFFIX_RANGE_SIZE = 5;
			List listToAppend =
				createListFromRange(RANGE_END + 1,
					                RANGE_END + SUFFIX_RANGE_SIZE);

			list.appendList(std::move(listToAppend));

			Assert::IsTrue(listToAppend.isEmpty());
			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END + SUFFIX_RANGE_SIZE));
		}

		TEST_METHOD(testSelfMoveAppendDoesNotChangeTheList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.appendList(std::move(list));

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testInsertAfterInvalidIteratorInsertsBack)
		{
			List list;
			Iterator invalidIterator =
				list.getIteratorToFirst();
			list = createListFromRange(RANGE_START, RANGE_END);
			
			list.insertAfter(invalidIterator, RANGE_END + 1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END + 1));
		}

		TEST_METHOD(testInsertAfterLastItem)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.insertAfter(list.getIteratorToLast(),
				             RANGE_END + 1);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END + 1));
		}

		TEST_METHOD(testInsertAfterMiddleItem)
		{
			List list = createListFromRange(1, 2);
			list.addBack(4);
			Iterator iterator = list.getIteratorToFirst();

			list.insertAfter(++iterator, 3);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 4));
		}

		TEST_METHOD(testInsertBeforeInvalidIteratorInsertsAtFront)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(RANGE_START + 1, RANGE_END);

			list.insertBefore(invalidIterator, RANGE_START);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testInsertBeforeFirstInList)
		{
			List list =
				createListFromRange(RANGE_START + 1, RANGE_END);

			list.insertBefore(list.getIteratorToFirst(),
				              RANGE_START);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testInsertBeforeMiddleItem)
		{
			List list = createListFromRange(1, 1);
			list.addBack(3);

			list.insertBefore(list.getIteratorToLast(), 2);

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
			List list =
				createListFromRange(RANGE_START + 1, RANGE_END);

			list.addFront(RANGE_START);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testAddBack)
		{
			List list;

			for (unsigned i = RANGE_START; i <= RANGE_END; ++i)
			{
				list.addBack(i);
			}

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveAtInvalidIteratorDoesNothing)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(RANGE_START, RANGE_END);

			list.removeAt(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveAtFrontOfList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.removeAt(list.getIteratorToFirst());

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START + 1,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveAtBackOfList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END + 1);

			list.removeAt(list.getIteratorToLast());

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveAtMiddleOfList)
		{
			List list = createListFromRange(1, 2);
			list.insertAfter(list.getIteratorToFirst(), 100);
			Iterator iterator = list.getIteratorToFirst();

			list.removeAt(++iterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list, 1, 2));
		}

		TEST_METHOD(testRemoveAfterInvalidIteratorDoesNothing)
		{
			List list;
			Iterator invalidIterator = list.getIteratorToFirst();
			list = createListFromRange(RANGE_START, RANGE_END);

			list.removeAfter(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveAfterLastDoesNothing)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.removeAfter(list.getIteratorToLast());

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
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
			list = createListFromRange(RANGE_START, RANGE_END);

			list.removeBefore(invalidIterator);

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveBeforeFirstItemDoesNothing)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.removeBefore(list.getIteratorToFirst());

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
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
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.removeFirst();

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START + 1,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveFirstInOneElementList)
		{
			List list = createListFromRange(1, 1);

			list.removeFirst();

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testRemoveLast)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END + 1);

			list.removeLast();

			Assert::IsTrue(listConsistsOfNumbersInRange(list,
				                                        RANGE_START,
				                                        RANGE_END));
		}

		TEST_METHOD(testRemoveLastInOneElementList)
		{
			List list = createListFromRange(1, 1);

			list.removeLast();

			Assert::IsTrue(list.isEmpty());
		}

		TEST_METHOD(testEmptyANonEmptyList)
		{
			List list =
				createListFromRange(RANGE_START, RANGE_END);

			list.empty();

			Assert::IsTrue(list.isEmpty());
		}
	};
}