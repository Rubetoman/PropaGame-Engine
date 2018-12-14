#ifndef __JSON_H__
#define __JSON_H__

#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <string>
#include <vector>
#include "MathGeoLib.h"

struct JSON_value
{
public:
	JSON_value(rapidjson::Document::AllocatorType* allocator, rapidjson::Type type = rapidjson::kObjectType);
	JSON_value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);

public:
	rapidjson::Value* rapidjson_value = nullptr;
	rapidjson::Document::AllocatorType* alloc = nullptr;
private:
	std::vector<JSON_value*> values_alloc;
};

class JSON
{
public:
	JSON();
	JSON(const char* data);
	~JSON();

	JSON_value* CreateValue(rapidjson::Type type = rapidjson::kObjectType);
	void AddValue(const char* name, JSON_value* value);
	JSON_value* GetValue(const char* name);
	std::string ToString();
	unsigned Size();

private:
	rapidjson::Document* document = nullptr;
	rapidjson::Document::AllocatorType* alloc = nullptr;
	rapidjson::StringBuffer* json_buffer = nullptr;
	std::vector<JSON_value*> values_alloc;
};

#endif __JSON_H__

