/*===============================================
=            NEON SDL Platform Layer            =
===============================================*/

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED 1
#endif

#include <SDL2/SDL.h>

// #define NEON_INIT_GL
// #define NEON_DEBUG_GL
// #include "neon_GL.h"

#include "neon_platform.h"

#include <cstdio>

// #include "neon_renderer.h"
#include <Windows.h>

#include "neon_sdl.h"

platform_t *Platform;

PLATFORM_LOG(Log)
{
	va_list Arguments;
	va_start(Arguments, Fmt);
	
	switch(Level)
	{
		case INFO:
			SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, Fmt, Arguments);
			break;
		case WARN:
			SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, Fmt, Arguments);
			break;
		case ERR:
		{
			SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, Fmt, Arguments);
			char Temp[200]; 
			vsnprintf(Temp, 200, Fmt, Arguments);
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", Temp, 0);
			
			SDL_Event QuitEvent;
			QuitEvent.type = SDL_QUIT;
			SDL_PushEvent(&QuitEvent);
		}
			break;
		default:
			SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, Fmt, Arguments);
			break;
	}

	va_end(Arguments);
}

PLATFORM_READ_FILE(ReadFile)
{
	read_file_result Result = {};
	//FILE *Fp;
	SDL_RWops *Fp = SDL_RWFromFile(Filename, "rb");
	//Fp = fopen(Filename, "rb");
	Assert(Fp != 0);
	if(Fp != 0)
	{
		Platform->Log(INFO, "File: %s read sucessfully\n", Filename);
		//fseek(Fp, 0, SEEK_END);
		SDL_RWseek(Fp, 0, RW_SEEK_END);
		// long Temp = ftell(Fp);
		s64 Temp = (s64)SDL_RWtell(Fp);
		if(Temp != -1L && Temp >= 0)
		{
			Result.ContentSize = (u32)Temp;
		}
		else
		{
			Platform->Log(ERR, "File: %s size is zero.\n", Filename);
		}

		// Set the position indicator to the start of the file
		//fseek(Fp, 0, SEEK_SET);
		SDL_RWseek(Fp, 0, RW_SEEK_SET);
		// Allocate the required memory
		Result.Content = (void *)malloc(sizeof(char) * Result.ContentSize);
		if(Result.Content == 0)
		{
			Platform->Log(ERR, "Memory allocation to read %s failed.\n", Filename);
		}

		// Read the file contents
		// size_t ContentReadSize = fread(Result.Content, sizeof(char), Result.ContentSize, Fp);
		size_t ContentReadSize = SDL_RWread(Fp, Result.Content, sizeof(char), Result.ContentSize);
		if(ContentReadSize == 0 || ContentReadSize != Result.ContentSize)
		{
			Platform->Log(ERR, "File: %s is empty or read incompletely.\n", Filename);
		}
		// fclose(Fp);	
		SDL_RWclose(Fp);
	}
	else
	{
		Platform->Log(ERR, "File: %s not found or cannot be opened for read.\n", Filename);
	}
	return Result;
}

PLATFORM_FREE_FILE_MEMORY(FreeFileMemory)
{
	free(ReadFileResult->Content);
}

PLATFORM_WRITE_FILE(WriteFile)
{
	// FILE *Fp;
	// Fp = fopen(Filename, "wb");
	SDL_RWops *Fp = SDL_RWFromFile(Filename, "wb");
	Assert(Fp != 0);
	if(Fp != 0)
	{
		// size_t BytesWritten = fwrite(FileContent, sizeof(u8), BytesToWrite, Fp);
		size_t BytesWritten = SDL_RWwrite(Fp, FileContent, sizeof(u8), BytesToWrite);
		if(BytesWritten != (size_t)BytesToWrite)
		{
			Platform->Log(WARN, "File: %s cannot be written completely.\n", Filename);
		}
		// fclose(Fp);
		SDL_RWclose(Fp);
		Platform->Log(INFO, "File: %s written sucessfully\n", Filename);
	}
	else
	{ 
		Platform->Log(WARN, "File: %s cannot be opened for writing.\n");
	}	
}

void LoadGameCode(game_code *GameCode)
{	
	GameCode->Handle = SDL_LoadObject("neon_game.dll");
	
	if(GameCode->Handle == 0)
	{
		Log(ERR, SDL_GetError());
	}

	GameCode->GameUpdateAndRender = (game_update_and_render *)SDL_LoadFunction(GameCode->Handle, "GameUpdateAndRender");
	GameCode->GameCodeLoaded = (game_code_loaded *)SDL_LoadFunction(GameCode->Handle, "GameCodeLoaded");
}

void SDLProcessKeyboardMessage(game_button_state *NewButtonState, bool IsDown)
{
	if(NewButtonState->EndedDown != IsDown)
	{
		NewButtonState->EndedDown = IsDown;
		++NewButtonState->HalfTransitionCount;
	}
}

void SDLProcessEvents(SDL_Event *Event, game_input *KeyboardController)
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
						SDLProcessKeyboardMessage(&KeyboardController->W, Event->key.state == SDL_PRESSED ? true : false);
					} break;
					
					case SDLK_a:
					{
						SDLProcessKeyboardMessage(&KeyboardController->A, Event->key.state == SDL_PRESSED ? true : false);
					} break;

					case SDLK_s:
					{
						SDLProcessKeyboardMessage(&KeyboardController->S, Event->key.state == SDL_PRESSED ? true : false);
					} break;
					
					case SDLK_d:
					{
						SDLProcessKeyboardMessage(&KeyboardController->D, Event->key.state == SDL_PRESSED ? true : false);
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

		case SDL_WINDOWEVENT:
		{
			switch(Event->window.event)
			{
				case SDL_WINDOWEVENT_RESIZED:
				{
					Platform->Width = Event->window.data1;
					Platform->Height = Event->window.data2;
				} break;
			}

		} break;

	}
}

int main(int argc, char **argv)
{ 
	Platform = (platform_t *)malloc(sizeof(platform_t));
	Platform->Log = &Log;
	Platform->Width = 1280;
	Platform->Height = 720;

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

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

		SDL_Window 		*Window;
		Window 		= SDL_CreateWindow("Neon God",
										SDL_WINDOWPOS_UNDEFINED,
										SDL_WINDOWPOS_UNDEFINED, 1280, 720,
										SDL_WINDOW_OPENGL); // | SDL_WINDOW_FULLSCREEN);

		Platform->ReadFile = &ReadFile;
		Platform->WriteFile = &WriteFile;
		Platform->FreeFileMemory = &FreeFileMemory;

		if(Window)
		{
			SDL_GLContext GLContext = SDL_GL_CreateContext(Window);

			if(GLContext)
			{

				game_code GameCode = {};
				
				LoadGameCode(&GameCode);
				GameCode.GameCodeLoaded(Platform);

				game_input Input[2] = {};
				game_input *NewInput = &Input[0];
				game_input *OldInput = &Input[1];

				SDL_Event Event;
				bool ShouldQuit = false;

				r64 FrameTime = 0;
				
				u64 PrevCounter, CurrentCounter, CounterFrequency;
				CounterFrequency = SDL_GetPerformanceFrequency();

				PrevCounter = SDL_GetPerformanceCounter();

				while(!ShouldQuit)
				{
					game_input *OldKeyboardController = OldInput;
					game_input *NewKeyboardController = NewInput;

					*NewKeyboardController = {};
					
					for(int ButtonIndex = 0; ButtonIndex < ArrayCount(NewKeyboardController->Buttons);
							++ButtonIndex)
					{
						NewKeyboardController->Buttons[ButtonIndex].EndedDown = 
							OldKeyboardController->Buttons[ButtonIndex].EndedDown;
					}

					while(SDL_PollEvent(&Event))
					{
						if(Event.type == SDL_QUIT)
						{
							ShouldQuit = true;
						}
						else
						{
							SDLProcessEvents(&Event, NewKeyboardController);
						}
					}
					
					PrevCounter = SDL_GetPerformanceCounter();
					GameCode.GameUpdateAndRender(NewInput, FrameTime);
					SDL_GL_SwapWindow(Window);
					
					game_input *temp = NewInput;
					NewInput = OldInput;
					OldInput = temp;	

					CurrentCounter = SDL_GetPerformanceCounter();
					FrameTime = (r64)((CurrentCounter - PrevCounter)*1000.0) / CounterFrequency;
					char DebugCountString[100];
					sprintf(DebugCountString, "%f ms\n", FrameTime);
					PrevCounter = CurrentCounter;
					OutputDebugString(DebugCountString);
					// Platform->Log(INFO, DebugCountString);
				}
			}
			else
			{
				Platform->Log(ERR, "Failed to create OpenGL context.\n");
			}
		}
		else
		{
			Platform->Log(ERR, "Failed to create a window.\n");
		}

	}			
	else 
	{
		Platform->Log(ERR, "%s\n", SDL_GetError());
	} 
	SDL_Quit();
	return 0;
}
