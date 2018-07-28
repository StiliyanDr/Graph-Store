#include "stdafx.h"
#include "CppUnitTest.h"
#include <utility>
#include "../../Graph Store/Graph Store/PriorityQueue.h"
#include "../../Graph Store/Graph Store/DynamicArray.h"
#include "HandleUpdator.h"
#include "KeyAccessor.h"
#include "Item.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PriorityQueueUnitTest
{		
	TEST_CLASS(PriorityQueueTest)
	{
		typedef PriorityQueue<Item, unsigned, HandleUpdator, KeyAccessor> PriorityQueue;

	private:
		static const size_t ARRAY_SIZE = 8;
		static DynamicArray<Item> items;

	private:
		static DynamicArray<Item*> createReversedArrayOfItems()
		{
			DynamicArray<Item*> itemsReversed(ARRAY_SIZE);

			for (int i = ARRAY_SIZE - 1; i >= 0; --i)
			{
				itemsReversed.add(&items[i]);
			}

			return itemsReversed;
		}

		static bool queueConsistsOfItemsInRange(PriorityQueue& queue,
												unsigned firstNumber,
												unsigned lastNumber)
		{
			assert(isValidRange(firstNumber, lastNumber));
			Item* item;

			for (unsigned i = firstNumber; i <= lastNumber; ++i)
			{
				assert(!queue.isEmpty());
				item = queue.extractMin();
				
				if (item != &items[i])
				{
					return false;
				}
			}

			return queue.isEmpty();
		}

		static PriorityQueue createQueueFromItemsInRange(unsigned firstNumber,
														 unsigned lastNumber)
		{
			PriorityQueue queue;
			fillQueueWithItemsInRange(queue, firstNumber, lastNumber);

			return queue;
		}

		static void fillQueueWithItemsInRange(PriorityQueue& queue,
											  unsigned firstNumber,
											  unsigned lastNumber)
		{
			assert(isValidRange(firstNumber, lastNumber));

			for (unsigned i = firstNumber; i <= lastNumber; ++i)
			{
				queue.add(&items[i]);
			}
		}

		static bool isValidRange(unsigned firstNumber, unsigned lastNumber)
		{
			return firstNumber <= lastNumber && lastNumber < ARRAY_SIZE;
		}

	public:
		TEST_METHOD_INITIALIZE(initialiseItems)
		{
			for (size_t i = 0; i < ARRAY_SIZE; ++i)
			{
				items[i] = Item(i);
			}
		}

		TEST_METHOD(testDefaultCtorCreatesAnEmptyQueue)
		{
			PriorityQueue queue;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testCtorFromIteratorAndCountInsertsFirstCountElements)
		{
			DynamicArray<Item*> itemsReversed = createReversedArrayOfItems();
			DynamicArrayIterator<Item*> iterator = itemsReversed.getIteratorToFirst();
			size_t numberOfItemsToInsert = itemsReversed.getCount();
			
			PriorityQueue queue(iterator, numberOfItemsToInsert);

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE - 1));
		}

		TEST_METHOD(testCtorFromIteratorAndNullCreatesAnEmptyQueue)
		{
			DynamicArray<Item*> items;
			DynamicArrayIterator<Item*> iterator = items.getIteratorToFirst();

			PriorityQueue queue(iterator, 0);

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testCopyCtorFromEmptyQueue)
		{
			PriorityQueue queueToCopy;
			
			PriorityQueue queue(queueToCopy);

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testCopyCtorFromNonEmptyQueue)
		{
			PriorityQueue queueToCopy = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			PriorityQueue queue(queueToCopy);

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyQueue)
		{
			PriorityQueue queueToCopy;
			PriorityQueue queue;

			queue = queueToCopy;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyQueue)
		{
			PriorityQueue queueToCopy;
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			queue = queueToCopy;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyQueue)
		{
			PriorityQueue queueToCopy = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			PriorityQueue queue;

			queue = queueToCopy;

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyQueue)
		{
			PriorityQueue queueToCopy =
				createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			PriorityQueue queue =
				createQueueFromItemsInRange((ARRAY_SIZE / 2) + 1, ARRAY_SIZE - 1);

			queue = queueToCopy;

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testMoveCtorFromEmptyQueue)
		{
			PriorityQueue queueToMove;

			PriorityQueue queue(std::move(queueToMove));

			Assert::IsTrue(queue.isEmpty());
			Assert::IsTrue(queueToMove.isEmpty(), L"The moved-from queue did not become empty!");
		}

		TEST_METHOD(testMoveCtorFromNonEmptyQueue)
		{
			PriorityQueue queueToMove;
			fillQueueWithItemsInRange(queueToMove, 0, ARRAY_SIZE / 2);

			PriorityQueue queue(std::move(queueToMove));

			Assert::IsTrue(queueToMove.isEmpty(), L"The moved-from queue did not become empty!");
			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyQueue)
		{
			PriorityQueue queueToMove;
			PriorityQueue queue;

			queue = std::move(queueToMove);

			Assert::IsTrue(queue.isEmpty());
			Assert::IsTrue(queueToMove.isEmpty(), L"The moved-from queue did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyQueue)
		{
			PriorityQueue queueToMove;
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			queue = std::move(queueToMove);

			Assert::IsTrue(queue.isEmpty(), L"The moved-into queue did not become empty!");
			Assert::IsTrue(queueToMove.isEmpty());
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			PriorityQueue queueToMove = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			PriorityQueue queue;

			queue = std::move(queueToMove);

			Assert::IsTrue(queueToMove.isEmpty(), L"The moved-from queue did not become empty!");
			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyQueue)
		{
			PriorityQueue queueToMove =
				createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			PriorityQueue queue =
				createQueueFromItemsInRange((ARRAY_SIZE / 2) + 1, ARRAY_SIZE - 1);

			queue = std::move(queueToMove);

			Assert::IsTrue(queueToMove.isEmpty(), L"The moved-from queue did not become empty!");
			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE / 2));
		}

		TEST_METHOD(testAddMaintainsOrderOfPriority)
		{
			PriorityQueue queue;
			size_t middle = ARRAY_SIZE / 2;

			fillQueueWithItemsInRange(queue, middle, ARRAY_SIZE - 1);
			fillQueueWithItemsInRange(queue, 0, middle - 1);

			const Item* minItem = queue.getMin();
			Assert::IsTrue(minItem == &items[0]);
		}

		TEST_METHOD(testExtractingTheOnlyItemLeavesTheQueueEmpty)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, 0);

			queue.extractMin();

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testExtractMinMaintainsOrderOfPriority)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			Item* minItem = queue.extractMin();

			Assert::IsTrue(minItem == &items[0], L"The method did not extract the item with min key!");
			Assert::IsTrue(queue.getMin() == &items[1]);
		}

		TEST_METHOD(testGetMin)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			const Item* minItem = queue.getMin();

			Assert::IsTrue(minItem == &items[0]);
		}

		TEST_METHOD(testDecreaseKeyWithNewMinKeyUpdatesMinItem)
		{
			PriorityQueue queue = createQueueFromItemsInRange(ARRAY_SIZE / 2, ARRAY_SIZE - 1);
			Item& itemWithMaxKey = items[ARRAY_SIZE - 1];
			PriorityQueueHandle handle = itemWithMaxKey.handle;

			queue.decreaseKey(handle, 0);

			const Item* minItem = queue.getMin();
			Assert::AreEqual(0u, minItem->key);
			Assert::IsTrue(minItem == &itemWithMaxKey);
		}

		TEST_METHOD(testDecreaseKeyOfMinItem)
		{
			PriorityQueue queue = createQueueFromItemsInRange(ARRAY_SIZE / 2, ARRAY_SIZE - 1);
			Item& itemWithMinKey = items[ARRAY_SIZE / 2];
			PriorityQueueHandle handle = itemWithMinKey.handle;

			queue.decreaseKey(handle, 0);

			const Item* minItem = queue.getMin();
			Assert::AreEqual(0u, minItem->key);
			Assert::IsTrue(minItem == &itemWithMinKey);
		}

		TEST_METHOD(testDecreaseKeyOfNonMinItemWithNonMinKey)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			Item& itemWithNonMinKey = items[ARRAY_SIZE / 2];
			PriorityQueueHandle handle = itemWithNonMinKey.handle;

			queue.decreaseKey(handle, 1);

			Assert::IsTrue(queue.getMin() == &items[0]);
		}

		TEST_METHOD(testAddAndExtractAllItems)
		{
			PriorityQueue queue;
			size_t middle = ARRAY_SIZE / 2;

			fillQueueWithItemsInRange(queue, middle, ARRAY_SIZE - 1);
			fillQueueWithItemsInRange(queue, 0, middle - 1);

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE - 1));
		}

	};

	DynamicArray<Item> PriorityQueueTest::items(ARRAY_SIZE, ARRAY_SIZE);
}