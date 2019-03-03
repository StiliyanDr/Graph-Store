#include "CppUnitTest.h"
#include "Directory Files Iterator\DirectoryIterator.h"
#include "Directory Files Iterator\Directory Files Iterator Exception\DirectoryIteratorException.h"
#include "File.h"
#include "ExtensionFilter.h"
#include "Iterator\Iterator.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DirectoryIteratorUnitTest
{		
	TEST_CLASS(DirectoryIteratorTest)
	{
		using Collection = std::vector<File>;

	public:
		TEST_METHOD(testDefaultCtorCreatesInvalidIterator)
		{
			DirectoryIterator iterator;

			Assert::IsFalse(iterator);
			Assert::IsTrue(!iterator);
		}

		TEST_METHOD(testIteratorForEmptyDirectoryIsInvalid)
		{
			DirectoryIterator iterator(EMPTY_TEST_DIRECTORY_NAME);

			Assert::IsFalse(iterator);
			Assert::IsTrue(!iterator);
		}

		TEST_METHOD(testIteratorForDirectoryWithNoPassingFilesIsInvalid)
		{
			auto alwaysFalseFilter = [](const String&) { return false; };
			DirectoryIterator iterator(TEST_DIRECTORY_NAME, alwaysFalseFilter);

			Assert::IsFalse(iterator);
			Assert::IsTrue(!iterator);
		}

		TEST_METHOD(testIteratorForDirectoryWithAPassingFileIsValid)
		{
			DirectoryIterator iterator(TEST_DIRECTORY_NAME,
				                       ExtensionFilter(EXTENSION));

			Assert::IsTrue(iterator);
			Assert::IsFalse(!iterator);
		}

		TEST_METHOD(testCtorThrowsExceptionIfItFailsToOpenDirectory)
		{
			try
			{
				DirectoryIterator iterator(NONEXISTENT_DIRECTORY_NAME);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (DirectoryIteratorException&)
			{
			}
		}

		TEST_METHOD(testIncrementingInvalidIteratorDoesNothing)
		{
			DirectoryIterator iterator;

			++iterator;

			Assert::IsFalse(iterator);
			Assert::IsTrue(!iterator);
		}

		TEST_METHOD(dereferencingInvalidIteratorThrowsException)
		{
			DirectoryIterator invalidIterator;

			try
			{
				*invalidIterator;
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::out_of_range&)
			{
			}
		}

		TEST_METHOD(testDirectoryIteration)
		{
			markPassingFilesAsUnencountered();
			DirectoryIterator iterator(TEST_DIRECTORY_NAME,
				                       ExtensionFilter(EXTENSION));

			Assert::IsTrue(encountersOnlyPassingFilesExactlyOnce(iterator));
		}
	
	private:
		static bool encountersOnlyPassingFilesExactlyOnce(DirectoryIterator& iterator)
		{
			return allOf(iterator, isPathOfPassingUnencounteredFile)
				   && encounteredAllPassingFiles();
		}

		static bool isPathOfPassingUnencounteredFile(const String& path)
		{
			Collection::iterator end = passingFiles.end();
			Collection::iterator iterator =
				find_if(passingFiles.begin(), end, [&](const File& file)
			{
				return !file.isEncountered && file.path == path;
			});

			bool foundUnencounteredPassingFile = iterator != end;

			if (foundUnencounteredPassingFile)
			{
				iterator->isEncountered = true;
			}

			return foundUnencounteredPassingFile;
		}

		static bool encounteredAllPassingFiles()
		{
			return std::all_of(passingFiles.cbegin(),
				               passingFiles.cend(),
				               [](const File& file)
			{
				return file.isEncountered;
			});
		}

		static void markPassingFilesAsUnencountered()
		{
			std::for_each(passingFiles.begin(),
				          passingFiles.end(),
				          [](File& file)
			{
				file.isEncountered = false;
			});
		}

	private:
			static const String EMPTY_TEST_DIRECTORY_NAME;
			static const String TEST_DIRECTORY_NAME;
			static const String NONEXISTENT_DIRECTORY_NAME;
			static const String EXTENSION;
			static Collection passingFiles;
	};

	const String DirectoryIteratorTest::EMPTY_TEST_DIRECTORY_NAME = "Empty test directory";
	const String DirectoryIteratorTest::TEST_DIRECTORY_NAME = "Test directory";
	const String DirectoryIteratorTest::NONEXISTENT_DIRECTORY_NAME = "not a directory";
	const String DirectoryIteratorTest::EXTENSION = ".txt";
	DirectoryIteratorTest::Collection DirectoryIteratorTest::passingFiles =
		{ "Test directory\\testFileOne.txt",
	      "Test directory\\testFileTwo.txt",
	      "Test directory\\testFileThree.txt" };
}