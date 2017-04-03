#ifndef NEON_PLATFORM_H
#define NEON_PLATFORM_H

#include <stdint.h>

#define local_persist static
#define global_variable static

#define Assert(Exp) if(!(Exp)) {*(volatile int *)0 = 0;}
#define ArrayCount(Array) (sizeof(Array)/sizeof((Array)[0]))

#define KILOBYTE(X) 1024LL * (X)
#define MEGABYTE(X) 1024LL * KILOBYTE(X)
#define GIGABYTE(X) 1024LL * MEGABYTE(X) // @CHECK: is LL required

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t  s16;
typedef int8_t   s8;
typedef s32		 b32;

typedef float 	 r32;
typedef double 	 r64;

//
struct read_file_result
{
	u32		ContentSize;
	void 	*Content;
};

read_file_result ReadFile(const char *Filename);
void FreeFileMemory(read_file_result *ReadFileResult);
void WriteFile(const char *Filename, u32 BytesToWrite, void *FileContent);

//
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

//
inline void GetWindowSize(u32 *Width, u32 *Height);
inline s32 GetWindowWidth();
inline s32 GetWindowHeight();

inline game_controller_input*
GetController(game_input *Input, u8 Index)
{
	Assert(Index < ArrayCount(Input->Controllers));
	return (&Input->Controllers[Index]);
}
#endif