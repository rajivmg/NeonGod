#ifndef NEON_PLATFORM_H
#define NEON_PLATFORM_H

#include <stdint.h>

#define internal static
#define local_persist static
#define global_persist static

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

struct read_file_result
{
	u32		ContentSize;
	void 	*Content;
};

internal read_file_result ReadFile(const char *Filename);
internal void FreeFileMemory(void *FileMemory);
internal void WriteFile(const char *Filename, u32 BytesToWrite, void *FileContent);

inline void GetWindowSize(u32 *Width, u32 *Height);
inline s32 GetWindowWidth();
inline s32 GetWindowHeight();

#endif