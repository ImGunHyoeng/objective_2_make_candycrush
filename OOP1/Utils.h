#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <Windows.h>
#include <string.h> // memset
#include <math.h>

struct Vector2 {
	float x;
	float y;

	Vector2(float x, float y) : x(x), y(y) {}

	float sqrMagnitude() const { return x * x + y * y; }
	float magnitude() const { return sqrt(sqrMagnitude()); }
	bool  equalApproximately(const Vector2& other) const { return distance(other) < 0.5f; }

	float distance(const Vector2& other) const { return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}

	Vector2 unit() const {
		if (equalApproximately({ 0.0f, 0.0f })) return { 1.0f, 0.0f };
		float mag = magnitude();
		return { this->x / mag, this->y / mag };
		
	}

	Vector2 operator+ (Vector2& other)
	{
		return Vector2{ x + other.x,y + other.y };
	}

	//Vector2 operator+ (Dimension& other);
	

	friend float sqrDistance(const Vector2& one, const Vector2& another) { return one.distance(another); }
};

struct Position {
	int x;
	int y;
	Position(int x, int y) : x(x), y(y) {}
};

typedef Position Dimension;


class Borland {

public:
	static void Initialize()
	{
		std::system("mode con:cols=200 lines=50");
		std::system("chcp 437");

		CONSOLE_CURSOR_INFO cci;
		cci.dwSize = 25;
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	}

	static int WhereX()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.X;
	}
	static int WhereY()
	{
		CONSOLE_SCREEN_BUFFER_INFO  csbiInfo;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiInfo);
		return csbiInfo.dwCursorPosition.Y;
	}
	static void GotoXY(int x, int y)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _COORD{ (SHORT)x, (SHORT)y });
	}
	static void GotoXY(const Position* pos)
	{
		if (!pos) return;
		GotoXY( (*pos).x, (*pos).y);
	}
	static void GotoXY(const Position& pos)
	{
		GotoXY( pos.x, pos.y);
	}
};

class Debug {
	static int nDebugLine;
	static char whiteSpaces[120];

public:
	static void Log(const char* fmt, ...);
};



#endif 
