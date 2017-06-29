#include "neon_game.h"

extern "C"
GAME_CODE_LOADED(GameCodeLoaded)
{
	Platform = aPlatform;
	InitRenderer();
}

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
	/*===================================
	=            Game Update            =
	===================================*/
	
	// @TODO: Add support for 2nd analog controller

	if(Input->W.EndedDown && Input->W.HalfTransitionCount == 1)
	{
		Platform->Log(INFO,"Go Up!");
			// SDL_SetWindowFullscreen(Platform->Window, SDL_WINDOW_FULLSCREEN);
	}

	
	//==================================//
	
	ClearBackBuffer();
	
	/*===================================
	=            Game Render            =
	===================================*/
	local_persist shader QuadShader;
	local_persist shader ColoredQuadShader;
	local_persist texture EngineLogo;
	local_persist b32 ShaderAndTextureSet = false;
	local_persist quad_batch TestQuadBatch = {};
	local_persist texture_quad EngineLogoQuad = {}; 
	local_persist color_quad CQuad1 = {};
	local_persist text_batch DebugUITextBatch = {};
	local_persist line_batch DebugLineBatch = {};

	if(!ShaderAndTextureSet)
	{
		TestQuadBatch.Init(TEXTURE_QUAD, MEGABYTE(1));
		read_file_result V1Src = Platform->ReadFile("texture_quad.vs");
		read_file_result F1Src = Platform->ReadFile("texture_quad.fs");
		
		QuadShader.CreateProgram(&V1Src, &F1Src);

		Platform->FreeFileMemory(&V1Src);
		Platform->FreeFileMemory(&F1Src);

		TestQuadBatch.SetShader(&QuadShader);

	 	EngineLogo.LoadFromFile("vortex.tga");
		TestQuadBatch.SetTexture(&EngineLogo);
		EngineLogo.FreeMemory();


		DebugLineBatch.Init(KILOBYTE(64));

		read_file_result V2Src = Platform->ReadFile("color_quad.vs");
		read_file_result F2Src = Platform->ReadFile("color_quad.fs");
		ColoredQuadShader.CreateProgram(&V2Src, &F2Src);
		Platform->FreeFileMemory(&V2Src);
		Platform->FreeFileMemory(&F2Src);

		DebugLineBatch.SetShader(&ColoredQuadShader);

 		EngineLogoQuad = TextureQuad(vec2(608, 296), vec2(64, 64));

 		DebugUITextBatch.SetFont(RenderResources.HUDFont, KILOBYTE(32));
 		

		vec2 Top = vec2(1.0f, 2.0f);
		vec2 Bottom = vec2(2.0f, 3.0f);
		r32 DotProduct = Top.Dot(Bottom);
		vec2 ResultS = Top + Bottom;
		vec4 P = vec4(-Top, -Bottom);

		ShaderAndTextureSet = true;
	}

	TestQuadBatch.PushQuad(&EngineLogoQuad);

	// vec3 Start, End;
	// Start.X = 0.0f;
	// Start.Y = Platform->Height;
	// Start.Z = 0.0f;

	// End.X = Start.X + 500 * cos((3.14/180) * 315);
	// End.Y = Start.Y + 500 * sin((3.14/180) * 315);
	// End.Z = 0.0f;
	// DebugLineBatch.PushLine(Start, End, vec4(1.0f, 0.0f, 1.0f, 1.0f));
	DebugLineBatch.PushLine(vec3(0, 0, 0), 3.14f/4, 500, vec4(1.0f, 1.0f, 0.0f, 1.0f));

	DebugLineBatch.PushLine(vec3(0, 40, 0), vec3(Platform->Width, 40, 0), vec4(1.0f, 0.0f, 1.0f, 1.0f));
	DebugLineBatch.PushLine(vec3(40, 0, 0), vec3(40, Platform->Height, 0), vec4(1.0f, 0.0f, 1.0f, 1.0f));

	local_persist int Counter = 0;
	if(Counter == 15)
	{	
		DebugUITextBatch.Flush();
		DebugUITextBatch.PushText(vec2(10, Platform->Height - 40), vec4(1.0f, 1.0f, 1.0f, 1.0f),
				"Frame time(ms): %.2f\nA quick brown fox jumps over the lazy dog.", FrameTime);
		Counter = 0;
	}
	Counter++;

	TestQuadBatch.Draw();
	DebugLineBatch.Draw();
	DebugUITextBatch.Draw();
}
