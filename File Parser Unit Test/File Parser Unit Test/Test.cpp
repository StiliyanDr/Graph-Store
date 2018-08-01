#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../Graph Store/Graph Store/FileParser.h"
#include <assert.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileParserUnitTest
{		
	TEST_CLASS(FileParserTest)
	{
	private:
		static const String testFileName;
		static const String nonExistentFileName;

	private:
		static void writeToTextFile(const char* text)
		{
			std::ofstream testFile(testFileName, std::ios::out | std::ios::trunc);
			assert(testFile.is_open());

			testFile << text;

			testFile.close();
		}

		static bool areEqualStrings(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

	public:
		TEST_METHOD(testDefaultCtorOpensNoFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedAFile());
		}

		TEST_METHOD(testCtorWithExistingFileOpensIt)
		{
			FileParser parser(testFileName);

			Assert::IsTrue(parser.hasOpenedAFile());
		}

		TEST_METHOD(testCtorWithNonExistentFileThrowsException)
		{
			try
			{
				FileParser parser(nonExistentFileName);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (std::exception& ex)
			{
				Assert::IsTrue(areEqualStrings("Could not open file for reading, name: " + nonExistentFileName,
											   ex.what()));
			}
		}

		TEST_METHOD(testOpenFileWithExistingFile)
		{
			FileParser parser;

			parser.openFile(testFileName);

			Assert::IsTrue(parser.hasOpenedAFile());
		}

		TEST_METHOD(testOpenFileThrowsExceptionIfItFails)
		{
			FileParser parser;

			try
			{
				parser.openFile(nonExistentFileName);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::exception& ex)
			{
				Assert::IsTrue(areEqualStrings("Could not open file for reading, name: " + nonExistentFileName,
											   ex.what()));
			}
		}

	};

	const String FileParserTest::testFileName = "testFile.txt";
	const String FileParserTest::nonExistentFileName = "noSuchFile";
}