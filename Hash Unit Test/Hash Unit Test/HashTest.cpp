#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Graph Store/Graph Store/Hash.h"
#include "../../Graph Store/Graph Store/HashFunctionStringSpecialization.h"
#include "Book.h"
#include "BookTitleAccessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HashUnitTest
{		
	TEST_CLASS(HashTest)
	{
		typedef Hash<Book, String, BookTitleAccessor> Hash;
		
		static const size_t ARRAY_SIZE = 16;
		static Book books[ARRAY_SIZE];

	public:

		void fillHash(Hash& hash, size_t from, size_t to)
		{
			while (from <= to)
				hash.add(books[from++]);
		}

		TEST_CLASS_INITIALIZE(HashTestInitialise)
		{
			static const char* titles[ARRAY_SIZE] = { "one", "two", "three", "four", "five", "six", "seven", 
				"eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen" };

			for (int i = 0; i < ARRAY_SIZE; ++i)
				books[i].setTitle(titles[i]);
		}

		TEST_METHOD(integerConstructor)
		{
			for (int i = 1; i <= 50; ++i)
			{
				Hash hash(i);
				Assert::AreEqual(0u, hash.getCount());
				Assert::IsTrue(hash.isEmpty());
			}

			for (int i = 0; i <= 10; ++i)
			{
				try
				{
					Hash hash(-i);

					Assert::Fail(L"Created an object with an invalid size!");
				}
				catch (...)
				{
				}
			}
		}

		TEST_METHOD(moveConstruction)
		{
			Hash hash(1);
			
			Hash fromEmpty(std::move(hash));
			Assert::IsTrue(fromEmpty.isEmpty() && hash.isEmpty());

			fillHash(hash, 0, ARRAY_SIZE - 1);
			
			Hash fromNonEmpty(std::move(hash));
			Assert::IsTrue(hash.isEmpty());

			fillHash(hash, 0, ARRAY_SIZE - 1);

			for (int i = 0; i < ARRAY_SIZE - 1; ++i)
			{
				Assert::IsNotNull(hash.search(books[i].getTitle()));
				Assert::IsNotNull(fromNonEmpty.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(copyConstruction)
		{
			Hash hash(10);

			Hash fromEmpty(hash);
			Assert::IsTrue(fromEmpty.isEmpty());
		
			fillHash(hash, 0, ARRAY_SIZE - 1);

			Hash fromNonEmpty(hash);
			Assert::AreEqual(hash.getCount(), fromNonEmpty.getCount());

			for (int i = 0; i < ARRAY_SIZE - 1; ++i)
				Assert::IsNotNull(fromNonEmpty.search(books[i].getTitle()));
		}

		TEST_METHOD(moveAssigningEmptyObject)
		{
			Hash empty(1);

			Hash lhs(1);
			lhs = std::move(empty);
			Assert::IsTrue(lhs.isEmpty() && empty.isEmpty());

			fillHash(lhs, 0, ARRAY_SIZE - 1);

			lhs = std::move(empty);
			Assert::IsTrue(lhs.isEmpty() && empty.isEmpty());
		}

		TEST_METHOD(moveAssigningNonEmptyObject)
		{
			size_t middle = ARRAY_SIZE / 2;

			Hash rhs(ARRAY_SIZE), lhs(1);
			fillHash(rhs, 0, middle - 1);

			lhs = std::move(rhs);
			Assert::IsTrue(rhs.isEmpty());

			for (size_t i = 0; i < middle; ++i)
				Assert::IsNotNull(lhs.search(books[i].getTitle()));

			fillHash(rhs, middle, ARRAY_SIZE - 1);

			lhs = std::move(rhs);
			Assert::IsTrue(rhs.isEmpty());

			for (size_t i = 0; i < middle; ++i)
				Assert::IsNull(lhs.search(books[i].getTitle()));

			for (size_t i = middle; i < ARRAY_SIZE; ++i)
				Assert::IsNotNull(lhs.search(books[i].getTitle()));
		}

		TEST_METHOD(copyAssigningEmptyObject)
		{
			Hash empty(1), lhs(ARRAY_SIZE);

			fillHash(lhs, 0, ARRAY_SIZE - 1);

			for (int i = 1; i <= 2; ++i)
			{
				lhs = empty;
				Assert::IsTrue(empty.isEmpty() && lhs.isEmpty());
			}
		}

		TEST_METHOD(copyAssigningNonEmptyObject)
		{
			size_t middle = ARRAY_SIZE / 2;

			Hash lhs(1), rhs(ARRAY_SIZE);

			fillHash(rhs, 0, middle - 1);

			lhs = rhs;
			Assert::AreEqual(middle, rhs.getCount());

			for (size_t i = 0; i < middle; ++i)
				Assert::IsNotNull(lhs.search(books[i].getTitle()));

			fillHash(rhs, middle, ARRAY_SIZE - 1);

			lhs = rhs;
			Assert::AreEqual(ARRAY_SIZE, rhs.getCount());

			for (size_t i = 0; i < ARRAY_SIZE; ++i)
			{
				Assert::IsNotNull(lhs.search(books[i].getTitle()));
				Assert::IsNotNull(rhs.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(addingAndSearching)
		{
			Hash hash(ARRAY_SIZE);

			const int middle = ARRAY_SIZE / 2;

			fillHash(hash, 0, middle - 1);

			for (int i = 0; i < middle; ++i)
				Assert::IsNotNull(hash.search(books[i].getTitle()));

			for (int i = middle; i < ARRAY_SIZE; ++i)
				Assert::IsNull(hash.search(books[i].getTitle()));
			
			fillHash(hash, middle, ARRAY_SIZE - 1);

			for (int i = 0; i < ARRAY_SIZE; ++i)
				Assert::IsNotNull(hash.search(books[i].getTitle()));

			Assert::AreEqual(ARRAY_SIZE, hash.getCount());
		}

		TEST_METHOD(correctCountWithInsertion)
		{
			Hash hash(1);

			for (size_t i = 0; i < ARRAY_SIZE; ++i)
			{
				hash.add(books[i]);
				Assert::AreEqual(i + 1, hash.getCount());
			}
		}

		TEST_METHOD(exceedingExpectedCount)
		{
			Hash hash(1);

			fillHash(hash, 0, ARRAY_SIZE - 1);

			Assert::AreEqual(ARRAY_SIZE, hash.getCount());

			for (int i = 0; i < ARRAY_SIZE; ++i)
				Assert::IsNotNull(hash.search(books[i].getTitle()));
		}

		TEST_METHOD(removal)
		{
			Hash hash(ARRAY_SIZE);
		
			const int end = ARRAY_SIZE - 1;

			fillHash(hash, 0, end);

			for (int i = end; i >= 0; --i)
			{
				Assert::IsTrue(&books[i] == hash.remove(books[i].getTitle()));

				Assert::AreEqual((size_t)i, hash.getCount());

				Assert::IsNull(hash.search(books[i].getTitle()));

				for (int k = 0; k < i; ++k)
					Assert::IsNotNull(hash.search(books[k].getTitle()));
			}

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(emptying)
		{
			Hash hash(ARRAY_SIZE);

			fillHash(hash, 0, ARRAY_SIZE - 1);

			Assert::IsFalse(hash.isEmpty());
			Assert::AreEqual(ARRAY_SIZE, hash.getCount());

			hash.empty();

			Assert::AreEqual(0u, hash.getCount());
			Assert::IsTrue(hash.isEmpty());
		}

	};

	Book HashTest::books[ARRAY_SIZE];
}