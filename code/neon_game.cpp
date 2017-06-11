#include "neon_game.h"
#include "neon_renderer.h"
#include "neon_texture.h"

void GameUpdateAndRender(game_input *Input)
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
				Platform->Log(INFO,"Go Up!");
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
	local_persist quad TestQuad = {}; 
	local_persist quad_colored CQuad1 = {};
	local_persist quad_colored CQuad2 = {};
	local_persist quad_colored CQuad3 = {};
	local_persist text_batch TBatch;

	if(!ShaderAndTextureSet)
	{
		InitQuadBatch(&GuiBatch, QUAD, MEGABYTE(1));

		read_file_result V1Src = Platform->ReadFile("quad_vert.glsl");
		read_file_result F1Src = Platform->ReadFile("quad_frag.glsl");
		
		CreateShader(&QuadShader, &V1Src, &F1Src);

		Platform->FreeFileMemory(&V1Src);
		Platform->FreeFileMemory(&F1Src);

		SetBatchShader(&GuiBatch, &QuadShader);

	 	// GuiTexture = LoadTextureFromFile("debug_art.tga");
	 	GuiTexture.LoadFromFile("debug_art.tga");
		SetBatchTexture(&GuiBatch, &GuiTexture);
		//FreeTextureMemory(GuiTexture);
		GuiTexture.FreeMemory();

		ShaderAndTextureSet = true;

		InitQuadBatch(&ColoredQuadBatch, QUAD_COLORED, MEGABYTE(1));

		read_file_result V2Src = Platform->ReadFile("quad_colored_vert.glsl");
		read_file_result F2Src = Platform->ReadFile("quad_colored_frag.glsl");

		CreateShader(&ColoredQuadShader, &V2Src, &F2Src);
		Platform->FreeFileMemory(&V2Src);
		Platform->FreeFileMemory(&F2Src);
		SetBatchShader(&ColoredQuadBatch, &ColoredQuadShader);

 		TestQuad = Quad(vec2(0, 0), vec2((r32)Platform->Width, (r32)Platform->Height),
						vec4(0.0f, 0.0f, 1.0f, 1.0f));
 		CQuad1 = ColorQuad(vec2(50, 50), vec2(200, 200), vec4(1.0f, 0.0f, 1.0f, 0.8f));
 		CQuad2 = ColorQuad(vec2(150, 50), vec2(200, 200), vec4(1.0f, 1.0f, 0.0f, 0.8f));
 		CQuad3 = ColorQuad(vec2(10, 510), vec2(200, 200), vec4(0.0f, 0.0f, 0.0f, 0.7f));
 		
 		InitTextBatch(&TBatch, "Inconsolata-Regular.ttf", 32, 2);

		vec2 Top = vec2(1.0f, 2.0f);
		vec2 Bottom = vec2(2.0f, 3.0f);
		vec2 ResultS = Top + Bottom;
		vec4 P = vec4(-Top, -Bottom);
	}

	PushQuad(&GuiBatch, &TestQuad);
	PushQuad(&ColoredQuadBatch, &CQuad1);
	PushQuad(&ColoredQuadBatch, &CQuad2);
	PushQuad(&ColoredQuadBatch, &CQuad3);
	
	DrawQuadBatch(&GuiBatch);
	DrawQuadBatch(&ColoredQuadBatch);

	DrawGUIText(&TBatch, "R", 10, 10);
	/*=====  End of Game Render  ======*/
}
