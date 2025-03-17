#ifndef ANDROMEDA
#define ANDROMEDA

#include "macros.h"
#include "image.h"
#include "util.h"
#include "audio.h"
#include "media.h"
#include "font.h"

namespace andromeda {
	extern bool use_opengl;
	extern bool use_openal;

	bool _load_opengl();
}

#endif // ANDROMEDA
