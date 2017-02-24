#ifndef NEON_GAME_H
#define NEON_GAME_H

#define Assert(Exp) if(!(Exp)) {*(volatile int *)0 = 0;}
#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))

#define KILOBYTE(X) 1024LL * (X)
#define MEGABYTE(X) 1024LL * KILOBYTE(X)
#define GIGABYTE(X) 1024LL * MEGABYTE(X) // @CHECK: is LL required

#if INTERNAL_BUILD

struct read_file_result
{
	u32		ContentSize;
	void 	*Content;
};

internal read_file_result ReadFile(const char *Filename);
internal void FreeFileMemory(void *FileMemory);
internal void WriteFile(const char *Filename, u32 BytesToWrite, void *FileContent);

#endif

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