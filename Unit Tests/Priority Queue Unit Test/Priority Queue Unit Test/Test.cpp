#include "CppUnitTest.h"
#include "Utility.h"
#include "Priority Queue/PriorityQueue.h"
#include "HandleUpdator.h"
#include "KeyAccessor.h"
#include "Item.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PriorityQueueUnitTest
{
	struct Item
	{
		explicit Item(unsigned key = 0) :
			key(key)
		{
		}

		unsigned key;
		PriorityQueueHandle handle;
	};

	class KeyAccessor
	{
	public:
		const unsigned& getKeyOf(const Item* item) const noexcept
		{
			assert(item != nullptr);

			return item->key;
		}

		void setKeyOfWith(Item* item, unsigned newKey) const noexcept
		{
			assert(item != nullptr);

			item->key = newKey;
		}
	};

	class HandleUpdator
	{
	public:
		void operator()(Item* item, const PriorityQueueHandle& h) const
		{
			assert(item != nullptr);

			item->handle = h;
		}
	};

	TEST_CLASS(PriorityQueueTest)
	{
		using Collection = std::vector<Item*>;
		using PriorityQueue =
			PriorityQueue<Item*, Less, unsigned, KeyAccessor, HandleUpdator>;

		static const std::size_t ARRAY_SIZE = 8;
		static std::vector<Item> items;

		static Collection createReversedArrayOfItems()
		{
			auto itemsReversed = Collection();

			for (auto i = int(ARRAY_SIZE - 1);
				 i >= 0;
				 --i)
			{
				itemsReversed.push_back(&items[i]);
			}

			return itemsReversed;
		}

		static bool queueConsistsOfItemsInRange(PriorityQueue& q,
												std::size_t start,
												std::size_t end)
		{
			assert(isValidRange(start, end));

			for (auto i = start; i <= end; ++i)
			{
				if (q.isEmpty()
				    || q.extractOptimal() != &items[i])
				{
					return false;
				}
			}

			return q.isEmpty();
		}

		static bool isValidRange(std::size_t start, std::size_t end)
		{
			return start <= end && end < ARRAY_SIZE;
		}

		static PriorityQueue createQueueFromItemsInRange(std::size_t start,
														 std::size_t end)
		{
			auto q = PriorityQueue();
			fillQueueWithItemsInRange(q, start, end);

			return q;
		}

		static void fillQueueWithItemsInRange(PriorityQueue& q,
											  std::size_t start,
											  std::size_t end)
		{
			assert(isValidRange(start, end));

			for (auto i = start; i <= end; ++i)
			{
				q.add(&items[i]);
			}
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			assert(lhs != nullptr && rhs != nullptr);

			return strcmp(lhs, rhs) == 0;
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

		TEST_METHOD(testCtorFromRange)
		{
			std::vector<Item*> items = createReversedArrayOfItems();

			PriorityQueue queue(items.begin(), items.end());

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ARRAY_SIZE - 1));
		}

		TEST_METHOD(testCtorFromEmptyRangeCreatesAnEmptyQueue)
		{
			std::vector<Item*> items;

			PriorityQueue queue(items.begin(), items.end());

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

			const Item* optimalItem = queue.getOptimal();
			Assert::IsTrue(optimalItem == &items[0]);
		}

		TEST_METHOD(testExtractingTheOnlyItemLeavesTheQueueEmpty)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, 0);

			queue.extractOptimal();

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testExtractOptimalMaintainsOrderOfPriority)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			Item* optimalItem = queue.extractOptimal();

			Assert::IsTrue(optimalItem == &items[0], L"The method did not extract the item with optimal key!");
			Assert::IsTrue(queue.getOptimal() == &items[1]);
		}

		TEST_METHOD(testExtractOptimalFromEmptyQueueThrowsException)
		{
			PriorityQueue queue;

			try
			{
				queue.extractOptimal();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}

		TEST_METHOD(testGetOptimal)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);

			const Item* optimalItem = queue.getOptimal();

			Assert::IsTrue(optimalItem == &items[0]);
		}

		TEST_METHOD(testGetOptimalFromEmptyQueueThrowsException)
		{
			PriorityQueue queue;

			try
			{
				queue.getOptimal();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}

		TEST_METHOD(testOptimiseKeyWithNewOptimalKeyUpdatesOptimalItem)
		{
			PriorityQueue queue = createQueueFromItemsInRange(ARRAY_SIZE / 2, ARRAY_SIZE - 1);
			Item& itemWithNonOptimalKey = items[ARRAY_SIZE - 1];
			PriorityQueueHandle handle = itemWithNonOptimalKey.handle;

			queue.optimiseKey(handle, 0);

			const Item* optimalItem = queue.getOptimal();
			Assert::AreEqual(0u, optimalItem->key);
			Assert::IsTrue(optimalItem == &itemWithNonOptimalKey);
		}

		TEST_METHOD(testOptimiseKeyOfOptimalItem)
		{
			PriorityQueue queue = createQueueFromItemsInRange(ARRAY_SIZE / 2, ARRAY_SIZE - 1);
			Item& itemWithOptimalKey = items[ARRAY_SIZE / 2];
			PriorityQueueHandle handle = itemWithOptimalKey.handle;

			queue.optimiseKey(handle, 0);

			const Item* optimalItem = queue.getOptimal();
			Assert::AreEqual(0u, optimalItem->key);
			Assert::IsTrue(optimalItem == &itemWithOptimalKey);
		}

		TEST_METHOD(testOptimiseKeyOfNonOptimalItemWithNonOptimalKey)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ARRAY_SIZE / 2);
			Item& itemWithNonOptimalKey = items[ARRAY_SIZE / 2];
			PriorityQueueHandle handle = itemWithNonOptimalKey.handle;

			queue.optimiseKey(handle, 1);

			Assert::IsTrue(queue.getOptimal() == &items[0]);
		}

		TEST_METHOD(testOptimiseKeyWithWorseKeyThrowsException)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, 0);

			try
			{
				queue.optimiseKey(items[0].handle, 1);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::invalid_argument& e)
			{
				Assert::IsTrue(areEqual("The key can't be worsened!", e.what()));
			}
		}

		TEST_METHOD(testOptimiseKeyWithInvalidHandleThrowsException)
		{
			PriorityQueue queue;
			PriorityQueueHandle invalidHandle;

			try
			{
				queue.optimiseKey(invalidHandle, 100);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::invalid_argument& e)
			{
				Assert::IsTrue(areEqual("Invalid handle!", e.what()));
			}
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

	std::vector<Item> PriorityQueueTest::items(ARRAY_SIZE);
}