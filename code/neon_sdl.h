#ifndef NEON_SDL_H
#define NEON_SDL_H

enum log_level
{
	INFO,
	WARN,
	ERR
};

#define PLATFORM_READ_FILE(Name) read_file_result (Name)(char const *Filename)
typedef PLATFORM_READ_FILE(platform_read_file);
#define PLATFORM_WRITE_FILE(Name) void (Name)(char const *Filename, u32 BytesToWrite, void *FileContent)
typedef PLATFORM_WRITE_FILE(platform_write_file);
#define PLATFORM_FREE_FILE_MEMORY(Name) void (Name)(read_file_result *ReadFileResult)
typedef PLATFORM_FREE_FILE_MEMORY(platform_free_file_memory);
#define PLATFORM_LOG(Name) void (Name)(log_level Level, char const * Fmt, ...)
typedef PLATFORM_LOG(platform_log);

struct platform_t
{
	SDL_Window 		*Window;
	SDL_GLContext	GLContext;
	SDL_Event		Event;
	s32				Width;
	s32				Height;
	b32				ShouldQuit;

	platform_read_file *ReadFile;
	platform_write_file *WriteFile;
	platform_free_file_memory *FreeFileMemory;
	platform_log *Log;
};

#endif
