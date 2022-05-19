#include "World.h"
#include <iostream>

World::World(int l_MapSize, int l_PixelSize,
	sf::RenderWindow* l_RenderWin ) :m_MapSize(l_MapSize),
m_PixelSize(l_PixelSize), m_RenderWin(l_RenderWin)
{
	m_BitMap = new bool[MAP_HEIGHT * MAP_WIDTH];

	m_VertexArray.setPrimitiveType(sf::Quads);
	m_VertexArray.resize(MAP_WIDTH * MAP_HEIGHT * 4);

	m_FreeLook.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	m_FreeLook.setCenter(0.3f * SCREEN_WIDTH / 2, 0.3f * SCREEN_HEIGHT / 2);
	m_FreeLook.zoom(0.3);

	int i = rand() % 6;
	switch (i)
	{
	case 0:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo01_PixelSky_1920x1080.png");
		break;
	case 1:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo02_PixelSky_1920x1080.png");
		break;
	case 2:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo03_PixelSky_1920x1080.png");
		break;
	case 3:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo04_PixelSky_1920x1080.png");
		break;
	case 4:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo05_PixelSky_1920x1080.png");
		break;
	case 5:
		m_BackGroundTexture.loadFromFile("Sprite/Background/demo06_PixelSky_1920x1080.png");
		break;
	}

	m_BackGroundSprite.setTexture(m_BackGroundTexture);
	m_BackGroundSprite.setScale(MAP_WIDTH/1920.0f,MAP_HEIGHT/ 1080.0f);
	m_BackGroundSprite.setPosition(0, 0);

	m_OjUnderControl = NULL;
	m_IsUnderControl = false;

	m_Font.loadFromFile("Font/SpaceMission-rgyw9.otf");
	m_GameOverText.setFont(m_Font);

	m_CrosshairTexture.loadFromFile("Sprite/Crosshair/crosshairs_tilesheet_white.png");
	m_CrossHairSprite.setTexture(m_CrosshairTexture);
	m_CrossHairSprite.setTextureRect(sf::IntRect(68.75f * 10, 0, 68.75, 68.5));
	m_CrossHairSprite.setOrigin(68.75f / 2, 68.5f / 2);
	m_CrossHairSprite.setScale(0.09f,0.09f);

	m_EnergyBar.setPrimitiveType(sf::Quads);
	m_EnergyBar.resize(8);
	for (int i = 0; i < 4; i++) {
		m_EnergyBar[i].color = sf::Color::Red;
		m_EnergyBar[i + 4].color = sf::Color::White;
	}

	m_Fire = false;
	m_FireEnergy = 0.0f;

	m_GameState = GAME_STATE::RESET;
	m_NextState = GAME_STATE::RESET;

	m_FullView = false;
}

World::~World() {
	delete[] m_BitMap;
}

void World::GenerateNewSeed(float* Seed) {

	for (int i = 0; i < m_MapSize; i++) {
		Seed[i] = (float)rand() / RAND_MAX;
	}
	Seed[0] = 0.5f;
}

void World::PerlinNoise1D(float* Seed, int m_Octaves, float* Output) {

	for (int i = 0; i < m_MapSize; i++) {
		float fNoise = 0.0f;
		float fScale = 1.0f;
		float fScaleAcc = 0.0f;
		for (int j = 0; j < m_Octaves; j++) {
			int nPitch = m_MapSize >> j;
			int nSample1 = (i / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % m_MapSize;
			float fBlend = (float)(i - nSample1) / (float)nPitch;
			float fSample = (1.0f - fBlend) * Seed[nSample1] + fBlend * Seed[nSample2];
			fNoise += fSample * fScale;
			fScaleAcc += fScale;
			fScale = fScale / 2.0f;
		}
		Output[i] = fNoise / fScaleAcc;
	}
}

void World::GenerateNewMap() {

	float* m_NoiseSeed = new float[m_MapSize];
	float* m_Surface = new float[m_MapSize];

	GenerateNewSeed(m_NoiseSeed);
	PerlinNoise1D(m_NoiseSeed, 8, m_Surface);

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {

			if (y >= m_Surface[x] * MAP_HEIGHT) {
				m_BitMap[y * MAP_WIDTH + x] = 1;

			}
			else {
				m_BitMap[y * MAP_WIDTH + x] = 0;

			}
		}
	}

	delete[] m_NoiseSeed;
	delete[] m_Surface;

	UpdateMapVertex();
}

void World::MoveView() {

	if (m_CameraTracking == NULL) {

	}
	else {
		m_CameraTarget = m_CameraTracking->m_Pos;
	}
	sf::Vector2f ViewSize = m_FreeLook.getSize();

	if (m_CameraTarget.x > MAP_WIDTH - ViewSize.x / 2) m_CameraTarget.x = MAP_WIDTH - ViewSize.x / 2;
	if (m_CameraTarget.y > MAP_HEIGHT - ViewSize.y / 2) m_CameraTarget.y = MAP_HEIGHT - ViewSize.y / 2;
	if (m_CameraTarget.x < ViewSize.x / 2) m_CameraTarget.x = ViewSize.x / 2;
	if (m_CameraTarget.y < ViewSize.y / 2) m_CameraTarget.y = ViewSize.y / 2;
	
}

void World::UpdateMapVertex() {

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {

			sf::Vertex* Quad = &m_VertexArray[(y * MAP_WIDTH + x) * 4];
			Quad[0].position = sf::Vector2f(x * m_PixelSize, y * m_PixelSize);
			Quad[1].position = sf::Vector2f(((x + 1) * m_PixelSize), y * m_PixelSize);
			Quad[2].position = sf::Vector2f((x + 1) * m_PixelSize, (y + 1) * m_PixelSize);
			Quad[3].position = sf::Vector2f(x * m_PixelSize, (y + 1) * m_PixelSize);

			if (m_BitMap[y * MAP_WIDTH + x]) {

				Quad[0].color = sf::Color(BROWN);
				Quad[1].color = sf::Color(BROWN);
				Quad[2].color = sf::Color(BROWN);
				Quad[3].color = sf::Color(BROWN);
			}
			else {
				Quad[0].color = sf::Color(34, 168, 191, (MAP_HEIGHT - y) / 5);
				Quad[1].color = sf::Color(34, 168, 191, (MAP_HEIGHT - y) / 5);
				Quad[2].color = sf::Color(34, 168, 191, (MAP_HEIGHT - y) / 5);
				Quad[3].color = sf::Color(34, 168, 191, (MAP_HEIGHT - y) / 5);
		
			}
		}
	}
}

void World::CreateDebris(sf::Vector2f l_Pos) {
	srand(time(0));
	for (int i = 0; i < 20; i++) {
		Debris* NewObject = new Debris(l_Pos);
		m_Objects.push_back(std::unique_ptr<Debris>(NewObject));
	}
}

void World::Boom(float l_Radius,sf::Vector2f l_Pos) {

	//Erase pixel in the circle.
	auto CircleBresenham = [&](int Xc, int Yc, int r) {
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;
		auto DrawLine = [&](int Sx, int Ex, int Ny) {
			for (int i = Sx; i < Ex; i++) {
				if (Ny >= 0 && Ny < MAP_HEIGHT && i >= 0 && i < MAP_WIDTH) {
					m_BitMap[Ny * MAP_WIDTH + i] = 0;
					sf::Vertex* Quad = &m_VertexArray[(Ny * MAP_WIDTH + i) * 4];
					Quad[0].color = sf::Color(34, 168, 191, (MAP_HEIGHT - Ny) / 5);
					Quad[1].color = sf::Color(34, 168, 191, (MAP_HEIGHT - Ny) / 5);
					Quad[2].color = sf::Color(34, 168, 191, (MAP_HEIGHT - Ny) / 5);
					Quad[3].color = sf::Color(34, 168, 191, (MAP_HEIGHT - Ny) / 5);
				}
			}
		};

		while (y >= x)
		{
			DrawLine(Xc - x, Xc + x, Yc - y);
			DrawLine(Xc - y, Xc + y, Yc - x);
			DrawLine(Xc - x, Xc + x, Yc + y);
			DrawLine(Xc - y, Xc + y, Yc + x);
			if (p < 0) p += 4 * x++ + 6;
			else p += 4 * (x++ - y--) + 10;
		}

	};

	CircleBresenham(l_Pos.x, l_Pos.y, l_Radius);

	for (auto &pOject : m_Objects) {
		sf::Vector2f DistVec = pOject->m_Pos - l_Pos;
		float Dist = sqrtf(DistVec.x * DistVec.x + DistVec.y * DistVec.y);
		if (Dist < 0.0001f) Dist = 0.0001;
		if (Dist < l_Radius) {
			pOject->m_Velo = 0.7f*DistVec*(l_Radius/Dist);
			pOject->TakeDamage(((l_Radius - Dist) / l_Radius)*70.0f);
			pOject->m_Stable = false;
		}
	}

	CreateDebris(l_Pos);
}

void World::UpdatePhysicObjects(const sf::Time& l_Elapsed) {

	for (int z = 0; z < 10; z++) {
		for (auto& pObject : m_Objects) {
			//Apply gravity.
			pObject->m_Acc.y += 2.0f;
			//Update velocity.
			pObject->m_Velo += pObject->m_Acc * l_Elapsed.asSeconds() ;
			//Potential Position.
			sf::Vector2f PotentialPos = pObject->m_Pos + pObject->m_Velo * l_Elapsed.asSeconds();
			//reset acceleration.
			pObject->m_Acc = sf::Vector2f(0, 0);
			pObject->m_Stable = false;

			//check collision with Map.
			float Angle = atan2f(pObject->m_Velo.y, pObject->m_Velo.x);

			sf::Vector2f ResponseVec = sf::Vector2f(0, 0);
			bool IsCollision = false;
			for (float r = Angle -Pi/2.0f; r <= Angle + Pi/2.0f; r += Pi / 8.0f) {
				sf::Vector2f TestPos(0,0);
				TestPos.x = pObject->m_Radius * cosf(r) + PotentialPos.x;
				TestPos.y = pObject->m_Radius * sinf(r) + PotentialPos.y;

				if (TestPos.x >= MAP_WIDTH) TestPos.x = MAP_WIDTH - 1;
				if (TestPos.x < 0) TestPos.x = 0;
				if (TestPos.y >= MAP_HEIGHT) TestPos.y = MAP_HEIGHT - 1;
				if (TestPos.y < 0) TestPos.y = 0;
				
				if (m_BitMap[(int)TestPos.y * MAP_WIDTH + (int)TestPos.x] != 0){
					ResponseVec += PotentialPos - TestPos;
					IsCollision = true;
				}
			}

			float MagVelo = sqrtf(pObject->m_Velo.x * pObject->m_Velo.x +
				pObject->m_Velo.y * pObject->m_Velo.y);
			float MagRespon = sqrtf(ResponseVec.x * ResponseVec.x +
				ResponseVec.y * ResponseVec.y);

			if (IsCollision) {
				pObject->m_Stable = true;

				float Dot = pObject->m_Velo.x * (ResponseVec.x / MagRespon) +
					pObject->m_Velo.y * (ResponseVec.y / MagRespon);

				pObject->m_Velo = pObject->m_Friction*(-2.0f * Dot * (ResponseVec/ MagRespon) + pObject->m_Velo);
				
				//object die after several bounce.
				if (pObject->m_BounceBeforeDeath > 0) pObject->m_BounceBeforeDeath--;
				pObject->m_Dead = pObject->m_BounceBeforeDeath == 0;
			}
			else {
			// Update position to potential pos if no collision occur.
			pObject->m_Pos = PotentialPos;
			}
			// Turn of movement when velocity is tiny.
			if (MagVelo < 0.1) {
				pObject->m_Stable = true;
			}

			if (pObject->m_Dead) {
				int BoomRadius = pObject->BounceToDeathAction();
				if (BoomRadius > 0) {
					Boom(BoomRadius, pObject->m_Pos);
					m_CameraTracking = NULL;
				}
			}

		}
		m_Objects.remove_if([](std::unique_ptr<PhysicObject>& Ob) {return Ob->m_Dead; });
	}
}

void World::LockView() {
	m_IsUnderControl = !m_IsUnderControl;
	std::cout << "CHanged view";
}

void World::DrawObject() {

	for (auto& Ob : m_Objects) {
		Ob->Draw(m_RenderWin);
	}

	if (m_OjUnderControl != NULL && m_IsUnderControl) {
		sf::Vector2f CPos =  sf::Vector2f(cosf(m_OjUnderControl->m_CrossHairAngle), sinf(m_OjUnderControl->m_CrossHairAngle)) * 15.0f;
		m_CrossHairSprite.setPosition(CPos + m_OjUnderControl->m_Pos);
		m_RenderWin->draw(m_CrossHairSprite);
	}

}

bool World::IsUnderControl() { return m_IsUnderControl; }

void World::SetRotate(const bool l_Rotate, const bool l_Right = false) {
	m_IsRotate = l_Rotate;
	m_RotateDirec = l_Right;
}

void World::UpdateCrossHair(const sf::Time& l_Elapsed) {
	
	if (!m_IsRotate) return;
	//True = Right
	if (m_RotateDirec == true){
		m_OjUnderControl->m_CrossHairAngle += 5.0f * l_Elapsed.asSeconds();
	}
	else
	{
		m_OjUnderControl->m_CrossHairAngle -= 5.0f * l_Elapsed.asSeconds();
	}
	if (m_OjUnderControl->m_CrossHairAngle > 2.0f * Pi) m_OjUnderControl->m_CrossHairAngle = 0.0f;
	if (m_OjUnderControl->m_CrossHairAngle < 0) m_OjUnderControl->m_CrossHairAngle = 2 * Pi;
}

void World::Jump() {
	if(m_IsUnderControl)
		if (m_OjUnderControl->m_Stable) {
			m_OjUnderControl->m_Velo += sf::Vector2f(cosf(m_OjUnderControl->m_CrossHairAngle), 
				sinf(m_OjUnderControl->m_CrossHairAngle)) * 10.0f;
		}
}

void World::Charge(const sf::Time& l_Elapsed) {
		m_FireEnergy += 0.8f * l_Elapsed.asSeconds();
		if (m_FireEnergy > 1.0f) m_FireEnergy = 1.0f;

		m_EnergyBar[0].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f, m_OjUnderControl->m_Pos.y - 7.0f);
		m_EnergyBar[3].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f, m_OjUnderControl->m_Pos.y - 5.0f);
		m_EnergyBar[1].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + m_FireEnergy*15, m_OjUnderControl->m_Pos.y - 7.0f);
		m_EnergyBar[2].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + m_FireEnergy*15, m_OjUnderControl->m_Pos.y - 5.0f);

		m_EnergyBar[5].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + 15, m_OjUnderControl->m_Pos.y - 7.0f);
		m_EnergyBar[6].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + 15, m_OjUnderControl->m_Pos.y - 5.0f);
		m_EnergyBar[4].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + m_FireEnergy * 15, m_OjUnderControl->m_Pos.y - 7.0f);
		m_EnergyBar[7].position = sf::Vector2f(m_OjUnderControl->m_Pos.x - 6.0f + m_FireEnergy * 15, m_OjUnderControl->m_Pos.y - 5.0f);
}

void World::SpawnWorm() {
	
	Worm* NewObject = new Worm(sf::Vector2f(MAP_WIDTH /2,0));
	m_Objects.push_back(std::unique_ptr<Worm>(NewObject));
	m_OjUnderControl = NewObject;
	m_CameraTracking = m_OjUnderControl;
}

void World::Fire() {
	if (m_Fire)
		if (m_IsUnderControl && m_OjUnderControl->m_Stable) {
				Missile* NewMissile = new Missile(m_OjUnderControl->m_Pos);
				NewMissile->m_Velo = sf::Vector2f(cosf(m_OjUnderControl->m_CrossHairAngle),
					sinf(m_OjUnderControl->m_CrossHairAngle)) * 50.0f * m_FireEnergy;
				m_Objects.push_back(std::unique_ptr<Missile>(NewMissile));
				m_CameraTracking = NewMissile;
				m_PlayerActionCompleted = true;
		}
	m_FireEnergy = 0.0f;
	m_Fire = false;
}

void World::SetFire(const bool& l_Fire) {
	m_Fire = l_Fire;
}

void World::UpdateCamera(const sf::Time& l_Elapsed) {
	if (m_CameraTracking != NULL) {
		sf::Vector2f CamPos = m_FreeLook.getCenter();
		CamPos += (m_CameraTarget - CamPos) * 5.0f * l_Elapsed.asSeconds();
		m_FreeLook.setCenter(CamPos);
	}
}

void World::SetGameStable() {
	m_GameIsStable = true;
	for (auto& Ob : m_Objects) {
		if (!Ob->m_Stable) {
			m_GameIsStable = false;
			break;
		}
			
	}
}

void World::HandleStateMachine() {
	switch (m_GameState)
	{
	case GAME_STATE::RESET:
		m_OjUnderControl = NULL;
		m_IsUnderControl = false;
		m_GameOver = false;
		m_TeamNum = 2;
		m_WormPerTeam = 4;
		m_NextState = GAME_STATE::GENERATE_TERRAIN;
		std::cout << "Reset\n";
		break;
	case GAME_STATE::GENERATE_TERRAIN:
		m_IsUnderControl = false;
		GenerateNewMap();
		m_NextState = GAME_STATE::GENERATING_TERRAIN;
		break;
	case GAME_STATE::GENERATING_TERRAIN:
		m_NextState = GAME_STATE::ALLOCATE_UNIT;
		break;
	case GAME_STATE::ALLOCATE_UNIT:
		m_IsUnderControl = false;	
		SpawnTeam();
		m_OjUnderControl = m_TeamVec[0].m_MemberVec[m_TeamVec[0].m_CurrentMember];
		m_CameraTracking = m_OjUnderControl;
		m_NextState = GAME_STATE::ALLOCATING_UNIT;
		break;
	
	case GAME_STATE::ALLOCATING_UNIT:
		GenerateTeamHealthBar();
		m_IsUnderControl = false;
		m_CurrentTeam = 0;
		if (m_GameIsStable) {
			m_PlayerActionCompleted = false;
			m_NextState = GAME_STATE::START_PLAY;
		}
		break;
	case GAME_STATE::START_PLAY:
		m_IsUnderControl = true;
		UpdateTeamHealthBar();
		if (m_PlayerActionCompleted) m_NextState = GAME_STATE::CAMERA_MODE;
		break;
	case GAME_STATE::CAMERA_MODE:
		m_IsUnderControl = false;
		m_PlayerActionCompleted = false;
		if (m_GameIsStable) {
			if (GetNextTeam()) {
				m_NextState = GAME_STATE::START_PLAY;
			}
			else {
				m_NextState = GAME_STATE::GAME_OVER;
			}
		}
		break;
	case GAME_STATE::GAME_OVER:
		std::cout << m_GameOver;
		UpdateTeamHealthBar();
		ShowTheWinner();
		break;
	default:
		break;
	}
}

void World::Update(const sf::Time& l_Elapsed) {

	SetGameStable();
	HandleStateMachine();
	MoveView();
	UpdatePhysicObjects(l_Elapsed);

	for (auto& Ob : m_Objects) {
		Ob->UpdateAnimation(l_Elapsed);
	}

	UpdateCrossHair(l_Elapsed);
	if (m_Fire) Charge(l_Elapsed);
	UpdateCamera(l_Elapsed);

	m_GameState = m_NextState;
}

void World::Render() {
	
	m_RenderWin->setView( m_RenderWin->getDefaultView());
	m_RenderWin->draw(m_BackGroundSprite);
	
	if (!m_FullView) {
		m_RenderWin->setView(m_FreeLook);
	}

	m_RenderWin->draw(m_VertexArray);
	DrawObject();
	if (m_Fire) m_RenderWin->draw(m_EnergyBar);

	m_RenderWin->setView(m_RenderWin->getDefaultView());
	m_RenderWin->draw(m_TeamHealthBar);
	if (m_GameOver) m_RenderWin->draw(m_GameOverText);

}

void World::SpawnTeam() {

	int Space = MAP_WIDTH / m_TeamNum - 200;

	for (int i = 0; i < m_TeamNum; i++) {

		m_TeamVec.emplace_back(WormTeam(m_WormPerTeam));

		for (int j = 0; j < m_WormPerTeam; j++) {
			float WormX = 100 + i*Space + float(rand() % Space);
			Worm* NewWorm = new Worm(sf::Vector2f(WormX, 0.0f),i);
			m_Objects.push_back(std::unique_ptr<Worm>(NewWorm));
			m_TeamVec[i].m_MemberVec.push_back(NewWorm);

		}
	}
}

bool World::GetNextTeam() {

	int OldTeam = m_CurrentTeam;
	int WormCount = 1;
	int TotalWorm = m_WormPerTeam * m_TeamNum;
	do{
		m_CurrentTeam++;
		WormCount++;
		m_CurrentTeam %= m_TeamVec.size();
	} while (!m_TeamVec[m_CurrentTeam].IsTeamAlive() && WormCount <= TotalWorm);

	if (m_CurrentTeam == OldTeam || WormCount > TotalWorm) {
		return false;
	}
	m_OjUnderControl = m_TeamVec[m_CurrentTeam].GetNextMember();
	m_CameraTracking = m_OjUnderControl;
	return true;
}

void World::GenerateTeamHealthBar() {
	m_TeamHealthBar.resize(8 * m_TeamNum);
	m_TeamHealthBar.setPrimitiveType(sf::Quads);

	for (int i = 0; i < m_TeamNum; i++) {
		for (int j = 0; j < 4; j++) {
			switch (i){
			case 0:
				m_TeamHealthBar[i*8 + j].color = sf::Color(TEAMBLUE);
				break;
			case 1:
				m_TeamHealthBar[i*8 + j].color = sf::Color(TEAMPINK);
				break;
			case 2:
				m_TeamHealthBar[i*8 + j].color = sf::Color(TEAMGREY);
				break;
			}
			m_TeamHealthBar[j*8 + 4].color = sf::Color::White;
		}
	}

}

void World::UpdateTeamHealthBar() {

	for (int i = 0; i < m_TeamNum; i++) {

		float HealthRate = (m_TeamVec[i].GetTeamHealth()) / (100.0f*m_WormPerTeam);

		m_TeamHealthBar[0 + 8*i].position = sf::Vector2f(10.0f, 10.0f + i*15.0f);
		m_TeamHealthBar[1 + 8 * i].position = sf::Vector2f(10.0f + HealthRate*200.0f, 10.0f + i * 15.0f);
		m_TeamHealthBar[2 + 8 * i].position = sf::Vector2f(10.0f + HealthRate * 200.0f, 10.0f + 10.0f + i * 15.0f);
		m_TeamHealthBar[3 + 8 * i].position = sf::Vector2f(10.0f, 10.0f + 10.0f + i * 15.0f);

		m_TeamHealthBar[5 + 8 * i].position = sf::Vector2f(10.0f + 200.0f, 10.0f + i * 15.0f);
		m_TeamHealthBar[4 + 8 * i].position = sf::Vector2f(10.0f + HealthRate * 200.0f, 10.0f + i * 15.0f);
		m_TeamHealthBar[7 + 8 * i].position = sf::Vector2f(10.0f + HealthRate * 200.0f, 10.0f + 10.0f + i * 15.0f);
		m_TeamHealthBar[6 + 8 * i].position = sf::Vector2f(10.0f + 200.0f, 10.0f + 10.0f + i * 15.0f);
	}
	

}

void World::ShowTheWinner() {

	if (m_GameOver) return;

	m_GameOver = true;

	int Winner = 3;

	for (int i = 0; i < m_TeamNum; i++) {
		if (m_TeamVec[i].IsTeamAlive()) {
			Winner = i;
			break;
		}
	}

	m_GameOverText.setCharacterSize(100.0f);

	switch (Winner)
	{
	case 0:
		m_GameOverText.setString("BLUE WIN");
		m_GameOverText.setFillColor(sf::Color(TEAMBLUE));
		break;
	case 1:
		m_GameOverText.setString("PINK WIN");
		m_GameOverText.setFillColor(sf::Color(TEAMPINK));
		break;
	case 2:
		m_GameOverText.setString("GRAY WIN");
		m_GameOverText.setFillColor(sf::Color(TEAMGREY));
		break;
	case 3:
		m_GameOverText.setString("DRAW!!");
		m_GameOverText.setFillColor(sf::Color::Red);
		break;
	}

	sf::FloatRect TextBound = m_GameOverText.getGlobalBounds();
	m_GameOverText.setFillColor(sf::Color::Black);
	m_GameOverText.setPosition(sf::Vector2f(m_RenderWin->getSize()) / 2.0f - sf::Vector2f(TextBound.width/2 - 25.0f,TextBound.height));

}

void World::ShowFullview(const bool& l_Fullview) {
	m_FullView = l_Fullview;
}
