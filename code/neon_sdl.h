#ifndef NEON_SDL_H
#define NEON_SDL_H

struct MainApp_t
{
	SDL_Window *Window;
	SDL_GLContext GLContext;
	SDL_Event WindowEvent;
	u32 WindowWidth;
	u32 WindowHeight;
	b32 ShouldQuit;
};

#endif
