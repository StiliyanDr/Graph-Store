#include "stdafx.h"
#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/File Parser/FileParser.h"
#include "../../../Graph Store/Graph Store/File Parser/File Parser Exception/FileParserException.h"
#include "../../../Graph Store/Graph Store/File Parser/Open File Fail Exception/OpenFileFailException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileParserUnitTest
{		
	TEST_CLASS(FileParserTest)
	{
	private:
		static const String firstTestFileName;
		static const String secondTestFileName;
		static const String nonExistentFileName;

	private:
		static void emptyFile(const String& fileName)
		{
			writeTextToFile("", fileName);
		}

		static void writeTextToFile(const char* text, const String& fileName)
		{
			std::ofstream file(fileName, std::ios::out | std::ios::trunc);
			assert(file.is_open());

			file << text;

			file.close();
		}

		static void reachEndOfEmptyFile(FileParser& parser)
		{
			assert(parser.hasOpenedFile());
			assert(!parser.hasReachedEnd());

			parser.peek();

			assert(parser.hasReachedEnd());
		}

		static FileParser parserAfterAFailedOperation()
		{
			writeTextToFile("-123", firstTestFileName);

			FileParser parser(firstTestFileName);

			try
			{
				parser.parseUnsigned();
			}
			catch (FileParserException&)
			{
			}

			return parser;
		}

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

	public:
		TEST_METHOD(testDefaultCtorOpensNoFile)
		{
			FileParser parser;

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(testMoveCtorWithOpenedFile)
		{
			writeTextToFile("@", firstTestFileName);
			FileParser parserToMove(firstTestFileName);

			FileParser parser(std::move(parserToMove));

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not still be associated with the file!");
			Assert::IsTrue(parser.hasOpenedFile());
			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testMoveCtorWithNoOpenedFile)
		{
			FileParser parserToMove;

			FileParser parser(std::move(parserToMove));

			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(parser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentNotOpenedToNotOpened)
		{
			FileParser parserToMove;
			FileParser parser;

			parser = std::move(parserToMove);
			
			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(parser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentNotOpenedToOpened)
		{
			FileParser parserToMove;
			FileParser parser(firstTestFileName);

			parser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(parser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentOpenedToNotOpened)
		{
			writeTextToFile("@", firstTestFileName);
			FileParser parserToMove(firstTestFileName);
			FileParser parser;

			parser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not be associated with a file!");
			Assert::IsTrue(parser.hasOpenedFile());
			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testMoveAssignmentOpenedToOpened)
		{
			emptyFile(firstTestFileName);
			writeTextToFile("@", secondTestFileName);
			FileParser parserToMove(secondTestFileName);
			FileParser parser(firstTestFileName);

			parser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not be associated with a file!");
			Assert::IsTrue(parser.hasOpenedFile());
			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testCtorWithExistingFileOpensIt)
		{
			FileParser parser(firstTestFileName);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testCtorWithNonExistentFileThrowsException)
		{
			try
			{
				FileParser parser(nonExistentFileName);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (OpenFileFailException& ex)
			{
				Assert::IsTrue(areEqual("Could not open file for reading, name: " + nonExistentFileName,
										ex.what()));
			}
		}

		TEST_METHOD(testOpenFileWithExistingFile)
		{
			FileParser parser;

			parser.openFile(firstTestFileName);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testOpenFileThrowsExceptionIfItFails)
		{
			FileParser parser;

			try
			{
				parser.openFile(nonExistentFileName);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (OpenFileFailException& ex)
			{
				Assert::IsTrue(areEqual("Could not open file for reading, name: " + nonExistentFileName,
										ex.what()));
			}
		}

		TEST_METHOD(testOpenFileWhenAFileIsAlreadyOpened)
		{
			emptyFile(firstTestFileName);
			FileParser parser(firstTestFileName);
			writeTextToFile("@", secondTestFileName);

			parser.openFile(secondTestFileName);

			Assert::IsTrue(parser.hasOpenedFile());
			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testCloseFile)
		{
			FileParser parser(firstTestFileName);

			parser.closeFile();

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(testReadLineReturnsACopyOfTheRestOfTheCurrentLine)
		{
			String lineInFile = "A line in the file";
			writeTextToFile(lineInFile, firstTestFileName);
			FileParser parser(firstTestFileName);

			String extractedLine = parser.readLine();

			Assert::IsTrue(areEqual(lineInFile, extractedLine));
		}

		TEST_METHOD(testReadLineWhenNoCharactersAreLeftInFile)
		{
			emptyFile(firstTestFileName);
			FileParser parser(firstTestFileName);

			try
			{
				parser.readLine();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& ex)
			{
				Assert::IsTrue(areEqual("No more characters left in the file! Error at line 1",
										ex.what()));
			}
		}

		TEST_METHOD(testReadLineWhenNoCharactersAreLeftInTheLine)
		{
			writeTextToFile("\n", firstTestFileName);
			FileParser parser(firstTestFileName);

			String extractedLine = parser.readLine();

			Assert::IsTrue(areEqual("", extractedLine));
		}

		TEST_METHOD(testParseUnsignedSkipsSpacesAndExtractsNumber)
		{
			writeTextToFile("   8", firstTestFileName);
			FileParser parser(firstTestFileName);

			unsigned number = parser.parseUnsigned();

			Assert::AreEqual(8u, number);
		}

		TEST_METHOD(testParseUnsignedWithInvalidNumber)
		{
			writeTextToFile("not a number", firstTestFileName);
			FileParser parser(firstTestFileName);

			try
			{
				parser.parseUnsigned();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& ex)
			{
				Assert::IsTrue(areEqual("Invalid number format! Error at line 1",
										ex.what()));
			}
		}

		TEST_METHOD(testParseUnsignedWithNegativeNumber)
		{
			writeTextToFile("-128", firstTestFileName);
			FileParser parser(firstTestFileName);

			try
			{
				parser.parseUnsigned();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("Number must not be negative! Error at line 1",
										e.what()));
			}
		}

		TEST_METHOD(testParseUnsignedWhenNoCharactersAreLeftInFile)
		{
			emptyFile(firstTestFileName);
			FileParser parser(firstTestFileName);

			try
			{
				parser.parseUnsigned();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& ex)
			{
				Assert::IsTrue(areEqual("End of file already reached!",
										ex.what()));
			}
		}

		TEST_METHOD(testSkipUntilStopsAfterThePassedSymbol)
		{
			writeTextToFile("skip *@", firstTestFileName);
			FileParser parser(firstTestFileName);

			parser.skipUntil('*');

			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testSkipUntilWithNoMatchingSymbolSkipsTheWholeFile)
		{
			writeTextToFile("the symbol is not here", firstTestFileName);
			FileParser parser(firstTestFileName);

			parser.skipUntil('@');

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testSkipSpaces)
		{
			writeTextToFile("   @", firstTestFileName);
			FileParser parser(firstTestFileName);

			parser.skipSpaces();

			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testSkipSpacesStopsAtEndOfFile)
		{
			writeTextToFile("   ", firstTestFileName);
			FileParser parser(firstTestFileName);

			parser.skipSpaces();

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testFileOperationsWhenNoFileIsOpenedThrowException)
		{
			FileParser parser;

			try
			{
				parser.readLine();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("No file is currently opened!",
										e.what()));
			}
		}

		TEST_METHOD(testFileOperationsWhenEndOfFileIsReachedThrowException)
		{
			emptyFile(firstTestFileName);
			FileParser parser(firstTestFileName);
			reachEndOfEmptyFile(parser);

			try
			{
				parser.readLine();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("End of file already reached!",
										e.what()));
			}
		}

		TEST_METHOD(testFileOperationsAfterAFailedOperationThrowException)
		{
			FileParser parser = parserAfterAFailedOperation();

			try
			{
				parser.readLine();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("A previous operation already failed!",
										e.what()));
			}
		}

	};

	const String FileParserTest::firstTestFileName = "testFileOne.txt";
	const String FileParserTest::secondTestFileName = "testFileTwo.txt";
	const String FileParserTest::nonExistentFileName = "noSuchFile";
}