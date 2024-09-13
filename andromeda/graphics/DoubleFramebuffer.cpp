#include "DoubleFramebuffer.hpp"

using namespace andromeda::graphics;

void DoubleFramebuffer::alloc(bool try_again)
{
	back_frame_buffer.alloc(try_again);
}

