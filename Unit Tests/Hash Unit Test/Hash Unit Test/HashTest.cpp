#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Hash/Hash.h"
#include "Book.h"
#include "BookTitleAccessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HashUnitTest
{		
	TEST_CLASS(HashTest)
	{
		typedef Hash<Book, String, BookTitleAccessor> Hash;

		static const std::size_t BOOKS_COUNT = 16;
		static Book books[BOOKS_COUNT];

		static void fillHashWithBooksFromTo(Hash& hash,
			                                std::size_t from,
			                                std::size_t to)
		{
			assert(isValidIndex(from));
			assert(isValidIndex(to));

			for (std::size_t i = from; i <= to; ++i)
			{
				hash.add(books[i]);
			}
		}

		static bool isValidIndex(std::size_t i)
		{
			return i < BOOKS_COUNT;
		}

		static bool hashConsistsOfBooksFromTo(const Hash& hash,
			                                  std::size_t from,
			                                  std::size_t to)
		{
			assert(from <= to);
			
			std::size_t booksCount = to - from + 1;

			return booksCount == hash.getSize()
				   && hashContainsBooksFromTo(hash, from, to);
		}

		static bool hashContainsBooksFromTo(const Hash& hash,
			                                std::size_t from,
			                                std::size_t to)
		{
			assert(isValidIndex(from));
			assert(isValidIndex(to));

			for (std::size_t i = from; i <= to; ++i)
			{
				if (!hash.contains(books[i].getTitle()))
				{
					return false;
				}
			}

			return true;
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

	public:
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

		TEST_METHOD(testEmptyHashContainsNothing)
		{
			Hash hash(10);

			for (std::size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				Assert::IsFalse(hash.contains(books[i].getTitle()));
			}
		}

		TEST_METHOD(testHashContainsAddedItems)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (std::size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				Assert::IsTrue(hash.contains(books[i].getTitle()));
			}
		}

		TEST_METHOD(testHashDoesNotContainNotAddedItems)
		{
			Hash hash(BOOKS_COUNT / 2);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT / 2);

			for (std::size_t i = BOOKS_COUNT / 2 + 1; i < BOOKS_COUNT; ++i)
			{
				Assert::IsFalse(hash.contains(books[i].getTitle()));
			}
		}

		TEST_METHOD(testAddDealsWithPotentialFillUp)
		{
			Hash hash(1);

			for (std::size_t i = 0; i < BOOKS_COUNT; ++i)
			{
				hash.add(books[i]);
				Assert::AreEqual(i + 1, hash.getSize());
			}

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT - 1));
		}

		TEST_METHOD(testRemoveLeavesNotRemovedItemsInTheHash)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (std::size_t i = BOOKS_COUNT / 2 + 1; i < BOOKS_COUNT; ++i)
			{
				hash.remove(books[i].getTitle());
			}

			Assert::IsTrue(hashConsistsOfBooksFromTo(hash, 0, BOOKS_COUNT / 2));
		}

		TEST_METHOD(testHashNoLongerContainsRemovedItems)
		{
			Hash hash(BOOKS_COUNT);
			fillHashWithBooksFromTo(hash, 0, BOOKS_COUNT - 1);

			for (std::size_t i = BOOKS_COUNT / 2; i < BOOKS_COUNT; ++i)
			{
				hash.remove(books[i].getTitle());
				Assert::IsFalse(hash.contains(books[i].getTitle()));
			}
		}

		TEST_METHOD(testRemoveDecrementsItemsCount)
		{
			Hash hash(BOOKS_COUNT);

			std::size_t indexOfMiddleBook = BOOKS_COUNT / 2;
			fillHashWithBooksFromTo(hash, 0, indexOfMiddleBook);

			std::size_t correctCount = hash.getSize();

			for (std::size_t i = 0; i <= indexOfMiddleBook; ++i)
			{
				hash.remove(books[i].getTitle());
				Assert::AreEqual(--correctCount, hash.getSize());
			}
		}

		TEST_METHOD(testOperatorSubscriptWithContainedKey)
		{
			Hash hash(1);
			Book& insertedBook = books[0];
			hash.add(insertedBook);

			Book& book = hash[insertedBook.getTitle()];

			Assert::IsTrue(&book == &insertedBook);
		}

		TEST_METHOD(testOperatorSubscriptWithNotContainedKeyThrowsException)
		{
			Hash hash;

			try
			{
				hash["key"];
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range& e)
			{
				Assert::IsTrue(areEqual("There is no item with such key!", e.what()));
			}
		}

	};

	Book HashTest::books[BOOKS_COUNT];
}