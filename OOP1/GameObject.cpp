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
	if (!select || !destination)
		return;
	if (select->getPosition().distance(destination->getPosition())<4.9f || select->getPosition().distance(destination->getPosition())> 7.1f)//한칸의 이동만 처리하기 위함
	{
		destination = nullptr;
		select = nullptr;
		return;
	}
	if (input.getMouseButtonUp(0))
	{
		//Borland::GotoXY(115, 5);
		//cout << "my objective" << endl;
		NumberCard *temp = new NumberCard(destination->getShape(),destination->getPosition(),destination->getDimension(), true, destination->getType());
		//cout << destination->getType() << endl;
		//cout << select->getType() << endl;

		destination->setType(select->getType());
		select->setType(temp->getType());
		//

		//cout << destination->getType() << endl;
		//cout << select->getType() << endl;



		if (destination->getType() != select->getType())
		{
			destination->reTouch(select->getShape());
			select->reTouch(temp->getShape());
			//select->draw();
			//destination->draw();
			//canvas.render();

		}


 		destination = nullptr;
		select = nullptr;
		//select->draw();
		//destination->draw();
		//canvas.render();
	}
	

}

void GameObject::UpdateAll(InputManager& input)
{
	for (int i = 0; i < max_objs; i++)
	{
		if (objs[i]->isVisible() == false)
		{
			dynamic_cast<NumberCard*>(objs[i])->change(rand() % 5);
		}
	}
	
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
	Sleep(50);
}

void GameObject::ExamineCollision()
{
	CardType x = CardType::One, y = CardType::One;
	//타입으로 다음것을 찾고 이를 분석해서 해결할 예정
	//static NumberCard** cardTotal;
	//for(int i=0;i<max_objs;i++)
	
	//	 cardTotal [i] = dynamic_cast<NumberCard*>(objs[i]);
	for (int i = 0; i < 7; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (i * 9 + j == 62) break;
			if (dynamic_cast<NumberCard*>(objs[i * 9 + j])->getType() == dynamic_cast<NumberCard*>(objs[i * 9 + j + 1])->getType())
			{
				switch (x)
				{
				case CardType::One:
					x = CardType::Two;
					continue;
				case CardType::Two:
					x = CardType::Three;
					continue;
				case CardType::Three:
					x = CardType::Four;
					continue;
				case CardType::Four:
					x = CardType::Five;
					continue;
				case CardType::Five:
					x = CardType::Six;
					continue;
				case CardType::Six:
					x = CardType::Seven;
					continue;
				case CardType::Seven:
					x = CardType::Eight;
					continue;
				case CardType::Eight:
					x = CardType::Nine;
					continue;
				}
			}
			if (x < CardType::Three)
			{
				x = CardType::One;
				continue;
			}

			for (int type = 0; type <= (int)x; type++)
			{
				dynamic_cast<NumberCard*>(objs[i*9+j-type])->setVisible(false);//is mean pang
			}



			x = CardType::One;//원상태로 복귀


		}
		x = CardType::One;


	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			
			if ( j != 6&&dynamic_cast<NumberCard*>(objs[i+ j*9])->getType() == dynamic_cast<NumberCard*>(objs[i  + (j + 1)*9])->getType())
				switch (y)
				{
				case CardType::One:
					y = CardType::Two;
					continue;
				case CardType::Two:
					y = CardType::Three;
					continue;
				case CardType::Three:
					y = CardType::Four;
					continue;
				case CardType::Four:
					y = CardType::Five;
					continue;
				case CardType::Five:
					y = CardType::Six;
					continue;
				case CardType::Six:
					y = CardType::Seven;
					continue;
				}

			if (y < CardType::Three)
			{
				y = CardType::One;
				continue;
			}


			for (int type = 0; type <= (int)y; type++)
			{
				dynamic_cast<NumberCard*>(objs[i + (j-type)*9])->setVisible(false);//is mean pang
			}

			y = CardType::One;//원상태로 복귀

		}
		y = CardType::One;

	}

	//RemoveDeadObjects();
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