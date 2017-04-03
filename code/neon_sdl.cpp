/*===============================================
=            NEON SDL Platform Layer            =
===============================================*/

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED 1
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "neon_platform.h"

#include "neon_sdl.h"

////
#include "neon_math.cpp"
#include "neon_renderer.cpp"
#include "neon_game.cpp"
#include "neon_texture_loader.cpp"
////

global_variable MainApp_t MainApp;

inline 
void GetWindowSize(s32 *Width, s32 *Height)
{
	SDL_GetWindowSize(MainApp.Window, &MainApp.WindowWidth, &MainApp.WindowHeight);
	*Width = MainApp.WindowWidth;
	*Height = MainApp.WindowHeight; 
}	

inline
s32 GetWindowWidth()
{
	SDL_GetWindowSize(MainApp.Window, &MainApp.WindowWidth, &MainApp.WindowHeight);
	return MainApp.WindowWidth;
}

inline
s32 GetWindowHeight()
{
	SDL_GetWindowSize(MainApp.Window, &MainApp.WindowWidth, &MainApp.WindowHeight);
	return MainApp.WindowHeight;
}

read_file_result ReadFile(const char *Filename)
{
	read_file_result Result = {};
	FILE *Fp;
	Fp = fopen(Filename, "rb");
	Assert(Fp != 0);
	if(Fp != 0)
	{
		fseek(Fp, 0, SEEK_END);
		long Temp = ftell(Fp);
		if(Temp != -1L && Temp >= 0)
		{
			Result.ContentSize = (u32)Temp;
		}
		else
		{
			//@TODO: LOG 
		}

		// Set the position indicator to the start of the file
		fseek(Fp, 0, SEEK_SET);

		// Allocate the required memory
		Result.Content = (void *)malloc(sizeof(char) * Result.ContentSize);
		if(Result.Content == 0)
		{
			//@TODO: LOG
		}

		// Read the file contents
		size_t ContentReadSize = fread(Result.Content, sizeof(char), Result.ContentSize, Fp);
		if(ContentReadSize == 0 || ContentReadSize != Result.ContentSize)
		{
			//@TODO: LOG
		}
		fclose(Fp);	
	}
	else
	{
		SDL_Log("File: %s not found or can not be opened.", Filename);
	}
	return Result;
}

void FreeFileMemory(read_file_result *ReadFileResult)
{
	free(ReadFileResult->Content);
}

void WriteFile(const char *Filename, u32 BytesToWrite, void *Content)
{
	FILE *Fp;
	Fp = fopen(Filename, "wb");
	Assert(Fp != 0);
	if(Fp != 0)
	{
		size_t BytesWritten = fwrite(Content, sizeof(u8), BytesToWrite, Fp);
		if(BytesWritten != (size_t)BytesToWrite)
		{
			//@TODO: LOG
		}
		fclose(Fp);
	}
	else
	{ 
		//@TODO: LOG
	}	
}

void SDLProcessKeyboardMessage(game_button_state *NewButtonState, bool IsDown)
{
	if(NewButtonState->EndedDown != IsDown)
	{
		NewButtonState->EndedDown = IsDown;
		++NewButtonState->HalfTransitionCount;
	}
}

void SDLProcessPendingEvents(SDL_Event *Event, game_controller_input *KeyboardController)
{
	switch(Event->type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			if(!Event->key.repeat)
			{
				switch(Event->key.keysym.sym)
				{
					case SDLK_w:
					{
						SDLProcessKeyboardMessage(&KeyboardController->Up, Event->key.state == SDL_PRESSED ? true : false);
						//printf("%s\n","DEBUG: KEYBOARD BUTTON PRESS!!");
					} break;
					
					case SDLK_a:
					{
						SDLProcessKeyboardMessage(&KeyboardController->Left, Event->key.state == SDL_PRESSED ? true : false);
					} break;

					case SDLK_s:
					{
						SDLProcessKeyboardMessage(&KeyboardController->Down, Event->key.state == SDL_PRESSED ? true : false);
					} break;
					
					case SDLK_d:
					{
						SDLProcessKeyboardMessage(&KeyboardController->Right, Event->key.state == SDL_PRESSED ? true : false);
					} break;

					case SDLK_ESCAPE:
					{
						SDL_Event QuitEvent;
						QuitEvent.type = SDL_QUIT;
						SDL_PushEvent(&QuitEvent);
					} break;	
				}
			}
		} break;
		
	}
}

int main(int argc, char **argv)
{

	if(SDL_Init(SDL_INIT_VIDEO) == 0)
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

		MainApp.Window = SDL_CreateWindow("Neon God",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED, 1280, 720,
										SDL_WINDOW_OPENGL);
		MainApp.WindowWidth = 1280;
		MainApp.WindowHeight = 720;
		if(MainApp.Window)
		{
			MainApp.GLContext = SDL_GL_CreateContext(MainApp.Window);
			if(MainApp.GLContext)
			{
				glewInit();
				printf("OpenGL Ver:%s\n", glGetString(GL_VERSION));
				// debug_read_file_result Info = DEBUGReadFile("test.cpp");
				// DEBUGWriteFile("test.copied", Info.ContentSize, Info.Content);
				// DEBUGFreeFileMemory(Info.Content);
				// debug_read_file_result VShaderSource = DEBUGReadFile("v.txt");
				// debug_read_file_result FShaderSource = DEBUGReadFile("f.txt");
				// renderer::shader Shader;
				// Shader.Sources(&VShaderSource, &FShaderSource);

				game_input Input[2] = {};
				game_input *NewInput = &Input[0];
				game_input *OldInput = &Input[1];
				
				InitRenderer();

				SDL_Event *Event = &MainApp.WindowEvent; 
				MainApp.ShouldQuit = false;
				while(!MainApp.ShouldQuit)
				{
					game_controller_input *OldKeyboardController = GetController(OldInput, 0);
					game_controller_input *NewKeyboardController = GetController(NewInput, 0);
//					*NewKeyboardController = (game_controller_input){};
					*NewKeyboardController = {};
					NewKeyboardController->IsConnected = true;
					
					for(int ButtonIndex = 0; ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
							++ButtonIndex)
					{
						NewKeyboardController->Buttons[ButtonIndex].EndedDown = 
							OldKeyboardController->Buttons[ButtonIndex].EndedDown;
					}

					while(SDL_PollEvent(Event))
					{
						if(Event->type == SDL_QUIT)
						{
							MainApp.ShouldQuit = true;
						}
						else
						{
							SDLProcessPendingEvents(Event, NewKeyboardController);
						}
					}

					GameUpdateAndRender(NewInput);
					SDL_GL_SwapWindow(MainApp.Window);

					game_input *temp = NewInput;
					NewInput = OldInput;
					OldInput = temp;	

				}
			}
			else
			{
				// logging
			}
		}
		else
		{
			// logging
		}

	}			
	else 
	{
		SDL_Log("%s\n", SDL_GetError());
	} 
	SDL_Quit();
	return 0;
}

/*=====  End of NEON SDL Platform Layer  ======*/