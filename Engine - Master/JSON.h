#ifndef __JSON_H__
#define __JSON_H__

#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/filereadstream.h"

#include <string>
#include <vector>
#include "MathGeoLib.h"

struct JSON_value
{
public:
	JSON_value(rapidjson::Document::AllocatorType* allocator);
	~JSON_value();

	void JSON_value::convertToArray();

	JSON_value* createValue();
	void addValue(const char* name, JSON_value* value);
	JSON_value* getValue(const char* name);

	void setValue(rapidjson::Value* value);
	rapidjson::Value* getRapidJSONValue();

	void AddInt(const char* name, int value);
	void AddUnsigned(const char* name, unsigned value);
	void AddString(const char* name, const char* value);
	void AddVec(const char* name, float* vec, int vector_size);
	void AddVec3(const char* name, math::float3 vec);
	void AddVec4(const char* name, math::float4 vec);
	void AddQuat(const char* name, math::Quat quat);

public:
	rapidjson::Value* value = nullptr;
	rapidjson::Document::AllocatorType* alloc = nullptr;

private:
	std::vector<JSON_value*> values_alloc;
};

struct JSON_file
{
public:
	JSON_file(rapidjson::FileWriteStream* os, FILE* fp);
	JSON_file(rapidjson::FileReadStream* is, FILE* fp);
	~JSON_file();

	bool Write();

	JSON_value* createValue();
	void addValue(const char* name, JSON_value* value);
	JSON_value* getValue(const char* name);

	void closeFile();

private:

	FILE* fp = nullptr;

	rapidjson::Document* document = nullptr;
	rapidjson::FileWriteStream* os = nullptr;
	rapidjson::FileReadStream* is = nullptr;

	rapidjson::Document::AllocatorType* alloc = nullptr;
};

class JSON
{
public:
	JSON() {};
	~JSON() {};

	JSON_file* openReadFile(const char* path);
	JSON_file* openWriteFile(const char* path);
	void closeFile(JSON_file* file);
};

#endif __JSON_H__

