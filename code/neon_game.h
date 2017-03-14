#ifndef NEON_GAME_H
#define NEON_GAME_H

struct game_button_state
{
	int HalfTransitionCount;
	bool EndedDown;
};

struct game_controller_input
{
	bool IsConnected;
	union
	{
		game_button_state Buttons[4];
		struct
		{
			game_button_state Up;
			game_button_state Down;
			game_button_state Left;
			game_button_state Right;
		};
	};
};

#define MAX_CONTROLLER_COUNT 1
struct game_input
{
	game_controller_input Controllers[MAX_CONTROLLER_COUNT];
};

inline game_controller_input*
GetController(game_input *Input, u8 Index)
{
	Assert(Index < ArrayCount(Input->Controllers));
	return (&Input->Controllers[Index]);
}

#endif