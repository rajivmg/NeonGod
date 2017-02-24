#include "neon_game.h"
#include "neon_renderer.h"
#include "neon_texture_loader.h"
#include <GL/glew.h>

internal void
DrawRectangle(float TopLeftX, float TopLeftY, float BottomRightX, float BottomRightY)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	   glVertex3f(TopLeftX, TopLeftY, 0.0f);
	   glVertex3f(-BottomRightX, BottomRightY, 0.0f);
	   glVertex3f(BottomRightX, -BottomRightY, 0.0f);

	   glVertex3f(-BottomRightX, BottomRightY, 0.0f);
	   glVertex3f(BottomRightX, BottomRightY, 0.0f);
	   glVertex3f(BottomRightX, -BottomRightY, 0.0f);
	glEnd();
}

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
	
	renderer::ClearWindowBuffers();
	
	/*===================================
	=            Game Render            =
	===================================*/
	
	local_persist shader_program Shader;
	local_persist rect_renderer RectRenderer= {};
	local_persist b32 IsShaderCreated = 0;
	
	if(!IsShaderCreated)
	{
		LoadBMP("sails.bmp");

		read_file_result VShaderSource = ReadFile("v.txt");
		read_file_result FShaderSource = ReadFile("f.txt");

		Shader.Sources(&VShaderSource, &FShaderSource);
		IsShaderCreated = 1;
	}


	rect TestRect = {};
	
	GLfloat Content1[36] = {-0.5f,  0.5f, 0.0f,
							1.0f, 0.0f, 0.0f, // C
					      -0.5f, -0.5f, 0.0f,
					      1.0f, 0.0f, 0.0f, // C
						   0.5f,  0.5f, 0.0f,
						   1.0f, 0.0f, 0.0f, //C

						  -0.5f, -0.5f, 0.0f,
						  1.0f, 0.0f, 0.0f, // C
						   0.5f, -0.5f, 0.0f,
						   1.0f, 0.0f, 0.0f, //C
						   0.5f,  0.5f, 0.0f, 
							1.0f, 0.0f, 0.0f, //C 
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
