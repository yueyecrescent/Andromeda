#ifndef ANDROMEDA
#define ANDROMEDA

#include "macros/macros.h"
#include "image/image.h"
#include "util/util.h"
#include "audio/audio.h"
#include "media/media.h"
#include "font/font.h"

namespace andromeda {
	extern bool use_opengl;
	extern bool use_openal;

	bool _load_opengl();
}

#endif // ANDROMEDA
