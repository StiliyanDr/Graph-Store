#pragma once

#include "../../Graph Store/Graph Store/String.h"

class Book
{
public:
	Book(const char* title = "");

	void setTitle(const char* title);
	const String& getTitle() const;

private:
	String title;
};
