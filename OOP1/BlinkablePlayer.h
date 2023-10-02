#pragma once
#include "Player.h"
class BlinkablePlayer :
    public Player
{
	int n_frames;

public:
	BlinkablePlayer(const char* shape, const Vector2& pos, const Dimension& dim, bool visible) : Player(shape, pos, dim, visible), n_frames(0)
	{}

	void draw() override {
		if (n_frames > 0) {
			n_frames--;
			if (n_frames % 2 == 0) return;
		}
		GameObject::draw();
	}

	void setBlinkingPeriod(int n_frames)
	{
		this->n_frames = n_frames;
	}

	void update(InputManager& input) override;

};

