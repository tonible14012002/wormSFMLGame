#include <iostream>
#include "GameManager.h"

int main(int argc, void* argv[]) {
	srand(time(0));

	sf::Clock clock;
	sf::Time time2 = clock.getElapsedTime();

	GameManager MyGame;
	while (!MyGame.GetWindowManager()->IsDone()){
		MyGame.Update();
		MyGame.Render();
	MyGame.LateUpdate();
	sf::Time time = clock.getElapsedTime();
	std::cout << 1.0f / (time - time2).asSeconds() << std::endl;
	time2 = time;
	}
}