#include "PhysicObject.h"
#include "Define.h"

class Debris : public PhysicObject
{
public:
	Debris(sf::Vector2f l_Pos) :PhysicObject(l_Pos) {
		m_Radius = 1;
		m_BounceBeforeDeath = 3;
		m_Shape.setRadius(m_Radius);
		m_Shape.setOrigin(m_Radius, m_Radius);
		m_Shape.setFillColor(sf::Color(BROWN));
		m_Velo.x = 10 * cosf(2.0f * Pi * ((float)rand() / (float)RAND_MAX));
		m_Velo.y = 10 * sinf(2.0f * Pi * ((float)rand() / (float)RAND_MAX));
	}

	void Draw(sf::RenderWindow* l_Window) {
		m_Shape.setPosition(m_Pos);
		l_Window->draw(m_Shape);
	}

	void UpdateAnimation(const sf::Time& l_Elapsed) { return; }

	int BounceToDeathAction() { return 0; }

	void TakeDamage(float Damage) { return; }

private:
	sf::CircleShape m_Shape;
};