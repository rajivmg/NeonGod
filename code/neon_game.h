#ifndef NEON_GAME_H
#define NEON_GAME_H

#define Assert(Exp) if(!(Exp)) {*(volatile int *)0 = 0;}
#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))

#define KILOBYTE(X) 1024LL * (X)
#define MEGABYTE(X) 1024LL * KILOBYTE(X)
#define GIGABYTE(X) 1024LL * MEGABYTE(X) // @CHECK: is LL required

#if INTERNAL_BUILD

struct debug_read_file_result
{
	uint32_t ContentSize;
	void *Content;
};

internal debug_read_file_result DEBUGReadFile(const char *Filename);
internal void DEBUGFreeFileMemory(void *FileMemory);
internal void DEBUGWriteFile(const char *Filename, uint32_t BytesToWrite, void *FileContent);

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
GetController(game_input *Input, uint8_t Index)
{
	Assert(Index < ArrayCount(Input->Controllers));
	return (&Input->Controllers[Index]);
}

#endif