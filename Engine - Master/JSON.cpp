#include "JSON.h"

#include "Globals.h"

JSON_value::JSON_value(rapidjson::Document::AllocatorType* allocator, rapidjson::Type type) : alloc(allocator)
{
	rapidjson_value = new rapidjson::Value(type);
};

JSON_value* JSON_value::CreateValue(rapidjson::Type type)
{
	JSON_value* ret = new JSON_value(alloc, type);
	values_alloc.push_back(ret);
	return ret;
}

JSON::JSON()
{
	document = new rapidjson::Document();
	document->SetObject();
	alloc = &document->GetAllocator();
	json_buffer = new rapidjson::StringBuffer();
}

JSON::JSON(const char *data)
{
	document = new rapidjson::Document();
	document->SetObject();
	document->Parse(data);
	alloc = &document->GetAllocator();
	json_buffer = new rapidjson::StringBuffer();
}

JSON::~JSON()
{
	RELEASE(document);
}


JSON_value * JSON::CreateValue(rapidjson::Type type)
{
	JSON_value* ret = new JSON_value(alloc, type);
	values_alloc.push_back(ret);
	return ret;
}

void JSON::AddValue(const char* name, JSON_value* value)
{
	std::string myname(name);
	rapidjson::Value key(myname.c_str(), myname.size(), *alloc);
	document->AddMember(key, *value->rapidjson_value, *alloc);
}

JSON_value * JSON::GetValue(const char* name)
{
	if (document->HasMember(name))
	{
		JSON_value * val = new JSON_value(alloc);
		val->rapidjson_value->CopyFrom(document->operator[](name), *alloc);
		values_alloc.push_back(val);
		return val;
	}
	return nullptr;
}


std::string JSON::ToString()
{
	json_buffer->Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(*json_buffer);
	document->Accept(writer);

	return json_buffer->GetString();
}

unsigned JSON::Size()
{
	json_buffer->Clear();
	rapidjson::Writer<rapidjson::StringBuffer> writer(*json_buffer);
	document->Accept(writer);
	return json_buffer->GetSize();
}
