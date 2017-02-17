#include "neon_game.h"
#include "neon_renderer.h"
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
		debug_read_file_result VShaderSource = DEBUGReadFile("v.txt");
		debug_read_file_result FShaderSource = DEBUGReadFile("f.txt");

		Shader.Sources(&VShaderSource, &FShaderSource);
		IsShaderCreated = 1;
	}


	rect TestRect = {};
	
	GLfloat Vertex[18] = {-0.5f, 0.5f, 0.0f,
					   -0.5f, -0.5f, 0.0f,
						0.5f, 0.5f, 0.0f,

						-0.5f, -0.5f, 0.0f,
						0.5f, -0.5f, 0.0f,
						0.5f, 0.5, 0.0f};
	memcpy(TestRect.Vertex, Vertex, sizeof(Vertex));

	// rect TestRect1 = {};
	
	// GLfloat Vertex1[18] = {-0.9f, 0.5f, 0.0f,
	// 				   -0.5f, -0.5f, 0.0f,
	// 					0.5f, 0.5f, 0.0f,

	// 					-0.5f, -0.5f, 0.0f,
	// 					0.5f, -0.5f, 0.0f,
	// 					0.5f, 0.5, 0.0f};
	// memcpy(TestRect1.Vertex, Vertex1, sizeof(Vertex));

	RectRenderer.PushRect(&TestRect);
	//RectRenderer.PushRect(&TestRect1);
	RectRenderer.SetShaderProgram(Shader);
	RectRenderer.Draw();

	glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	
	/*=====  End of Game Render  ======*/
	
	//DrawRectangle(-0.5f, 0.5f, 0.5f, -0.5f);
}
