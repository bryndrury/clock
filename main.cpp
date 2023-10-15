#include <iostream>
#include <chrono>
#include <SDL2/SDL.h>

#define fps 15
void cap_framerate(Uint32 starting_tick);
void drawHand(SDL_Renderer* renderer, float length, int screen_width, int screen_height, float tick, float angle, SDL_Color color);

int main(int argc, char* argv[]) {
	// Get time
	time_t now = time(0);
	tm *ltm = localtime(&now);
	float second = ltm->tm_sec;
	float minute = ltm->tm_min + (second / 60);
	float hour = ltm->tm_hour + (minute / 60);

	// Create Window and create some variables
	float screen_width = 200;
	float screen_height = 200;
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 10);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 100);

	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	window = SDL_CreateWindow("\"clock\"", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, (SDL_WINDOW_SHOWN, SDL_WINDOW_BORDERLESS ,SDL_WINDOW_OPENGL));
	if ( window == nullptr ) std::cerr << "Failed to apply video mode : " << SDL_GetError();
	renderer = SDL_CreateRenderer(window, -1, (SDL_RENDERER_ACCELERATED, SDL_RENDERER_PRESENTVSYNC));
	if ( renderer == nullptr ) std::cerr << "Could not create renderer!";

	// Render Black Screen and initialize colors
	SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
	SDL_RenderClear( renderer );
	SDL_Color red = {255, 0, 0, 255};
	SDL_Color white = {255, 255, 255, 255};

	// Initializing main loop
	SDL_Event event;
    Uint32 starting_tick;
	const auto startTime = std::chrono::steady_clock::now();
	std::chrono::duration<double> fElapsedTime = std::chrono::steady_clock::now() - startTime;
	bool running = true;
	float one_sixty = 1.0 / 60.0;


    while (running) {

		starting_tick = SDL_GetTicks();

		fElapsedTime = std::chrono::steady_clock::now() - startTime;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
				SDL_Quit();
				break;
			}
		}
		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		SDL_RenderClear( renderer );

		float tick = fElapsedTime.count() * 2 * 3.14159;
		float second_ang = 2 * 3.141593 * one_sixty * second;
		float minute_ang = 2 * 3.141593 * one_sixty * minute;
		float hour_ang = (2 * 3.141593 / 12) * hour;

		// second, minute, hour
		drawHand(renderer, 80, screen_width, screen_height, tick*one_sixty/(24), hour_ang, white);
		drawHand(renderer, 95, screen_width, screen_height, tick*one_sixty, minute_ang, white);
		drawHand(renderer, 100, screen_width, screen_height, tick, second_ang, red);
		
		SDL_RenderPresent(renderer);
		cap_framerate(starting_tick);
    }
	return 0;
}

void cap_framerate(Uint32 starting_tick)
{
	if ((1000 / fps) > SDL_GetTicks() - starting_tick)
	{
		SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
		// std::cout << "FPS: " << 1000 / (SDL_GetTicks() - starting_tick) << std::endl;
	}
}

void drawHand(SDL_Renderer* renderer, float length, int screen_width, int screen_height, float tick, float angle, SDL_Color color) {
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
	SDL_RenderDrawLine(renderer, 
						screen_width / 2, 
						screen_height / 2, 
						screen_width / 2 + (length * sin( (tick/60) + angle)), 
						screen_height / 2 + length * -cos( (tick/60) + angle));
}