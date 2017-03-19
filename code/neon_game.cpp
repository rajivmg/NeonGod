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
	
	BackBufferFlush();
	
	/*===================================
	=            Game Render            =
	===================================*/
	local_persist shader Lvl1Shader;
	local_persist texture Lvl1Ground;
	local_persist b32 ShaderAndTextureSet = false;
	local_persist quad_batch Level1 = {};
	local_persist quad *GroundQuad = {}; 

	if(!ShaderAndTextureSet)
	{
		InitQuadBatch(&Level1, MEGABYTE(8));

		read_file_result VShaderSrc = ReadFile("gen_vert.glsl");
		read_file_result FShaderSrc = ReadFile("gen_frag.glsl");
		CreateShader(&Lvl1Shader, &VShaderSrc, &FShaderSrc);
		FreeFileMemory(&VShaderSrc);
		FreeFileMemory(&FShaderSrc);
		SetShader(&Level1, &Lvl1Shader);

		Lvl1Ground = LoadBMP_RGBA("level1_ground.bmp");
		SetTextureRGBA(&Level1, &Lvl1Ground);

		ShaderAndTextureSet = true;

 		GroundQuad = MakeQuad(Vector2(0, 0), Vector2((r32)GetWindowWidth(), (r32)GetWindowHeight()),
								Vector4(1.0f, 1.0f, 0.0f, 1.0f),
								Vector4(0.0f, 0.0f, 1.0f, 1.0f));

		Vector2 Top = Vector2(1.0f, 2.0f);
		Vector2 Bottom = Vector2(2.0f, 3.0f);
		Vector2 ResultS = Top + Bottom;
		Vector4 P = Vector4(-Top, -Bottom);
	}

	PushQuad(&Level1, GroundQuad);
	
	DrawQuadBatch(&Level1);

	/*=====  End of Game Render  ======*/
}
