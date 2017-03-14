#ifndef NEON_SDL_H
#define NEON_SDL_H

struct MainApp_t
{
	SDL_Window *Window;
	SDL_GLContext GLContext;
	SDL_Event WindowEvent;
	s32 WindowWidth;
	s32 WindowHeight;
	b32 ShouldQuit;
};

#endif
