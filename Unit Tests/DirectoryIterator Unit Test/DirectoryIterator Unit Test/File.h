#pragma once
#include "String/String.h"

struct File
{
	File() :
		isEncountered(false)
	{
	}

	File(const char* path) :
		path(path), isEncountered(false)
	{
	}

	String path;
	bool isEncountered;
};
