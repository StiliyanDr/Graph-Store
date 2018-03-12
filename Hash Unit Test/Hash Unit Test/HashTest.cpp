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
				hash.insert(books[from++]);
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

		TEST_METHOD(insertingAndSearching)
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
				hash.insert(books[i]);
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