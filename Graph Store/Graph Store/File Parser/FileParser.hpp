
template <class Number>
Number FileParser::parseNumber()
{
	assertValidState();

	Number number;
	file >> number;

	throwExceptionIfInErrorState("Invalid number format!");

	return number;
}
