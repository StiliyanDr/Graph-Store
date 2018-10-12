#include "CppUnitTest.h"
#include <utility>
#include <cstring>
#include "../../../Graph Store/Graph Store/String/String.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringUnitTest
{		
	TEST_CLASS(StringTest)
	{
		static bool areEqual(const char* expected, const String& actual)
		{
			return areEqual(expected, actual.cString());
		}

		static bool areEqual(const char* expected, const char* actual)
		{
			return strcmp(expected, actual) == 0;
		}

	public:
		TEST_METHOD(testConstructorFromSymbol)
		{
			String string('a');

			Assert::IsTrue(areEqual("a", string));
		}

		TEST_METHOD(testDefaultConstructorCreatesAnEmptyString)
		{
			String string;

			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testConstructorFromStringCopiesIt)
		{
			char word[] = "word";
			String string(word);

			Assert::IsTrue(areEqual(word, string));
		}

		TEST_METHOD(testConstructorFromNullptrCreatesAnEmptyString)
		{
			String string(nullptr);

			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testMoveConstructorFromEmptyString)
		{
			String emptyString = "";
			String string(std::move(emptyString));

			Assert::IsTrue(areEqual("", string));
			Assert::IsTrue(areEqual("", emptyString));
		}

		TEST_METHOD(testMoveConstructorFromNonEmptyString)
		{
			char word[] = "word";
			String stringToMove = word;
			String string(std::move(stringToMove));

			Assert::IsTrue(areEqual(word, string));
			Assert::IsTrue(areEqual("", stringToMove));
		}

		TEST_METHOD(testCopyConstructorFromEmptyString)
		{
			const String emptyString = "";
			String copy(emptyString);

			Assert::IsTrue(copy == emptyString);
		}

		TEST_METHOD(testCopyConstructorFromNonEmptyString)
		{
			const String word = "word";
			String copy(word);

			Assert::IsTrue(word == copy);
		}

		TEST_METHOD(testCopyAssignmentEmptyToEmptyString)
		{
			const String emptyString = "";
			String string = "";

			string = emptyString;

			Assert::IsTrue(emptyString == string);
		}

		TEST_METHOD(testCopyAssignmentEmptyToNonEmptyString)
		{
			const String emptyString = "";
			String string = "word";

			string = emptyString;

			Assert::IsTrue(string == emptyString);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToEmptyString)
		{
			const String word = "word";
			String string = "";

			string = word;

			Assert::IsTrue(string == word);
		}

		TEST_METHOD(testCopyAssignmentNonEmptyToNonEmptyString)
		{
			const String word = "word";
			String string = "string";

			string = word;

			Assert::IsTrue(string == word);
		}

		TEST_METHOD(testCopyAssignmentWithNullptrAssignsEmptyString)
		{
			String string = "string";
			string = nullptr;

			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testMoveAssignmentEmptyToEmpty)
		{
			String stringToMove = "";
			String string = "";

			string = std::move(stringToMove);

			Assert::IsTrue(areEqual("", stringToMove));
			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testMoveAssignmentEmptyToNonEmpty)
		{
			String emptyString = "";
			String string = "word";

			string = std::move(emptyString);

			Assert::IsTrue(areEqual("", emptyString));
			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToEmpty)
		{
			char word[] = "word";
			String stringToMove = word;
			String string = "";

			string = std::move(stringToMove);

			Assert::IsTrue(areEqual("", stringToMove));
			Assert::IsTrue(areEqual(word, string));
		}

		TEST_METHOD(testMoveAssignmentNonEmptyToNonEmpty)
		{
			char word[] = "word";
			String stringToMove = word;
			String string = "string";

			string = std::move(stringToMove);

			Assert::IsTrue(areEqual("", stringToMove));
			Assert::IsTrue(areEqual(word, string));
		}

		TEST_METHOD(testGetLengthForEmptyString)
		{
			const String emptyString = "";

			Assert::AreEqual(0u, emptyString.getLength());
		}

		TEST_METHOD(testGetLengthForNonEmptyString)
		{
			const String string = "12345";

			Assert::AreEqual(5u, string.getLength());
		}

		TEST_METHOD(testConcatenateEmptyToEmptyString)
		{
			String string = "";

			string += "";

			Assert::IsTrue(areEqual("", string));
		}

		TEST_METHOD(testConcatenateEmptyToNotEmptyString)
		{
			char word[] = "word";
			String string = word;

			string += "";

			Assert::IsTrue(areEqual(word, string));
		}

		TEST_METHOD(testConcatenateNonEmptyToEmptyString)
		{
			char word[] = "word";
			String string = "";

			string += word;

			Assert::IsTrue(areEqual(word, string));
		}

		TEST_METHOD(testConcatenateNonEmptyToNonEmptyString)
		{
			String prefix = "Hello";
			String suffix = " world!";

			prefix += suffix;

			Assert::IsTrue(areEqual("Hello world!", prefix));
		}

	};
}