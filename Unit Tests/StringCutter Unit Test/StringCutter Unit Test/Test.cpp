#include "CppUnitTest.h"
#include "../../../Graph Store/Graph Store/String Cutter/StringCutter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringCutterUnitTest
{		
	TEST_CLASS(StringCutterTest)
	{
		using StringList = std::vector<std::string>;

		static bool areEqual(const char* lhs, const char* rhs)
		{
			return strcmp(lhs, rhs) == 0;
		}

		static bool wordsMatch(const char* expected[],
			                   const StringList& actual,
			                   size_t expectedSize)
		{
			if (actual.size() != expectedSize)
			{
				return false;
			}

			for (size_t i = 0; i < expectedSize; ++i)
			{
				if (!areEqual(expected[i], actual[i].c_str()))
				{
					return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(testDefaultCtorUsesIntervalsAsDelimiters)
		{
			StringCutter cutter;
			char sentence[] = " \'two\' \"words\"";

			StringList words = cutter.cutToWords(sentence);

			size_t expectedSize = 2;
			const char* expected[] = { "\'two\'", "\"words\"" };
			Assert::IsTrue(wordsMatch(expected, words, expectedSize));
		}

		TEST_METHOD(testCtorWithDelimiterOptions)
		{
			StringCutter cutter({ '\'', '\"' });
			char sentence[] = "\'first word\' \"second word\"";

			StringList words = cutter.cutToWords(sentence);

			size_t expectedSize = 2;
			const char* expected[] = { "first word", "second word" };
			Assert::IsTrue(wordsMatch(expected, words, expectedSize));
		}

		TEST_METHOD(testCtorWithDelimiterOptionsUsesIntervalAsDelimiter)
		{
			StringCutter cutter({ '\'' });
			char sentence[] = " first \'long word\'";

			StringList words = cutter.cutToWords(sentence);

			size_t expectedSize = 2;
			const char* expected[] = { "first", "long word" };
			Assert::IsTrue(wordsMatch(expected, words, expectedSize));
		}

		TEST_METHOD(testCutToWordsThrowsExceptionIfWordDelimitersDontMatch)
		{
			StringCutter cutter({ '\"', '\'' });
			char sentence[] = "\"different word delimiters\'";

			try
			{
				cutter.cutToWords(sentence);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("Word delimiters don't match!",
					                    e.what()));
			}
		}

		TEST_METHOD(testCutToWordsThrowsExceptionIfClosingDelimiterIsMissing)
		{
			StringCutter cutter({ '\"' });
			char sentence[] = "\"missing delimiter";

			try
			{
				cutter.cutToWords(sentence);
				Assert::Fail(L"The method did not throw an exception!");
			}
			catch (std::logic_error& e)
			{
				Assert::IsTrue(areEqual("Word delimiters don't match!",
					                    e.what()));
			}
		}

		TEST_METHOD(testCutToWordsReturnsEmptyStringIfPassedAStringOfIntervals)
		{
			StringCutter cutter;
			char intervals[] = "   ";

			StringList words = cutter.cutToWords(intervals);

			size_t expectedSize = 1;
			const char* expected[] = { "" };
			Assert::IsTrue(wordsMatch(expected, words, expectedSize));
		}

		TEST_METHOD(testCutToWordsIgnoresSpacesAfterTheLastWord)
		{
			StringCutter cutter;
			char sentence[] = "first second    ";

			StringList words = cutter.cutToWords(sentence);

			size_t expectedSize = 2;
			const char* expected[] = { "first", "second" };
			Assert::IsTrue(wordsMatch(expected, words, expectedSize));
		}
	};
}