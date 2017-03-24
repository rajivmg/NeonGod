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
	local_persist shader QuadShader;
	local_persist shader ColoredQuadShader;
	local_persist texture GuiTexture;
	local_persist b32 ShaderAndTextureSet = false;
	local_persist quad_batch GuiBatch = {};
	local_persist quad_batch ColoredQuadBatch = {};
	local_persist quad *TestQuad = {}; 
	local_persist quad_colored CQuad1 = {};
	local_persist quad_colored CQuad2 = {};
	local_persist quad_colored CQuad3 = {};

	if(!ShaderAndTextureSet)
	{
		InitQuadBatch(&GuiBatch, QUAD, MEGABYTE(8));

		read_file_result V1Src = ReadFile("quad_vert.glsl");
		read_file_result F1Src = ReadFile("quad_frag.glsl");
		CreateShader(&QuadShader, &V1Src, &F1Src);
		FreeFileMemory(&V1Src);
		FreeFileMemory(&F1Src);
		SetShader(&GuiBatch, &QuadShader);

	 	GuiTexture = LoadBMP_RGBA("nyan_face.bmp");
		SetTextureRGBA(&GuiBatch, &GuiTexture);

		ShaderAndTextureSet = true;

		InitQuadBatch(&ColoredQuadBatch, QUAD_COLORED, MEGABYTE(1));

		read_file_result V2Src = ReadFile("quad_colored_vert.glsl");
		read_file_result F2Src = ReadFile("quad_colored_frag.glsl");

		CreateShader(&ColoredQuadShader, &V2Src, &F2Src);
		FreeFileMemory(&V2Src);
		FreeFileMemory(&F2Src);
		SetShader(&ColoredQuadBatch, &ColoredQuadShader);

 		TestQuad = MakeQuad(Vector2(0, 0), Vector2((r32)GetWindowWidth(), (r32)GetWindowHeight()),
								Vector4(1.0f, 1.0f, 0.0f, 1.0f),
								Vector4(0.0f, 0.0f, 1.0f, 1.0f));
 		CQuad1 = MakeColoredQuad(Vector2(50, 50), Vector2(200, 200), Vector4(1.0f, 0.0f, 1.0f, 0.8f));
 		CQuad2 = MakeColoredQuad(Vector2(150, 50), Vector2(200, 200), Vector4(1.0f, 1.0f, 0.0f, 0.8f));
 		CQuad3 = MakeColoredQuad(Vector2(10, 510), Vector2(200, 200), Vector4(0.0f, 0.0f, 0.0f, 0.7f));

		Vector2 Top = Vector2(1.0f, 2.0f);
		Vector2 Bottom = Vector2(2.0f, 3.0f);
		Vector2 ResultS = Top + Bottom;
		Vector4 P = Vector4(-Top, -Bottom);
	}

	PushQuad(&GuiBatch, TestQuad);
	PushQuad(&ColoredQuadBatch, &CQuad1);
	PushQuad(&ColoredQuadBatch, &CQuad2);
	PushQuad(&ColoredQuadBatch, &CQuad3);
	
	DrawQuadBatch(&GuiBatch);
	DrawQuadBatch(&ColoredQuadBatch);
	/*=====  End of Game Render  ======*/
}
