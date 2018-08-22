#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Queue/Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QueueUnitTest
{		
	TEST_CLASS(QueueTest)
	{
		typedef Queue<int> Queue;

	private:
		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static Queue createQueueFromRange(int firstNumber, int lastNumber)
		{
			Queue queue;

			for (int number = firstNumber; number <= lastNumber; ++number)
			{
				queue.enqueue(number);
			}

			return queue;
		}

	public:
		TEST_METHOD(testDefaultConstructorCreatesAnEmptyQueue)
		{
			Queue queue;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testEnqueueAndDequeue)
		{
			Queue queue;

			for (int number = 1; number <= 10; ++number)
			{
				queue.enqueue(number);
			}

			for (int number = 1; number <= 10; ++number)
			{
				Assert::AreEqual(number, queue.dequeue());
			}

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(testDequeueOnEmptyQueueThrowsException)
		{
			Queue queue;

			try
			{
				queue.dequeue();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}

		TEST_METHOD(testFirstReturnsTheHeadOfTheQueue)
		{
			Queue queue = createQueueFromRange(1, 5);

			Assert::AreEqual(1, queue.first());
		}

		TEST_METHOD(testFirstOnEmptyQueueThrowsException)
		{
			Queue queue;

			try
			{
				queue.first();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("The queue is empty!", e.what()));
			}
		}
	};
}