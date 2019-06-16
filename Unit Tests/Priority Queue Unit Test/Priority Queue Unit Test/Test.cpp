#include "CppUnitTest.h"
#include "Utility.h"
#include "Priority Queue/PriorityQueue.h"
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

		static const std::size_t ITEMS_COUNT = 8;
		static std::vector<Item> items;

		static Collection createCollectionOfAllItems()
		{
			auto itemsReversed = Collection();

			for (auto i = int(ITEMS_COUNT - 1);
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
			return start <= end && end < ITEMS_COUNT;
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

		static bool itemsHaveInvalidHandles(std::size_t start = 0,
											std::size_t end = ITEMS_COUNT - 1)
		{
			for (auto i = start; i <= end; ++i)
			{
				if (items[i].handle.isValid())
				{
					return false;
				}
			}

			return true;
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			assert(lhs != nullptr && rhs != nullptr);

			return strcmp(lhs, rhs) == 0;
		}

	public:
		TEST_METHOD_INITIALIZE(initialiseItems)
		{
			for (auto i = 0u; i < ITEMS_COUNT; ++i)
			{
				items[i] = Item(i);
			}
		}

		TEST_METHOD(testDefaultCtorCreatesAnEmptyQueue)
		{
			auto q = PriorityQueue();

			Assert::IsTrue(q.isEmpty());
		}

		TEST_METHOD(testCtorFromRange)
		{
			auto items = createCollectionOfAllItems();

			auto q = PriorityQueue(items.begin(), items.end());

			Assert::IsTrue(queueConsistsOfItemsInRange(q, 0, ITEMS_COUNT - 1));
		}

		TEST_METHOD(testCtorFromEmptyRangeCreatesAnEmptyQueue)
		{
			auto items = Collection();

			auto q = PriorityQueue(items.begin(), items.end());

			Assert::IsTrue(q.isEmpty());
		}

		TEST_METHOD(testMoveCtorFromEmptyQueue)
		{
			auto queueToMove = PriorityQueue();

			auto q = PriorityQueue(std::move(queueToMove));

			Assert::IsTrue(q.isEmpty());
			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
		}

		TEST_METHOD(testMoveCtorFromNonEmptyQueue)
		{
			auto queueToMove = PriorityQueue();
			fillQueueWithItemsInRange(queueToMove, 0, ITEMS_COUNT / 2);

			auto q = PriorityQueue(std::move(queueToMove));

			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
			Assert::IsTrue(queueConsistsOfItemsInRange(q, 0, ITEMS_COUNT / 2));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyQueue)
		{
			auto queueToMove = PriorityQueue();
			auto q = PriorityQueue();

			q = std::move(queueToMove);

			Assert::IsTrue(q.isEmpty());
			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyQueue)
		{
			auto queueToMove = PriorityQueue();
			auto q = createQueueFromItemsInRange(0, ITEMS_COUNT / 2);

			q = std::move(queueToMove);

			Assert::IsTrue(q.isEmpty(),
				           L"The moved-into queue did not become empty!");
			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
			Assert::IsTrue(itemsHaveInvalidHandles(0, ITEMS_COUNT / 2));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyQueue)
		{
			auto queueToMove =
				createQueueFromItemsInRange(0, ITEMS_COUNT / 2);
			auto q = PriorityQueue();

			q = std::move(queueToMove);

			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
			Assert::IsTrue(queueConsistsOfItemsInRange(q, 0, ITEMS_COUNT / 2));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyQueue)
		{
			auto queueToMove =
				createQueueFromItemsInRange(0, ITEMS_COUNT / 2);
			auto q =
				createQueueFromItemsInRange((ITEMS_COUNT / 2) + 1, ITEMS_COUNT - 1);

			q = std::move(queueToMove);

			Assert::IsTrue(queueToMove.isEmpty(),
				           L"The moved-from queue did not become empty!");
			Assert::IsTrue(itemsHaveInvalidHandles((ITEMS_COUNT / 2) + 1,
				                                   ITEMS_COUNT - 1),
				           L"Handles of removed elements did not become invalid!");
			Assert::IsTrue(queueConsistsOfItemsInRange(q, 0, ITEMS_COUNT / 2));
		}

		TEST_METHOD(testDestructorInvalidatesAllHandles)
		{
			{
				auto q =
					createQueueFromItemsInRange(0, ITEMS_COUNT - 1);
			}

			Assert::IsTrue(itemsHaveInvalidHandles());
		}

		TEST_METHOD(testAddMaintainsOrderOfPriority)
		{
			PriorityQueue queue;
			size_t middle = ITEMS_COUNT / 2;

			fillQueueWithItemsInRange(queue, middle, ITEMS_COUNT - 1);
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
			PriorityQueue queue = createQueueFromItemsInRange(0, ITEMS_COUNT / 2);

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
			PriorityQueue queue = createQueueFromItemsInRange(0, ITEMS_COUNT / 2);

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
			PriorityQueue queue = createQueueFromItemsInRange(ITEMS_COUNT / 2, ITEMS_COUNT - 1);
			Item& itemWithNonOptimalKey = items[ITEMS_COUNT - 1];
			PriorityQueueHandle handle = itemWithNonOptimalKey.handle;

			queue.optimiseKey(handle, 0);

			const Item* optimalItem = queue.getOptimal();
			Assert::AreEqual(0u, optimalItem->key);
			Assert::IsTrue(optimalItem == &itemWithNonOptimalKey);
		}

		TEST_METHOD(testOptimiseKeyOfOptimalItem)
		{
			PriorityQueue queue = createQueueFromItemsInRange(ITEMS_COUNT / 2, ITEMS_COUNT - 1);
			Item& itemWithOptimalKey = items[ITEMS_COUNT / 2];
			PriorityQueueHandle handle = itemWithOptimalKey.handle;

			queue.optimiseKey(handle, 0);

			const Item* optimalItem = queue.getOptimal();
			Assert::AreEqual(0u, optimalItem->key);
			Assert::IsTrue(optimalItem == &itemWithOptimalKey);
		}

		TEST_METHOD(testOptimiseKeyOfNonOptimalItemWithNonOptimalKey)
		{
			PriorityQueue queue = createQueueFromItemsInRange(0, ITEMS_COUNT / 2);
			Item& itemWithNonOptimalKey = items[ITEMS_COUNT / 2];
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
			size_t middle = ITEMS_COUNT / 2;

			fillQueueWithItemsInRange(queue, middle, ITEMS_COUNT - 1);
			fillQueueWithItemsInRange(queue, 0, middle - 1);

			Assert::IsTrue(queueConsistsOfItemsInRange(queue, 0, ITEMS_COUNT - 1));
		}

	};

	std::vector<Item> PriorityQueueTest::items(ITEMS_COUNT);
}