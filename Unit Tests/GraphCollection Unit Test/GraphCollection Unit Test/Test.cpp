#include "CppUnitTest.h"
#include "Graph Collection\GraphCollection.h"
#include "Graph\Undirected Graph\UndirectedGraph.h"
#include "Runtime Error\RuntimeError.h"

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

		TEST_METHOD(testAdd)
		{
			GraphCollection graphs(1);
			String id = "g";
			
			graphs.add(makeGraph(id));

			Assert::IsFalse(graphs.isEmpty());
			Assert::AreEqual(1u, graphs.getCount());
			Assert::IsTrue(graphs.contains(id));
		}

		TEST_METHOD(testAddThrowsExceptionForADuplicateName)
		{
			String id = "g";
			GraphCollection graphs =
				makeCollectionContainingGraphWithID(id);

			try
			{
				graphs.add(makeGraph(id));
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (RuntimeError&)
			{
			}
		}

		TEST_METHOD(testAddThrowsExceptionForNullpointer)
		{
			GraphCollection graphs;

			try
			{
				graphs.add(nullptr);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (RuntimeError&)
			{
			}
		}

		TEST_METHOD(testOperatorSubscript)
		{
			const String id = "test id";
			GraphCollection graphs =
				makeCollectionContainingGraphWithID(id);

			Graph& g = graphs[id];

			Assert::IsTrue(id == g.getID());
		}

		TEST_METHOD(testOperatorSubscriptThrowsExceptionForNonmatchingID)
		{
			GraphCollection graphs;

			try
			{
				graphs["nonmatching id"];
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range&)
			{
			}
		}

		TEST_METHOD(testContains)
		{
			String id = "g";
			GraphCollection graphs =
				makeCollectionContainingGraphWithID(id);

			Assert::IsTrue(graphs.contains(id));
			Assert::IsFalse(graphs.contains("nonmatching id"));
		}

		TEST_METHOD(testRemove)
		{
			String id = "g";
			GraphCollection graphs =
				makeCollectionContainingGraphWithID(id);

			graphs.remove(id);

			Assert::IsTrue(graphs.isEmpty());
		}

		TEST_METHOD(testRemoveThrowsExceptionForNonmatchingID)
		{
			GraphCollection graphs;

			try
			{
				graphs.remove("nonmatching id");
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range&)
			{
			}
		}

		TEST_METHOD(testEmpty)
		{
			GraphCollection graphs =
				makeCollectionContainingGraphWithID("id");

			graphs.empty();

			Assert::IsTrue(graphs.isEmpty());
		}

	private:
		static GraphCollection
		makeCollectionContainingGraphWithID(const String& id)
		{
			GraphCollection graphs(1);
			graphs.add(makeGraph(id));

			return graphs;
		}

		static GraphCollection::GraphPointer makeGraph(const String& id)
		{
			return std::make_unique<UndirectedGraph>(id);
		}
	};
}