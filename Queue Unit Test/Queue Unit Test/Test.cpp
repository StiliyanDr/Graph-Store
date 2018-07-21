#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Graph Store/Graph Store/Queue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QueueUnitTest
{		
	TEST_CLASS(QueueTest)
	{
		typedef Queue<int> Queue;

	public:
		Queue createQueueFromRange(int firstNumber, int lastNumber)
		{
			Queue queue;

			for (int number = firstNumber; number <= lastNumber; ++number)
			{
				queue.enqueue(number);
			}

			return queue;
		}

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

		TEST_METHOD(testFirstReturnsTheHeadOfTheQueue)
		{
			Queue queue = createQueueFromRange(1, 5);

			Assert::AreEqual(1, queue.first());
		}
	};
}