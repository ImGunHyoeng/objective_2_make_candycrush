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
	void reTouch(const char* shape);
	void change(int type);

	NumberCard& operator=(const NumberCard& other)
	{
		if (this == &other)
			return *this;
		GameObject::~GameObject();
		
		 
		setShape(other.getShape());
		setPosition(other.getPosition());
		setVisible(other.isVisible());
		type = other.type;
		return *this;
	}
};
enum class CardType
{
	One,
	Two,
	Three,
	Four,
	Five,
	Six,
	Seven,
	Eight,
	Nine
};

