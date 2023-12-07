#include <iostream>
#include "Renderer.h"
#include "Canvas.h"
#include "Random.h"
#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Mesh.h"
#include <memory>
#include <glm/gtx/color_space.hpp>

void InitScene01(Scene& scene, const Canvas& canvas);
void InitScene02(Scene& scene, const Canvas& canvas);
void InitCornellBox(Scene& scene, const Canvas& canvas);
void InitCornellBoxPlane(Scene& scene, const Canvas& canvas);


int main(int, char**) {
	std::cout << "Hello World!\n";

	const int width = 400;
	const int height = 300;
	const int depth = 8;
	const int samples = 1000;

	seedRandom((unsigned int)time(nullptr));

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("hi", width, height);
	Canvas canvas(width, height, renderer);
	Scene scene( glm::vec3{ 1 }, glm::vec3{ 0.5f, 0.7f, 1.0f});// sky color could be set with the top and bottom color
	InitScene01(scene, canvas);
	// render scene 
	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, samples, depth);
	canvas.Update();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = true;
						break;
				}
				break;
		}

		renderer.PresentCanvas(&canvas);
	}

	renderer.Shutdown();

	return 0;
}

void InitScene01(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 2, 10 }, glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);
	scene.SetCamera(camera);

	// create objects -> add to scene
	for (int x = -10; x < 10; x++)
	{
		for (int z = -10; z < 10; z++)
		{
			std::shared_ptr<Material> material;

			// create random material
			float r = random01();
			if (r < 0.3f)		material = std::make_shared<Lambertian>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }));
			else if (r < 0.6f)	material = std::make_shared<Metal>(color3_t{ random(0.5f, 1.0f) }, random(0, 0.5f));
			else if (r < 0.9f)	material = std::make_shared<Dielectric>(color3_t{ 1.0f }, random(1.1f, 2));
			else				material = std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }), 5.0f);

			// set random sphere radius
			float radius = random(0.2f, 0.3f);
			// create sphere using random radius and material
			auto sphere = std::make_unique<Sphere>(glm::vec3{ x + random(-0.5f, 0.5f), radius, z + random(-0.5f, 0.5f) }, radius, material);
			// add sphere to the scene
			scene.AddObject(std::move(sphere));
		}
	}

	auto plane = std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, std::make_shared<Lambertian>(color3_t{ 0.2f }));
	scene.AddObject(std::move(plane));
}

void InitScene02(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 2, 10 }, glm::vec3{ 0, 1, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);
	scene.SetCamera(camera);

	//auto triangle = std::make_unique<Triangle>(glm::vec3{ -1, 0, 0 }, glm::vec3{ 1, 0, 0 }, glm::vec3{ 0, 2, 0 }, std::make_shared<Lambertian>(color3_t{ 1, 0, 0 }));
	//scene.AddObject(std::move(triangle));

	auto plane = std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, std::make_shared<Lambertian>(color3_t{ 0.2f }));
	scene.AddObject(std::move(plane));

	auto mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 0, 1 }));
	mesh->Load("models/cube-1.obj", glm::vec3{ 0, 0.5f, 0 }, glm::vec3{ 0, 45, 0 });
	scene.AddObject(std::move(mesh));
}

void InitCornellBox(Scene& scene, const Canvas& canvas) {
	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 3 }, glm::vec3{ 0, 1, -1 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);
	scene.SetCamera(camera);

	auto mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	mesh->Load("models/quad-1.obj", glm::vec3{ 0, 1, -3 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 4.0f });
	scene.AddObject(std::move(mesh));

	auto sphere = std::make_unique<Sphere>(glm::vec3{ 1, -.5f, 0 }, 0.5f, std::make_shared<Lambertian>(color3_t{ 1 }));
	scene.AddObject(std::move(sphere));
	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(glm::vec3{ 0.7}));
	mesh->Load("models/cube-1.obj", glm::vec3{ -0.5, 0, -0.5 }, glm::vec3{ 0, 90, 0 }, glm::vec3{ 1.0f });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	mesh->Load("models/quad-1.obj", glm::vec3{ 0, 1, 1 }, glm::vec3{ 180, 0, 0 }, glm::vec3{ 4.0f });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 0.2f, 0.2f }));
	mesh->Load("models/quad-1.obj", glm::vec3{ -2, 1, -1 }, glm::vec3{ 0, 90, 0 }, glm::vec3{4.0f});
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0.2f, 1, 0.2f }));
	mesh->Load("models/quad-1.obj", glm::vec3{ 2, 1, -1 }, glm::vec3{ 0, 270, 0 }, glm::vec3{ 4.0f });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	mesh->Load("models/quad-1.obj", glm::vec3{ 0, -1, -1 }, glm::vec3{ 270, 0, 0 }, glm::vec3{ 4.0f });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	mesh->Load("models/quad-1.obj", glm::vec3{ 0, 3, -1 }, glm::vec3{ 90, 0, 0 }, glm::vec3{ 4.0f });
	scene.AddObject(std::move(mesh));

	mesh = std::make_unique<Mesh>(std::make_shared<Emissive>(glm::vec3{1, 1, 1}, 10.0f));
	mesh->Load("models/quad-1.obj", glm::vec3{ 0, 2.9, -1 }, glm::vec3{ 90, 0, 0 }, glm::vec3{ 1.0f });
	scene.AddObject(std::move(mesh));
}

void InitCornellBoxPlane(Scene& scene, const Canvas& canvas) {
	float aspectRatio = canvas.GetSize().x / canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 3 }, glm::vec3{ 0, 1, -1 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);
	scene.SetCamera(camera);
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 1, -.5f, 0 }, 0.5f, std::make_shared<Lambertian>(color3_t{ 1 }));
	scene.AddObject(std::move(sphere));
	auto mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(glm::vec3{ 1, 1, 1 }));
	mesh->Load("models/cube-1.obj", glm::vec3{ -0.5, 0, -0.5 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 1.0f });
	scene.AddObject(std::move(mesh));
	mesh = std::make_unique<Mesh>(std::make_shared<Emissive>(glm::vec3{ 1, 1, 1 }, 5.0f));
	mesh->Load("models/cube-1.obj", glm::vec3{ 0, 2.9, -1 }, glm::vec3{ 90, 0, 0 }, glm::vec3{ 1.0f });
	scene.AddObject(std::move(mesh));
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, 1, -3 }, glm::vec3{ 0, 0, 1 }, std::make_shared<Lambertian>(color3_t{ 1 }));
	scene.AddObject(std::move(plane));
	plane = std::make_unique<Plane>(glm::vec3{ 0, -1, -1 }, glm::vec3{ 0, 1, 0 }, std::make_shared<Lambertian>(color3_t{ 1 }));
	scene.AddObject(std::move(plane));
	plane = std::make_unique<Plane>(glm::vec3{ 0, 3, -1 }, glm::vec3{ 0, -1, 0 }, std::make_shared<Lambertian>(color3_t{ 1 }));
	scene.AddObject(std::move(plane));
	plane = std::make_unique<Plane>(glm::vec3{ -2, 1, -1 }, glm::vec3{ 1, 0, 0 }, std::make_shared<Lambertian>(color3_t{ 1, 0 ,0 }));
	scene.AddObject(std::move(plane));
	plane = std::make_unique<Plane>(glm::vec3{ 2, 1, -1 }, glm::vec3{ -1, 0, 0 }, std::make_shared<Lambertian>(color3_t{ 0, 1 ,0 }));
	scene.AddObject(std::move(plane));
}