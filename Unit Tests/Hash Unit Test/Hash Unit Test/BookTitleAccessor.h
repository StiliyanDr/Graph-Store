#pragma once

#include "Book.h"

class BookTitleAccessor
{
public:
	const String& operator()(const Book& book) const
	{
		return book.getTitle();
	}
};
