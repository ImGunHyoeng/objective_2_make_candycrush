#include <cstdlib> // include rand()
#include <Windows.h>
#include <conio.h>
#include "GameObject.h"
#include "Canvas.h"
#include "BlinkablePlayer.h"
#include "Enemy.h"
#include "Bullet.h"
#include "InputManager.h"
#include "NumberCard.h"


Canvas GameObject::canvas(45, 49);
int GameObject::max_objs = 63;
GameObject** GameObject::objs = new GameObject * [GameObject::max_objs];

void GameObject::draw() { if (alive == true) canvas.draw(shape, position, dimension, visible); }



void GameObject::Initialize()
{
	for (int i = 0; i < max_objs; i++)
		objs[i] = nullptr;

	auto dim = canvas.getDimension();
	for (int i = 0; i < 7; i++)
	{	
		for (int j = 0; j < 9; j++)
		{
			static Vector2 temp = Vector2{ 0,0 };
			temp = { j * 5,i * 7 };
			switch (rand() % 5)
			{
				
			case 0:
				GameObject::Add(new NumberCard("     #    #    #    #    #         ", temp, Dimension{ 5, 7 }, true, 1));
				break;
			case 1:
				GameObject::Add(new NumberCard("     ###    #  ###  #    ###       ", temp, Dimension{5, 7}, true, 2));
				break;
			case 2:
				GameObject::Add(new NumberCard("     ###    #  ###    #  ###       ", temp, Dimension{ 5, 7 }, true, 3));
				break;
			case 3:
				GameObject::Add(new NumberCard("     # #  # #  ###    #    #       ", temp, Dimension{ 5, 7 }, true, 4));
				break;
			case 4:
				GameObject::Add(new NumberCard("     ###  #    ###    #  ###       ", temp, Dimension{ 5, 7 }, true, 5));
				break;
			}
		}
	}
}

void GameObject::Add(GameObject* obj)
{
	if (obj == nullptr) return;

	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] != nullptr) continue;
		objs[i] = obj;
		break;
	}
}

void GameObject::Remove(GameObject* obj)
{
	if (obj == nullptr) return;

	for (int i = 0; i < max_objs; i++)
	{
		if (obj == objs[i]) continue;

		delete objs[i];
		objs[i] = nullptr;
		return;
	}
}

void GameObject::ClearFrame()
{
	canvas.clear();
}

void GameObject::ProcessInput(bool& exit_flag, InputManager& input)
{
	Bullet* bullet = nullptr;
	BlinkablePlayer* player = nullptr;
	static NumberCard* select = nullptr;
	static NumberCard* destination = nullptr;

	if (input.getKey(0x49)) { // press 'i'
		for (int i = 0; i < max_objs; i++)
		{
			if (objs[i] == nullptr) continue;
			auto pos = objs[i]->getPosition();

			Debug::Log("[%2d: %s (%2df, %ddf) %d %d] ", i, objs[i]->getShape(), (int)pos.x, (int)pos.y, objs[i]->isVisible(), objs[i]->isAlive());
		}
		Bullet::printNumberOfCreatedBullets();
	}
	else if (input.getKeyDown(VK_SPACE)) {
		auto canvasDim = canvas.getDimension();

		// pick any player closer to a randomized position.
		auto randPos = Vector2{ (float)(rand() % (canvasDim.x - 5)), (float)(rand() % (canvasDim.y - 2)) };		
		for (int i = 0; i < max_objs; i++)
		{
			if (objs[i] == nullptr) continue;
			BlinkablePlayer* obj = dynamic_cast<BlinkablePlayer*>(objs[i]);
			if (obj == nullptr) continue;
			if (obj->isVisible() == false) continue;
			if (player == nullptr) {
				player = obj;
				continue;
			}
			// player is not null, that is it is assumed to find any player during array traversal.
			if (sqrDistance(obj->getPosition(), randPos) < sqrDistance(player->getPosition(), randPos) ) { // find a closer player from a random position
				player = obj;
			}
		}
		if (player != nullptr) { // if found
			Enemy* enemy = nullptr;
			for (int i = 0; i < max_objs; i++)
			{
				if (objs[i] == nullptr) continue;
				Enemy* obj = dynamic_cast<Enemy*>(objs[i]);
				if (obj == nullptr) continue;
				enemy = obj;
				break;
			}
			auto targetPos = Vector2{ (float)(rand() % canvasDim.x), (float)(rand() % canvasDim.y) };
			if (enemy) targetPos = enemy->getPosition();

			bullet = new Bullet("O", targetPos, { 1,1 }, 1.0f);
			GameObject::Add(bullet);
			bullet->fire(player, targetPos);

			player->setBlinkingPeriod(30);
		}
	}
	else if (input.getKeyDown(0x51)) { // 'q'
		exit_flag = true;
	}
	if (input.getMouseButtonDown(0)) {
		auto mousePos = input.getMousePosition();
		int x = mousePos.x;
		int y = mousePos.y;
		if (x >= 45 || y >= 49) return;
		NumberCard* obj = dynamic_cast<NumberCard*>(objs[x / 5+y / 7*9]);
		if (obj == nullptr) return;
		cout << obj->getType() << endl;
		select = obj;
		Borland::GotoXY(115, 0);
		cout<<select->getPosition().x<<" "<< select->getPosition().y;
		input.mou
	}
	if (input.getMouseButton(0)) {
		auto mousePos = input.getMousePosition();
	}
	if (input.getMouseButtonUp(0)) {
		auto mousePos = input.getMousePosition();
		int x = mousePos.x;
		int y = mousePos.y;
		if (x >= 45 || y >= 49) return;
		NumberCard* obj = dynamic_cast<NumberCard*>(objs[x / 5 + y / 7 * 9]);
		if (obj == nullptr) return;
		cout << obj->getType() << endl;
		destination = obj;
		Borland::GotoXY(115, 2);
		cout<<destination->getPosition().x << " " << destination->getPosition().y;
	}

	if (select == nullptr || destination == nullptr)return;

	//if (select->getPosition().x != destination->getPosition().x|| select->getPosition().y != destination->getPosition().y)
	//{
	//	select->change(*destination);
	//}

}

void GameObject::UpdateAll(InputManager& input)
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr || objs[i]->isAlive() == false) continue;
		objs[i]->update(input);
	}
	RemoveDeadObjects();
}

void GameObject::DrawAll()
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		objs[i]->draw();
	}
}

void GameObject::RenderFrame()
{
	canvas.render();
	Sleep(10);
}

void GameObject::ExamineCollision()
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		if (objs[i]->isAlive() == false) continue;
		ICollidable* c_i = dynamic_cast<ICollidable*>(objs[i]);
		if (c_i == nullptr) continue;
		for (int j = i + 1; j < max_objs && objs[i]->isAlive() == true; j++)
		{
			if (objs[j] == nullptr) continue;
			if (objs[j]->isAlive() == false) continue;
			ICollidable* c_j = dynamic_cast<ICollidable*>(objs[j]);
			if (c_j == nullptr) continue;
			if (objs[i]->isColliding(objs[j]) == false)  continue;
			c_j->onCollision(objs[i]);
			c_i->onCollision(objs[j]);
		}
	}
	RemoveDeadObjects();
}

void GameObject::RemoveDeadObjects()
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		if (objs[i]->isAlive() == true) continue;
		delete objs[i];
		objs[i] = nullptr;
	}
}

void GameObject::Deinitialize()
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i] == nullptr) continue;
		delete objs[i];
		objs[i] = nullptr;
	}
	delete[] objs;
}