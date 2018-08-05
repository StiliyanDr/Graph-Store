#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Hash/Hash.h"
#include "../../../Graph Store/Graph Store/Hash/Hash Function/HashFunctionStringSpecialization.h"
#include "Book.h"
#include "BookTitleAccessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HashUnitTest
{		
	TEST_CLASS(HashTest)
	{
		typedef Hash<Book, String, BookTitleAccessor> Hash;
		
		static const size_t BOOKS_COUNT = 16;
		static Book books[BOOKS_COUNT];

	public:

		void fillHashWithBooksFromTo(Hash& hash, size_t from, size_t to)
		{
			assert(from < BOOKS_COUNT);
			assert(to < BOOKS_COUNT);

			for (size_t i = from; i <= to; ++i)
			{
				hash.add(books[i]);
			}
		}

		bool hashConsistsOfBooksFromTo(Hash& hash, size_t from, size_t to)
		{
			assert(from <= to);
			
			size_t booksCount = to - from + 1;

			return booksCount == hash.getCount()
				   && hashContainsBooksFromTo(hash, from, to);
		}

		bool hashContainsBooksFromTo(Hash& hash, size_t from, size_t to)
		{
			assert(from < BOOKS_COUNT);
			assert(to < BOOKS_COUNT);

			for (size_t i = from; i <= to; ++i)
			{
				if (!hash.search(books[i].getTitle()))
				{
					return false;
				}
			}

			return true;
		}

		TEST_CLASS_INITIALIZE(initialiseHashTest)
		{
			static const char* titles[BOOKS_COUNT] = { "one", "two", "three", "four", "five", "six", "seven", 
				"eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen" };

			for (int i = 0; i < BOOKS_COUNT; ++i)
			{
				books[i].setTitle(titles[i]);
			}
		}

		TEST_METHOD(testConstructorFromIntegerCreatesEmptyHash)
		{
			Hash hash(100);

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testConstructorFromNilThrowsException)
		{
			try
			{
				Hash hash(0);

				Assert::Fail(L"Constructor should have thrown an exception!");
			}
			catch (std::invalid_argument&)
			{
			}
		}

		TEST_METHOD(testMoveConstructorFromEmptyHash)
		{
			Hash emptyHash(1);

			Hash hash(std::move(emptyHash));

			Assert::IsTrue(emptyHash.isEmpty(), L"The moved-from hash should be empty!");
			Assert::IsTrue(hash.isEmpty(), L"The moved-into hash should be empty!");
		}

		TEST_METHOD(testMoveConstructorFromNonEmptyHash)
		{
			Hash hashToMove(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToMove, 0, BOOKS_COUNT / 2);

			Hash hash(std::move(hashToMove));

			Assert::IsTrue(hashToMove.isEmpty(), L"The moved-from hash should be empty!");
			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testCopyConstructorFromEmptyHash)
		{
			Hash emptyHash(1);

			Hash hash(emptyHash);

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyHash)
		{
			Hash hashToCopy(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToCopy, 0, BOOKS_COUNT / 2);

			Hash hash(hashToCopy);

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyHash)
		{
			Hash emptyHash(1);
			Hash hash(1);

			hash = emptyHash;

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyHash)
		{
			Hash emptyHash(1);
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT / 2);

			hash = emptyHash;

			Assert::IsTrue(hash.isEmpty());
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyHash)
		{
			Hash hashToCopy(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToCopy, 0, BOOKS_COUNT / 2);
			Hash hash(1);

			hash = hashToCopy;

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyHash)
		{
			Hash hashToCopy(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToCopy, 0, BOOKS_COUNT / 2);
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, BOOKS_COUNT / 2 + 1, BOOKS_COUNT - 1);

			hash = hashToCopy;

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmptyHash)
		{
			Hash hashToMove(1);
			Hash hash(1);

			hash = std::move(hashToMove);

			Assert::IsTrue(hashToMove.isEmpty(), L"The empty hash should remain empty!");
			Assert::IsTrue(hash.isEmpty(), L"The moved-into hash should be empty!");
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmptyHash)
		{
			Hash emptyHash(1);
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT / 2);

			hash = std::move(emptyHash);

			Assert::IsTrue(emptyHash.isEmpty(), L"The empty hash should remain empty!");
			Assert::IsTrue(hash.isEmpty(), L"The non-empty hash should now be empty!");
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmptyHash)
		{
			Hash hashToMove(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToMove, 0, BOOKS_COUNT / 2);
			Hash hash(1);

			hash = std::move(hashToMove);

			Assert::IsTrue(hashToMove.isEmpty(), L"The moved-from hash should be empty!");
			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmptyHash)
		{
			Hash hashToMove(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hashToMove, 0, BOOKS_COUNT / 2);
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, BOOKS_COUNT / 2 + 1, BOOKS_COUNT - 1);

			hash = std::move(hashToMove);

			Assert::IsTrue(hashToMove.isEmpty(), L"The moved-from hash should be empty!");
			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testSearchInEmptyHashFindsNothing)
		{
			Hash hash(10);

			for (size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				Assert::IsNull(hash.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(testSearchFindsAddedItems)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				Assert::IsTrue(&books[i] == hash.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(testSearchDoesNotFindNotAddedItems)
		{
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT / 2);

			for (size_t i = BOOKS_COUNT / 2 + 1; i < BOOKS_COUNT; ++i)
			{
				Assert::IsNull(hash.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(testAddDealsWithPotentialFillUp)
		{
			Hash hash(1);

			for (size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				hash.add(books[i]);
				Assert::AreEqual(i + 1, hash.getCount());
			}

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT - 1));
		}

		TEST_METHOD(testRemoveLeavesNotRemovedItemsInTheHash)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (size_t i = BOOKS_COUNT / 2 + 1; i < BOOKS_COUNT; ++i)
			{
				hash.remove(books[i].getTitle());
			}

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testSearchForRemovedItemsIsUnsuccessfull)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (size_t i = BOOKS_COUNT / 2; i < BOOKS_COUNT; ++i)
			{
				hash.remove(books[i].getTitle());
				Assert::IsNull(hash.search(books[i].getTitle()));
			}
		}

		TEST_METHOD(testRemoveDecrementsItemsCount)
		{
			Hash hash(BOOKS_COUNT);

			size_t indexOfMiddleBook = BOOKS_COUNT / 2;
			fillHashWithBooksFromTo(hash, 0, indexOfMiddleBook);

			size_t correctCount = hash.getCount();

			for (size_t i = 0; i <= indexOfMiddleBook; ++i)
			{
				hash.remove(books[i].getTitle());
				Assert::AreEqual(--correctCount, hash.getCount());
			}
		}
	};

	Book HashTest::books[BOOKS_COUNT];
}