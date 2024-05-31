#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vec2.h"
#include "Game.h"
#include <fstream>



int main()
{

	Game g("config.txt");
	g.run();

}