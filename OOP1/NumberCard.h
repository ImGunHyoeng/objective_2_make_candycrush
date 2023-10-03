#pragma once
#include "GameObject.h"
#include "ICollidable.h"
class NumberCard : public GameObject
{
private:
	int type;

public:
	NumberCard(const char* shape, const Vector2& pos, const Dimension& dim, bool visible,int type) : GameObject(shape, pos, dim, visible),type(type)
	{}
	int getType()const { return type; }
	void setType(int _type) { type = _type; }
	virtual void update(InputManager& input) override;
	void reTouch(char* shape);
	void change(NumberCard& target);

	NumberCard& operator=(const NumberCard& other)
	{
		setShape(other.getShape());
		setPosition(other.getPosition());
		setVisible(other.isVisible());
		type = other.type;
		
	}
};

