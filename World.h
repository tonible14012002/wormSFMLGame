#pragma once
#include <SFML/Graphics.hpp>
#include "Define.h"
#include "Missile.h"
#include "Debris.h"
#include "Worm.h"
#include <list>
#include "WormTeam.h"

class World
{
public:
	World(int l_MapSize, int l_PixelSize, sf::RenderWindow*);
	~World();


	void GenerateNewMap();

	void Render();
	void Update(const sf::Time& l_Elapsed);

	void SpawnWorm();

	void LockView();
	bool IsUnderControl();
	void SetRotate(const bool l_Rotate, const bool l_Right);
	void Jump();
	void SetFire(const bool& l_Fire);
	void Fire();
	void HandleStateMachine(); //State Machine.

	void ShowFullview(const bool& l_FullView);

private:

	void GenerateNewSeed(float* Seed);
	void PerlinNoise1D(float* Seed, int m_Octaves, float* Output);
	void UpdateMapVertex();
	void UpdatePhysicObjects(const sf::Time& l_ElapsedTime);
	void MoveView();
	void Boom(float l_Radius, sf::Vector2f l_Pos);
	void CreateDebris(sf::Vector2f l_Pos);

	void DrawObject();
	void UpdateCrossHair(const sf::Time& l_Elapsed);
	void Charge(const sf::Time& l_Elapsed);
	void UpdateCamera(const sf::Time& l_Elapsed);
	void SetGameStable();
	bool GetNextTeam();
	void SpawnTeam();
	void GenerateTeamHealthBar();
	void UpdateTeamHealthBar();
	void ShowTheWinner();

	int m_MapSize;
	int m_PixelSize;

	bool* m_BitMap;
	sf::VertexArray m_VertexArray;
	sf::View m_FreeLook;

	sf::Vector2f m_CameraTarget;

	sf::RenderWindow* m_RenderWin;
	std::list<std::unique_ptr<PhysicObject>> m_Objects;

	Worm* m_OjUnderControl;
	PhysicObject* m_CameraTracking;
	
	bool m_IsRotate;
	bool m_RotateDirec;	 // 0: left, 1::right

	bool m_IsUnderControl;
	bool m_GameIsStable;
	bool m_PlayerActionCompleted;
	bool m_GameOver = false;

	sf::Texture m_CrosshairTexture;
	sf::Sprite m_CrossHairSprite;
	sf::Sprite m_BackGroundSprite;
	sf::Texture m_BackGroundTexture;

	bool m_Fire;
	float m_FireEnergy;
	sf::VertexArray m_EnergyBar;
	sf::VertexArray m_TeamHealthBar;

	GAME_STATE m_GameState, m_NextState;
	std::vector<WormTeam> m_TeamVec;
	int m_TeamNum;
	int m_WormPerTeam;
	int m_CurrentTeam;
	
	sf::Font m_Font;
	sf::Text m_GameOverText;
	bool m_FullView;
};


