#include "CppUnitTest.h"
#include "Graph Collection\GraphCollection.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GraphCollectionUnitTest
{		
	TEST_CLASS(GraphCollectionTest)
	{
	public:
		TEST_METHOD(testDefaultCtorCreatesAnEmptyCollection)
		{
			GraphCollection graphs;

			Assert::IsTrue(graphs.isEmpty());
		}

		TEST_METHOD(testCtorFromSizeCreatesAnEmptyCollection)
		{
			std::size_t expectedSize = 10;
			GraphCollection graphs(expectedSize);

			Assert::IsTrue(graphs.isEmpty());
		}

	};
}