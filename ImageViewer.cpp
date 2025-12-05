#include <iostream>
#include <SDL3\SDL.h> //Inspiration was done in SDL2, but I'm dumb so let's force it with SDL3 lmao
#include <fstream>
#include <string>

int main() {

	int app_running = 1;

	std::ifstream in("image.ppm", std::ios::binary); //Load the image file in binary
	if (!in.is_open()) {
		std::cerr << "File not found\n";
		return 1;
	}

	// Read First Line (Type)
	std::string line;
	std::getline(in, line);
	// Read Second Line (Dimension)
	int width, height;
	in >> width >> height;
	// Read Max Value (not needed)
	int maxval;
	in >> maxval;

	in.get();

	std::cout << "PPM Image Viewer!\n";
	std::cout << "Image Width:" << width << "\n";
	std::cout << "Image Height:" << height << "\n";

	SDL_Window* window = SDL_CreateWindow("PPM Image Viewer", width, height, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surface->format);

	int x{};
	int y{};

	SDL_Rect pixel{x, y, 1, 1};
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			unsigned char r = in.get();
			unsigned char g = in.get();
			unsigned char b = in.get();
			Uint32 color = SDL_MapRGB(fmt, NULL, r, g, b);
			pixel.x = x;
			pixel.y = y;
			SDL_FillSurfaceRect(surface, &pixel, color);
		}
	}

	while (app_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) { 
			if (event.type == SDL_EVENT_QUIT) {
				app_running = 0;
			}
		}
		SDL_UpdateWindowSurface(window);
		SDL_Delay(100);
	}
	
	return 0;
}
