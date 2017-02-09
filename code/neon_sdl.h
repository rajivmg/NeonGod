#ifndef NEON_SDL_H
#define NEON_SDL_H

struct MainApp_t
{
	SDL_Window *Window;
	SDL_GLContext GLContext;
	SDL_Event WindowEvent;
	uint32_t WindowWidth;
	uint32_t WindowHeight;
	bool ShouldQuit;
};

#endif
