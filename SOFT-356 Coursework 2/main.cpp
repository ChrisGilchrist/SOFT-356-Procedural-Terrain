#include "Game.h"

int main()
{

	Game loader("SOFT356 Interactive Prototype",
		1920, 1080,
		4, 4,
		false);

	//MAIN LOOP
	while (!loader.getWindowShouldClose())
	{
		//UPDATE INPUT ---
		loader.update();
		loader.render();
	}
	

	return 0;
}