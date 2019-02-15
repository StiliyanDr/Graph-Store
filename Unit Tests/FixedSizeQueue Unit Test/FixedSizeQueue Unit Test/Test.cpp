#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Fixed Size Queue/FixedSizeQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FixedSizeQueueUnitTest
{		
	TEST_CLASS(FixedSizeQueueTest)
	{
		using Queue = FixedSizeQueue<unsigned>;
		
		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static Queue makeQueueFromRange(unsigned start, unsigned end)
		{
			assert(start <= end);
			
			Queue q(end - start + 1);

			for (unsigned i = start; i <= end; ++i)
			{
				q.enqueue(i);
			}

			return q;
		}

	public:
		TEST_METHOD(testAQueueForZeroElementsIsFull)
		{
			Queue q(0);

			Assert::IsTrue(q.isFull());
		}
		
		TEST_METHOD(testCtorCreatesAnEmptyQueue)
		{
			Queue q(10);

			Assert::IsTrue(q.isEmpty());
			Assert::IsFalse(q.isFull());
		}

		TEST_METHOD(testEnqueueInsertsAtTheBackOfTheQueue)
		{
			Queue q(5);
			
			q.enqueue(1);
			q.enqueue(2);

			Assert::IsFalse(q.isEmpty());
			Assert::AreEqual(1u, q.first());
		}

		TEST_METHOD(testEnqueueThrowsExceptionIfTheQueueIsFull)
		{
			Queue q(1);
			q.enqueue(0);
			Assert::IsTrue(q.isFull());

			try
			{
				q.enqueue(1);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is full!", e.what()));
			}
		}

		TEST_METHOD(testDequeueExtractsTheHeadOfTheQueue)
		{
			Queue q = makeQueueFromRange(1, 3);

			unsigned head = q.dequeue();

			Assert::AreEqual(1u, head);
			Assert::IsFalse(q.isEmpty());
			Assert::AreEqual(2u, q.first());
		}

		TEST_METHOD(testTheQueueIsEmptyAfterDequeuingTheOnlyElement)
		{
			Queue q = makeQueueFromRange(1, 1);

			q.dequeue();

			Assert::IsFalse(q.isFull());
			Assert::IsTrue(q.isEmpty());
		}

		TEST_METHOD(testDequeueThrowsExceptionIfTheQueueIsEmpty)
		{
			Queue q(1);

			try
			{
				q.dequeue();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}

		TEST_METHOD(testFirstReturnsACopyOfTheFirstElement)
		{
			Queue q = makeQueueFromRange(1, 3);

			Assert::IsFalse(q.isEmpty());
			Assert::AreEqual(1u, q.first());
		}

		TEST_METHOD(testFirstThrowsExceptionIfTheQueueIsEmpty)
		{
			Queue q(1);

			try
			{
				q.first();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}
	};
}
