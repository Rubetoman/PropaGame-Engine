#include "Texture.h"

Texture::Texture()
{
}


Texture::~Texture()
{
}

const char* Texture::getResizeMode() 
{
	switch (this->resize_mode) {
	case nearest: return "nearest"; break;
	case linear: return "linear"; break;
	}
}

const char* Texture::getWrapMode() 
{
	switch (this->wrap_mode) {
	case clamp_to_boder: return "clamp to boder"; break;
	case clamp: return "clamp"; break;
	case repeat: return "repeat"; break;
	case mirrored: return "mirrored"; break;
	}
}