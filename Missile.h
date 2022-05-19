#pragma once
#include "PhysicObject.h"

class Missile : public PhysicObject
{
public:
	Missile(sf::Vector2f l_Pos) :PhysicObject(l_Pos) {
		m_BounceBeforeDeath = 1;
		m_Friction = 0.7f;
		m_Radius = 3.0f;
		m_Texture.loadFromFile("Sprite/Missile/missile_medium.png");
		m_Sprite.setTexture(m_Texture);
		m_Sprite.setTextureRect(sf::IntRect(0,0, 13, 28));
		m_Sprite.setOrigin(6.5f, 14);
		m_Sprite.setScale(0.3,0.3);
	};

	void Draw(sf::RenderWindow* l_Window) {
		m_Sprite.setPosition(m_Pos);
		l_Window->draw(m_Sprite);
	}

	void UpdateAnimation(const sf::Time& l_Elapsed) {

		float A = Pi + atan2f(-m_Velo.x, m_Velo.y);
		m_Sprite.setRotation((float)(A*180)/Pi);
	}

	int BounceToDeathAction() { return 25; }

	void TakeDamage(float l_Damage) { return; }

private:
	sf::Sprite m_Sprite;
	sf::Texture m_Texture;
};

