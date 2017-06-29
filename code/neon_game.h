#ifndef NEON_GAME_H
#define NEON_GAME_H

#include "neon_platform.h"

#include "neon_math.cpp"
#include "neon_renderer.cpp"
#include "neon_texture.cpp"

#include "neon_renderer.h"
#include "neon_math.h"
#include "neon_texture.h"

struct game_resources
{
	text_batch DebugUITextBatch = {};
	line_batch DebugLineBatch = {};
}GameResources;

void InitGameResources();

platform_t *Platform;

#endif