#pragma once
#include <SFML/Graphics.hpp>

class PhysicObject
{
public:

	PhysicObject(sf::Vector2f l_Pos = sf::Vector2f(0, 0)) {
		m_Pos = l_Pos;
	}
	virtual void Draw(sf::RenderWindow* l_RenderWin) = 0;

	virtual void UpdateAnimation(const sf::Time& l_Elapsed) = 0;

	virtual int BounceToDeathAction() = 0;

	virtual void TakeDamage(float Damage) = 0;

	sf::Vector2f m_Pos = sf::Vector2f(0, 0);
	sf::Vector2f m_Acc = sf::Vector2f(0, 0);
	sf::Vector2f m_Velo = sf::Vector2f(0, 0);
	float m_Radius = 4.0f;
	bool m_Stable = false;
	float m_Friction = 0.8f;
	int m_BounceBeforeDeath = -1; //unlimited bounce.
	bool m_Dead = false;

	float m_Health = 0.0f;
};

