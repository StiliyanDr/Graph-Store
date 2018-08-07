#pragma once
#include "../../../Graph Store/Graph Store/String/String.h"

struct File
{
	File(const char* name) :
		name(name), isEncountered(false) { }

	const String name;
	bool isEncountered;
};
