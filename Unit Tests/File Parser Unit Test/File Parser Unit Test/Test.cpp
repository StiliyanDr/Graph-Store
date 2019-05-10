#include "stdafx.h"
#include "CppUnitTest.h"
#include "File Parser/FileParser.h"
#include "File Parser/File Parser Exception/FileParserException.h"
#include "File Parser/Open File Fail Exception/OpenFileFailException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FileParserUnitTest
{
	TEST_CLASS(FileParserTest)
	{
	private:
		static const String FIRST_TEST_FILE_NAME;
		static const String SECOND_TEST_FILE_NAME;
		static const String NON_EXISTENT_FILE_NAME;

	private:
		static void emptyFile(const String& fileName)
		{
			writeTextToFile("", fileName);
		}

		static void writeTextToFile(const String& text,
			                        const String& fileName)
		{
			writeTextToFile(text.cString(), fileName);
		}

		static void writeTextToFile(const char* text,
			                        const String& fileName)
		{
			std::ofstream file(fileName.cString(),
				               std::ios::out | std::ios::trunc);
			assert(file.is_open());

			file << text;
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
			writeTextToFile("-123", FIRST_TEST_FILE_NAME);

			FileParser parser(FIRST_TEST_FILE_NAME);

			try
			{
				parser.parseUnsigned();
			}
			catch (FileParserException&)
			{
			}

			return parser;
		}

		static bool areEqual(const String& lhs, const String& rhs)
		{
			return areEqual(lhs.cString(), rhs.cString());
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
			writeTextToFile("@", FIRST_TEST_FILE_NAME);
			FileParser parserToMove(FIRST_TEST_FILE_NAME);

			FileParser movedIntoParser(std::move(parserToMove));

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not still be associated with the file!");
			Assert::IsTrue(movedIntoParser.hasOpenedFile());
			Assert::AreEqual('@', movedIntoParser.peek());
		}

		TEST_METHOD(testMoveCtorWithNoOpenedFile)
		{
			FileParser parserToMove;

			FileParser movedIntoParser(std::move(parserToMove));

			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(movedIntoParser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentNotOpenedToNotOpened)
		{
			FileParser parserToMove;
			FileParser movedIntoParser;

			movedIntoParser = std::move(parserToMove);
			
			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(movedIntoParser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentNotOpenedToOpened)
		{
			FileParser parserToMove;
			FileParser parser(FIRST_TEST_FILE_NAME);

			parser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile());
			Assert::IsFalse(parser.hasOpenedFile(),
							L"The moved-into parser should not be associated with a file!");
		}

		TEST_METHOD(testMoveAssignmentOpenedToNotOpened)
		{
			writeTextToFile("@", FIRST_TEST_FILE_NAME);
			FileParser parserToMove(FIRST_TEST_FILE_NAME);
			FileParser movedIntoParser;

			movedIntoParser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not be associated with a file!");
			Assert::IsTrue(movedIntoParser.hasOpenedFile());
			Assert::AreEqual('@', movedIntoParser.peek());
		}

		TEST_METHOD(testMoveAssignmentOpenedToOpened)
		{
			emptyFile(FIRST_TEST_FILE_NAME);
			writeTextToFile("@", SECOND_TEST_FILE_NAME);
			FileParser parserToMove(SECOND_TEST_FILE_NAME);
			FileParser movedIntoParser(FIRST_TEST_FILE_NAME);

			movedIntoParser = std::move(parserToMove);

			Assert::IsFalse(parserToMove.hasOpenedFile(),
							L"The moved-from parser should not be associated with a file!");
			Assert::IsTrue(movedIntoParser.hasOpenedFile());
			Assert::AreEqual('@', movedIntoParser.peek());
		}

		TEST_METHOD(testCtorWithExistingFileOpensIt)
		{
			FileParser parser(FIRST_TEST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testCtorWithNonExistentFileThrowsException)
		{
			try
			{
				FileParser parser(NON_EXISTENT_FILE_NAME);
				Assert::Fail(L"Constructor did not throw an exception!");
			}
			catch (OpenFileFailException&)
			{
			}
		}

		TEST_METHOD(testOpenFileWithExistingFile)
		{
			FileParser parser;

			parser.openFile(FIRST_TEST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile());
		}

		TEST_METHOD(testOpenFileThrowsExceptionIfItFails)
		{
			FileParser parser;

			try
			{
				parser.openFile(NON_EXISTENT_FILE_NAME);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (OpenFileFailException&)
			{
			}
		}

		TEST_METHOD(testOpenFileWhenAFileIsAlreadyOpened)
		{
			emptyFile(FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);
			writeTextToFile("@", SECOND_TEST_FILE_NAME);

			parser.openFile(SECOND_TEST_FILE_NAME);

			Assert::IsTrue(parser.hasOpenedFile());
			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testCloseFile)
		{
			FileParser parser(FIRST_TEST_FILE_NAME);

			parser.closeFile();

			Assert::IsFalse(parser.hasOpenedFile());
		}

		TEST_METHOD(testReadLineReturnsACopyOfTheRestOfTheCurrentLine)
		{
			char lineInFile[] = "test";
			writeTextToFile(lineInFile, FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			String extractedLine = parser.readLine();

			Assert::IsTrue(areEqual(lineInFile, extractedLine));
		}

		TEST_METHOD(testReadLineWhenNoCharactersAreLeftInFile)
		{
			emptyFile(FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			try
			{
				parser.readLine();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("No more characters left in the file! Error at line 1",
										e.what()));
			}
		}

		TEST_METHOD(testReadLineWhenNoCharactersAreLeftInTheLine)
		{
			writeTextToFile("\n", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			String extractedLine = parser.readLine();

			Assert::IsTrue(areEqual("", extractedLine));
		}

		TEST_METHOD(testReadAndTrimLineReturnsATrimmedCopyOfTheRestOfTheLine)
		{
			String word = "test";
			String spaceSurroundedLine = " " + word + " ";
			writeTextToFile(spaceSurroundedLine, FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			String extractedLine = parser.readAndTrimLine();
			
			Assert::IsTrue(areEqual(word, extractedLine));
		}

		TEST_METHOD(testReadAndTrimATrimmedLine)
		{
			char trimmedLineInFile[] = "test";
			writeTextToFile(trimmedLineInFile, FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			String extractedLine = parser.readAndTrimLine();

			Assert::IsTrue(areEqual(trimmedLineInFile, extractedLine));
		}

		TEST_METHOD(testParseUnsignedSkipsSpacesAndExtractsNumber)
		{
			writeTextToFile("   8", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			unsigned number = parser.parseUnsigned();

			Assert::AreEqual(8u, number);
		}

		TEST_METHOD(testParseUnsignedWithInvalidNumber)
		{
			writeTextToFile("not a number", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			try
			{
				parser.parseUnsigned();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("Invalid number format! Error at line 1",
										e.what()));
			}
		}

		TEST_METHOD(testParseUnsignedWithNegativeNumber)
		{
			writeTextToFile("-128", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

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
			emptyFile(FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			try
			{
				parser.parseUnsigned();
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (FileParserException& e)
			{
				Assert::IsTrue(areEqual("End of file already reached!",
										e.what()));
			}
		}

		TEST_METHOD(testSkipUntilStopsAfterThePassedCharacter)
		{
			writeTextToFile("skip *@", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			parser.skipUntil('*');

			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testSkipUntilWithNoMatchingCharacterSkipsTheWholeFile)
		{
			writeTextToFile("the character is not here",
				            FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			parser.skipUntil('@');

			Assert::IsTrue(parser.hasReachedEnd());
		}

		TEST_METHOD(testSkipSpaces)
		{
			writeTextToFile("   @", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

			parser.skipSpaces();

			Assert::AreEqual('@', parser.peek());
		}

		TEST_METHOD(testSkipSpacesStopsAtEndOfFile)
		{
			writeTextToFile("   ", FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);

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
			emptyFile(FIRST_TEST_FILE_NAME);
			FileParser parser(FIRST_TEST_FILE_NAME);
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

	const String FileParserTest::FIRST_TEST_FILE_NAME = "testFileOne.txt";
	const String FileParserTest::SECOND_TEST_FILE_NAME = "testFileTwo.txt";
	const String FileParserTest::NON_EXISTENT_FILE_NAME = "noSuchFile";
}