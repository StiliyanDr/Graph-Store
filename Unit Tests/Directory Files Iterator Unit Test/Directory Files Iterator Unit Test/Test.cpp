#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/Directory Files Iterator/DirectoryFilesIterator.h"
#include "../../../Graph Store/Graph Store/Directory Files Iterator/Directory Files Iterator Exception/DirectoryFilesIteratorException.h"
#include "File.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DirectoryFilesIteratorUnitTest
{		
	TEST_CLASS(DirectoryFilesIteratorTest)
	{
	private:
		static const String EMPTY_TEST_DIRECTORY_NAME;
		static const String TEST_DIRECTORY_NAME;
		static const String NONEXISTENT_DIRECTORY_NAME;
		static const size_t FILES_COUNT = 3;
		static File files[FILES_COUNT];

	private:
		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static bool encountersEachFileExactlyOnce(DirectoryFilesIterator& iterator)
		{
			while (iterator.isValid())
			{
				if (!isNameOfUnencounteredFile(iterator.getNameOfCurrentFile()))
				{
					return false;
				}

				iterator.advance();
			}

			return encounteredAllFiles();
		}

		static bool isNameOfUnencounteredFile(const String& fileName)
		{
			for (size_t i = 0; i < FILES_COUNT; ++i)
			{
				if (!files[i].isEncountered && files[i].name == fileName)
				{
					files[i].isEncountered = true;
					return true;
				}
			}

			return false;
		}

		static bool encounteredAllFiles()
		{
			for (size_t i = 0; i < FILES_COUNT; ++i)
			{
				if (!files[i].isEncountered)
				{
					return false;
				}
			}

			return true;
		}

		static void markAllFilesAsUnencountered()
		{
			for (size_t i = 0; i < FILES_COUNT; ++i)
			{
				files[i].isEncountered = false;
			}
		}

	public:
		TEST_METHOD(testDefaultCtorCreatesInvalidIterator)
		{
			DirectoryFilesIterator iterator;

			Assert::IsFalse(iterator.isValid());
		}

		TEST_METHOD(testCtorWithEmptyDirectoryCreatesInvalidIterator)
		{
			DirectoryFilesIterator iterator(EMPTY_TEST_DIRECTORY_NAME);

			Assert::IsFalse(iterator.isValid());
		}

		TEST_METHOD(testCtorWithNonEmptyDirectoryCreatesValidIterator)
		{
			DirectoryFilesIterator iterator(TEST_DIRECTORY_NAME);

			Assert::IsTrue(iterator.isValid());
		}

		TEST_METHOD(testCtorThrowsExceptionIfItFailsToOpenDirectory)
		{
			try
			{
				DirectoryFilesIterator iterator(NONEXISTENT_DIRECTORY_NAME);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (DirectoryFilesIteratorException& ex)
			{
				Assert::IsTrue(areEqual("Could not open directory: " + NONEXISTENT_DIRECTORY_NAME,
										ex.what()));
			}
		}

		TEST_METHOD(testDirectoryIteration)
		{
			markAllFilesAsUnencountered();
			DirectoryFilesIterator iterator(TEST_DIRECTORY_NAME);
			
			Assert::IsTrue(encountersEachFileExactlyOnce(iterator));
		}

		TEST_METHOD(testStartIterationInWithValidIterator)
		{
			DirectoryFilesIterator iterator(TEST_DIRECTORY_NAME);

			iterator.startIterationIn(EMPTY_TEST_DIRECTORY_NAME);

			Assert::IsFalse(iterator.isValid());
		}

	};

	const String DirectoryFilesIteratorTest::EMPTY_TEST_DIRECTORY_NAME = "Empty test directory";
	const String DirectoryFilesIteratorTest::TEST_DIRECTORY_NAME = "Test directory";
	const String DirectoryFilesIteratorTest::NONEXISTENT_DIRECTORY_NAME = "not a directory";
	File DirectoryFilesIteratorTest::files[FILES_COUNT] = { "testFileOne.txt",
															"testFileTwo.txt",
															"testFileThree.txt" };
}