#include "neon_game.h"
#include "neon_renderer.h"
#include "neon_texture_loader.h"
#include <GL/glew.h>

internal void 
GameUpdateAndRender(game_input *Input)
{
	/*===================================
	=            Game Update            =
	===================================*/
	
	// @TODO: Add support for 2nd analog controller
	for(uint8_t ControllerIndex = 0; ControllerIndex < ArrayCount(Input->Controllers); ++ControllerIndex)
	{
		game_controller_input *Controller = GetController(Input, ControllerIndex);

		if(Controller->IsConnected)
		{
			if(Controller->Up.EndedDown && Controller->Up.HalfTransitionCount == 1)
			{
				SDL_Log("Go Up!");
			}
		}
	}
	
	/*=====  End of Game Update  ======*/
	
	renderer::ClearBackBuffer();
	
	/*===================================
	=            Game Render            =
	===================================*/
	local_persist shader_program Shader;
	local_persist rectangle_renderer RectRenderer= {};
	local_persist b32 IsShaderCreated = 0;
	
	if(!IsShaderCreated)
	{
		texture SailsTexture = LoadBMP_32_RGBA("art.bmp");
		RectRenderer.SetTextureMap(&SailsTexture);
		FreeTextureMemory(&SailsTexture);

		read_file_result VShaderSource = ReadFile("v.txt");
		read_file_result FShaderSource = ReadFile("f.txt");

		Shader.Sources(&VShaderSource, &FShaderSource);
		IsShaderCreated = 1;

		Vector2 Top = Vector2(1.0f, 2.0f);
		Vector2 Bottom = Vector2(2.0f, 3.0f);
		Vector2 ResultS = Top + Bottom;
	}


	rectangle TestRect = {};
	
	GLfloat Content1[48] = {-0.5f,  0.5f, 0.0f,
							1.0f, 0.0f, 0.0f, // C
							0.0f, 1.0f, // T
					      -0.5f, -0.5f, 0.0f,
					      1.0f, 0.0f, 0.0f, // C
					      	0.0f, 0.0f, //T
						   0.5f,  0.5f, 0.0f,
						   1.0f, 0.0f, 0.0f, //C
						   1.0f, 1.0f, //T

						  -0.5f, -0.5f, 0.0f,
						  1.0f, 0.0f, 0.0f, // C
						  	0.0f, 0.0f, // T
						   0.5f, -0.5f, 0.0f,
						   1.0f, 0.0f, 0.0f, //C
						   1.0f, 0.0f, // T
						   0.5f,  0.5f, 0.0f, 
							1.0f, 0.0f, 0.0f, //C
							1.0f, 1.0f
							};

	memcpy(TestRect.Content, Content1, sizeof(Content1));

	// rect TestRect2 = {};

	// GLfloat Content2[36] = {-0.8f,  0.8f, 0.0f,
	// 						0.0f, 1.0f, 0.0f, // C
	// 				      -0.8f, -0.8f, 0.0f,
	// 				      0.0f, 1.0f, 0.0f, // C
	// 					   0.8f,  0.8f, 0.0f,
	// 					   0.0f, 1.0f, 0.0f, //C

	// 					  -0.8f, -0.8f, 0.0f,
	// 					  0.0f, 1.0f, 0.0f, // C
	// 					   0.8f, -0.8f, 0.0f,
	// 					   0.0f, 1.0f, 0.0f, //C
	// 					   0.8f,  0.8f, 0.0f, 
	// 						0.0f, 1.0f, 0.0f, //C 
	// 						};

	// memcpy(TestRect2.Content, Content2, sizeof(Content2));
	//RectRenderer.PushRect(&TestRect2);
	RectRenderer.PushRect(&TestRect);
	RectRenderer.SetShaderProgram(Shader);
	RectRenderer.Draw();
	
	/*=====  End of Game Render  ======*/
	
	//DrawRectangle(-0.5f, 0.5f, 0.5f, -0.5f);
}
