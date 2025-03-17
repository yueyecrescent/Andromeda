#ifndef ANDROMEDA
#define ANDROMEDA

#include "app.h"
#include "audio.h"
#include "graphics.h"
#include "io.h"
#include "math.h"
#include "media.h"
#include "thread.h"
#include "traits.h"
#include "util.h"

namespace andromeda {
	extern bool use_opengl;
	extern bool use_openal;

	bool _load_opengl();
}

#endif // ANDROMEDA
