#pragma once

#include <stdio.h>

namespace Log
{
	static char const *  ansiColorRed = "\x1b[31m";
	static char const *  ansiColorGreen = "\x1b[32m";
	static char const *  ansiColorYellow = "\x1b[33m";
	static char const *  ansiColorReset = "\x1b[0m";

	void Error(char const* message)
	{
		printf("%sError: %s.\n", ansiColorRed, message);
		printf("%s", ansiColorReset);
	}
	void Warning(char const* message)
	{
		printf("%sWarning: %s.\n", ansiColorYellow, message);
		printf("%s", ansiColorReset);
	}
}