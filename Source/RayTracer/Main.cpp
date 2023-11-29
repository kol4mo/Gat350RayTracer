#include <iostream>
#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"

int main(int, char**) {
	std::cout << "Hello World!\n";

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("hi", 400, 300);
	Canvas canvas(400, 300, renderer);
	bool quit = false;
	while (!quit) {
	canvas.Clear({ 0, 0, 0, 1 });
	for (int i = 0; i < 1000; i++) canvas.DrawPoint({random(0, 400), random(0, 300)}, {random(0, 255), random(0, 255), random(0, 255), 1});
	canvas.Update();

	renderer.PresentCanvas(&canvas);
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		}
	}

	renderer.Shutdown();

	return 0;
}