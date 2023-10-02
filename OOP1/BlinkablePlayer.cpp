#include "BlinkablePlayer.h"
#include "InputManager.h"

void BlinkablePlayer::update(InputManager& input)
{
	static Vector2 move = { 0,0 };
	if (input.getKey(VK_LEFT))
	{
		move = Vector2{ -1,0 };
		auto pos = getPosition();
		setPosition(getPosition() + move);
	}
	if (input.getKey(VK_RIGHT))
	{
		move = Vector2{ 1,0 };
		auto pos = getPosition();
		setPosition(getPosition() + move);
	}
	if (input.getKey(VK_UP))
	{
		move = Vector2{ 0,-1 };
		auto pos = getPosition();
		setPosition(getPosition() + move);
	}
	if (input.getKey(VK_DOWN))
	{
		move = Vector2{ 0,1 };
		auto pos = getPosition();
		setPosition(getPosition() + move);
	}
}
