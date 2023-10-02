#include "Utils.h"

int Debug::nDebugLine;
char Debug::whiteSpaces[120] = { '\0' };

void Debug::Log(const char* fmt, ...)
{

	char dest[1024];
	static clock_t startTime = clock();

	if (whiteSpaces[0] == '\0') { // if not initialized yet.
		memset(whiteSpaces, ' ', 120);
		whiteSpaces[119] = '\0';
	}

	va_list argPtr;

	va_start(argPtr, fmt);

	Borland::GotoXY(80, nDebugLine++ % 20);
	vsprintf(dest, fmt, argPtr);
	va_end(argPtr);
	printf("[%5d: %.3f] %s", nDebugLine, (clock() - startTime) / 1000.0f, dest);
	Borland::GotoXY(80, nDebugLine + 1 % 20);
	printf("%s", whiteSpaces);

}

//Vector2 Vector2::operator+(Dimension& other)
//{
//	return Vector2{ x + (float)other.x,y + (float)other.y };
//}
