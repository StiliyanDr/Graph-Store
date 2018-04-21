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

		TEST_METHOD(defaultConstructor)
		{
			Queue queue;

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(isEmpty)
		{
			Queue queue;
			Assert::IsTrue(queue.isEmpty());

			for (int i = 1; i <= 10; ++i)
			{
				queue.enqueue(i);
				Assert::IsFalse(queue.isEmpty());
			}
		}

		TEST_METHOD(enqueueAndDequeue)
		{
			Queue queue;

			for (int i = 1; i <= 20; ++i)
				queue.enqueue(i);

			for (int i = 1; i <= 20; ++i)
				Assert::AreEqual(i, queue.dequeue());

			Assert::IsTrue(queue.isEmpty());
		}

		TEST_METHOD(dequeueAnEmptyQueue)
		{
			Queue queue;

			try
			{
				queue.dequeue();

				Assert::Fail(L"Dequeued an empty queue!");
			}
			catch (...)
			{
			}
		}

		TEST_METHOD(first)
		{
			Queue queue;

			for (int i = 1; i <= 10; ++i)
				queue.enqueue(i);

			for (int i = 1; i <= 10; ++i)
			{
				Assert::AreEqual(i, queue.first());
				queue.dequeue();
			}
		}

		TEST_METHOD(firstInEmptyQueue)
		{
			Queue queue;

			try
			{
				queue.first();

				Assert::Fail(L"Got a copy of the first element in an empty queue!");
			}
			catch (...)
			{
			}
		}
	};
}