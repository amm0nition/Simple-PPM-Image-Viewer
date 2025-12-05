#include <iostream>
#include <SDL3\SDL.h> //Inspiration was done in SDL2, but I'm dumb so let's force it with SDL3 lmao
#include <fstream>
#include <string>
#include <vector>

void drawWindow(SDL_Surface* surface,
	const SDL_PixelFormatDetails* fmt,
	const std::vector<unsigned char>& pixels,
	int width, int height, // original image size
	int new_width, int new_height) // current window size
{
	SDL_Rect pixel{ 0, 0, 1, 1 };

	
	for (int y = 0; y < new_height; y++) {
		for (int x = 0; x < new_width; x++) {

			// nearest-neighbor image scaling
			int src_x = (x * width) / new_width;
			int src_y = (y * height) / new_height;

			int idx = (src_y * width + src_x) * 3;

			unsigned char r = pixels[idx];
			unsigned char g = pixels[idx + 1];
			unsigned char b = pixels[idx + 2];

			Uint32 color = SDL_MapRGB(fmt, NULL, r, g, b);

			pixel.x = x;
			pixel.y = y;
			SDL_FillSurfaceRect(surface, &pixel, color);
		}
	}
}

int main() {

	int app_running = 1;

	std::string filedir{};
	std::cout << "Enter the file path: ";
	std::getline(std::cin, filedir);

	std::ifstream in(filedir, std::ios::binary); //Load the image file in binary
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

	std::vector<unsigned char> pixels(width * height * 3);
	in.read((char*)pixels.data(), pixels.size());
	in.close();  // done with the file

	std::cout << "PPM Image Viewer!\n";
	std::cout << "Image Width:" << width << "\n";
	std::cout << "Image Height:" << height << "\n";

	int original_width = width;
	int original_height = height;

	SDL_Window* window = SDL_CreateWindow("PPM Image Viewer", width, height, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surface->format);
	
	drawWindow(surface, fmt, pixels, original_width, original_height, width, height);

	while (app_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) { 
			if (event.type == SDL_EVENT_QUIT) {
				app_running = 0;
			}
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_I) {

				width /= 1.2f;
				height /= 1.2f;
				if (width < 1) width = 1;
				if (height < 1) height = 1;
				
				SDL_SetWindowSize(window, width, height); // resize the actual window
				// update surface and format
				surface = SDL_GetWindowSurface(window);
				fmt = SDL_GetPixelFormatDetails(surface->format);
				drawWindow(surface, fmt, pixels, original_width, original_height, width, height); 
				system("cls"); // Windows only
				std::cout << "Window size shrinked\n";
				std::cout << "Image Width:" << width << "\n";
				std::cout << "Image Height:" << height << "\n";
			}
			if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_O) {
				width *= 1.2f;
				height *= 1.2f;
				if (width < 1) width = 1;
				if (height < 1) height = 1;

				SDL_SetWindowSize(window, width, height); // resize the actual window
				// update surface and format
				surface = SDL_GetWindowSurface(window);
				fmt = SDL_GetPixelFormatDetails(surface->format);
				drawWindow(surface, fmt, pixels, original_width, original_height, width, height);
				system("cls"); // Windows only
				std::cout << "Window size enlarged\n";
				std::cout << "Image Width:" << width << "\n";
				std::cout << "Image Height:" << height << "\n";
			}
		}
		SDL_UpdateWindowSurface(window);
		SDL_Delay(100);
	}
	
	return 0;
}
