#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#define NAME_SIZE 50
#define PATH_SIZE 250
#define EXTEN_SIZE 4

enum Resize_Mode { nearest, linear };
enum Wrap_Mode { clamp_to_boder, clamp, repeat, mirrored };

class Texture 
{
public:
	char name[NAME_SIZE] = "";
	char path[PATH_SIZE] = "";
	char extension[EXTEN_SIZE] = "";
	unsigned id = 0u;
	int width = 0;
	int height = 0;
	const char* format = "";
	Resize_Mode resize_mode = linear;
	Wrap_Mode wrap_mode = clamp;
	bool use_mipmap = false;

	Texture();
	~Texture();

	const char* getResizeMode();
	inline void setResizeMode(const char* mode) 
	{
		if (mode == "nearest") { this->resize_mode = nearest; }
		else if (mode == "linear") { this->resize_mode = linear; }
	}

	const char* getWrapMode();
	inline void setWrapMode(const char* mode) {
		if (mode == "clamp to boder") { this->wrap_mode = clamp_to_boder; }
		else if (mode == "clamp") { this->wrap_mode = clamp; }
		else if (mode == "repeat") { this->wrap_mode = repeat; }
		else if (mode == "mirrored") { this->wrap_mode = mirrored; }
	}
};

#endif // __TEXTURE_H__

