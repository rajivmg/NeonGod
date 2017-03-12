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
	
	renderer::BackBufferFlush();
	
	/*===================================
	=            Game Render            =
	===================================*/
	local_persist shader_program Shader;

	local_persist rect_batch RectBatch= {};

	local_persist b32 IsShaderCreated = 0;

	local_persist rect *Shape = MakeRectangle(Vector2(590, 310),
						  Vector2(100, 100),
						  Vector4(0.2f, 0.2f, 0.2f, 1.0f),
						  Vector4(0, 0, 1, 1));
	
	if(!IsShaderCreated)
	{

		texture Art = LoadBMP_32_RGBA("NyanFaceA.bmp");
		RectBatch.SetTextureMap(&Art);
		FreeTextureMemory(&Art);

		read_file_result VShaderSource = ReadFile("v.txt");
		read_file_result FShaderSource = ReadFile("f.txt");
		Shader.Sources(&VShaderSource, &FShaderSource);
		IsShaderCreated = 1;

		Vector2 Top = Vector2(1.0f, 2.0f);
		Vector2 Bottom = Vector2(2.0f, 3.0f);
		Vector2 ResultS = Top + Bottom;
		Vector4 P = Vector4(-Top, -Bottom);
	}

	RectBatch.SetShaderProgram(Shader);

	BatchBind(&RectBatch);
	
	BatchPushContent(Shape);
	
	BatchDraw(&RectBatch);
	
	BatchUnbind(&RectBatch);

	// renderer::BindBatch()

	// renderer::PushIntoBatch()

	// renderer::DrawBatch()


	renderer::RectBufferFlush();
	/*=====  End of Game Render  ======*/
}
