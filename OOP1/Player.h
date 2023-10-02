#pragma once
#include "GameObject.h"

class Player :
    public GameObject
{
public:
	Player(const char* shape, const Vector2& pos, const Dimension& dim, bool visible) : GameObject(shape, pos, dim, visible)
	{}

	~Player()
	{}
};

