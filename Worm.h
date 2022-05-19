#pragma once
#include "PhysicObject.h"
#include <iostream>

class Worm : public PhysicObject
{
public:
	Worm(sf::Vector2f l_Pos, const int& TeamColor = 0) : PhysicObject(l_Pos) {

		m_IsPlayable = true;
		m_Health = 100.0f;
		m_BounceBeforeDeath = -1;
		m_Radius = 5.0f;
		m_Friction = 0.2f;
		m_Dead = false;
		m_XSpriteCoordinate[0] = 0;
		m_XSpriteCoordinate[1] = 3;
		m_Time = 0.0f;

		switch (TeamColor){
			case 0:
				m_Texture[0].loadFromFile("Sprite/Characters/Dude_Monster/Dude_Monster_Idle_4.png");
				m_Texture[1].loadFromFile("Sprite/Characters/Dude_Monster/Dude_Monster_Jump_8.png");
				break;
			case 1:
				m_Texture[0].loadFromFile("Sprite/Characters/Pink_Monster/Pink_Monster_Idle_4.png");
				m_Texture[1].loadFromFile("Sprite/Characters/Pink_Monster/Pink_Monster_Jump_8.png");
				break;
			case 2:
				m_Texture[0].loadFromFile("Sprite/Characters/Owlet_Monster/Owlet_Monster_Idle_4.png");
				m_Texture[1].loadFromFile("Sprite/Characters/Owlet_Monster/Owlet_Monster_Jump_8.png");
				break;
		}
			
		m_HealthBar.resize(8);
		m_HealthBar.setPrimitiveType(sf::Quads);
		for (int i = 0; i < 4; i++) {
			m_HealthBar[i].color = sf::Color::Green;
			m_HealthBar[i + 4].color = sf::Color::White;
		}

	}

	void Draw(sf::RenderWindow* l_Window) {
		m_Sprite.setPosition(m_Pos);
		l_Window->draw(m_Sprite);
		l_Window->draw(m_HealthBar);
	}

	void UpdateAnimation(const sf::Time& l_Elapsed) {
		m_Time += l_Elapsed.asSeconds();

		if (m_Stable) {
			if (m_Time > 0.3f) {
				m_Time = 0.0f;
				m_XSpriteCoordinate[0] += 1;
				m_XSpriteCoordinate[1] = 3;
				if (m_XSpriteCoordinate[0] >= 4) m_XSpriteCoordinate[0] = 0;
			}
			m_Sprite.setTexture(m_Texture[0]);
			m_Sprite.setTextureRect(sf::IntRect(m_XSpriteCoordinate[0] * 32, 0, 32, 32));
		}
		else {
			if (m_Time > 0.3f) {
				m_Time = 0.0f;
				m_XSpriteCoordinate[1] += 1;
				m_XSpriteCoordinate[0] = 0;
					if (m_XSpriteCoordinate[1] >= 8) m_XSpriteCoordinate[1] = 3;
			}
			m_Sprite.setTexture(m_Texture[1]);
			m_Sprite.setTextureRect(sf::IntRect(m_XSpriteCoordinate[1] * 32, 0, 32, 32));

		}
		m_Sprite.setOrigin(16, 16);
		if (m_CrossHairAngle < Pi / 2.0f || m_CrossHairAngle > 1.5f * Pi)
			m_Sprite.setScale(0.35f, 0.35f);
		else m_Sprite.setScale(-0.35f, 0.35f);

		float HRate = (float)m_Health/100.0f;
		//Update HealthBar;
		m_HealthBar[0].position = sf::Vector2f(m_Pos.x - 6.0f, m_Pos.y - 10.0f);
		m_HealthBar[1].position = sf::Vector2f(m_Pos.x - 6.0f + HRate * 15.0f, m_Pos.y - 10.0f);
		m_HealthBar[2].position = sf::Vector2f(m_Pos.x - 6.0f + HRate * 15.0f, m_Pos.y - 8.0f);
		m_HealthBar[3].position = sf::Vector2f(m_Pos.x - 6.0f, m_Pos.y - 8.0f);

		m_HealthBar[5].position = sf::Vector2f(m_Pos.x - 6.0f + 15.0f, m_Pos.y - 10.0f);
		m_HealthBar[4].position = sf::Vector2f(m_Pos.x - 6.0f + HRate * 15.0f, m_Pos.y - 10.0f);
		m_HealthBar[7].position = sf::Vector2f(m_Pos.x - 6.0f + HRate * 15.0f, m_Pos.y - 8.0f);
		m_HealthBar[6].position = sf::Vector2f(m_Pos.x - 6.0f + 15.0f, m_Pos.y - 8.0f);


	}

	int BounceToDeathAction() { return 20; }
	void TakeDamage(float Damage) {
		if (!m_IsPlayable) return;
		m_Health -= Damage;
		if (m_Health <= 0) {
			m_Health = 0;
			m_IsPlayable = false;
		}
	}

	float m_CrossHairAngle = 0.0f;
	bool m_IsPlayable;
private:
	int m_XSpriteCoordinate[2];
	sf::Texture m_Texture[2];
	sf::Sprite m_Sprite;
	float m_Time;
	sf::VertexArray m_HealthBar;
};