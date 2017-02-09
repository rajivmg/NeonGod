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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	DrawRectangle(-0.5f, 0.5f, 0.5f, -0.5f);
}