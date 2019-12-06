#include "ModelLoader.h"

int main()
{

	ModelLoader loader("Model Loader",
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