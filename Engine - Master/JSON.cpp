#include "JSON.h"

#include "Globals.h"

#pragma region JSON_value

JSON_value::JSON_value(rapidjson::Document::AllocatorType* allocator) : alloc(allocator)
{
	value = new rapidjson::Value(rapidjson::kObjectType);
};

JSON_value::~JSON_value()
{
	RELEASE(value);
}

void JSON_value::convertToArray()
{
	RELEASE(value);
	value = new rapidjson::Value(rapidjson::kArrayType);
}

JSON_value * JSON_value::createValue()
{
	return new JSON_value(alloc);
}

void JSON_value::addValue(const char * name, JSON_value * value)
{
	if (this->value->GetType() == rapidjson::kObjectType)
	{
		std::string str = name;
		rapidjson::Value index(str.c_str(), str.size(), *alloc);
		this->value->AddMember(index, *value->getRapidJSONValue(), *alloc);
	}
	else if (this->value->GetType() == rapidjson::kArrayType)
	{
		this->value->PushBack(*value->getRapidJSONValue(), *alloc);
	}
}

JSON_value* JSON_value::getValue(const char * name)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& trueValue = value->operator[](name);
		JSON_value* ret = new JSON_value(alloc);
		ret->getRapidJSONValue()->CopyFrom(trueValue, *alloc, false);
		return ret;
	}
	return nullptr;
}

void JSON_value::setValue(rapidjson::Value * value)
{
	RELEASE(this->value);
	this->value = value;
}

rapidjson::Value* JSON_value::getRapidJSONValue()
{
	return value;
}

void JSON_value::AddInt(const char* name, int value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);
	this->value->AddMember(index, value, *alloc);
}

void JSON_value::AddUnsigned(const char* name, unsigned value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);
	this->value->AddMember(index, value, *alloc);
}

void JSON_value::AddString(const char* name, const char* value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);
	std::string str2 = value;
	rapidjson::Value val(str2.c_str(), str2.size(), *alloc);
	this->value->AddMember(index, val, *alloc);
}

#pragma endregion

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#pragma region JSON_file

JSON_file::JSON_file(rapidjson::FileWriteStream* os, FILE* fp) : os(os), fp(fp)
{
	document = new rapidjson::Document();
	document->SetObject();
	alloc = &document->GetAllocator();
}

JSON_file::JSON_file(rapidjson::FileReadStream* is, FILE* fp) : is(is), fp(fp)
{
	document = new rapidjson::Document();
	document->ParseStream(*is);
	alloc = &document->GetAllocator();
}

JSON_file::~JSON_file()
{
	RELEASE(document);
	RELEASE(is);
	RELEASE(os);
}

bool JSON_file::Write()
{
	if (os != nullptr)
	{
		//rapidjson::Writer<rapidjson::FileWriteStream> writer(*os);
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(*os);
		document->Accept(writer);
		return true;
	}
	else
		return false;
}

JSON_value* JSON_file::createValue()
{
	return new JSON_value(alloc);
}

void JSON_file::addValue(const char * name, JSON_value* value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);
	document->AddMember(index, *value->getRapidJSONValue(), *alloc);
}

JSON_value* JSON_file::getValue(const char* name)
{
	if (document->HasMember(name))
	{
		rapidjson::Value& value = document->operator[](name);
		JSON_value* ret = new JSON_value(alloc);
		ret->getRapidJSONValue()->CopyFrom(value, *alloc, false);

		return ret;
	}
	return nullptr;
}

void JSON_file::closeFile()
{
	fclose(fp);
}

#pragma endregion

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#pragma region JSON

JSON_file* JSON::openReadFile(const char* path)
{
	FILE* fp = fopen(path, "rb");
	char readBuffer[65536];

	return new JSON_file(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
}

JSON_file* JSON::openWriteFile(const char * path)
{
	FILE* fp = fopen(path, "wb");
	char writeBuffer[65536];

	return new JSON_file(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
}

void JSON::closeFile(JSON_file* file)
{
	file->closeFile();

	RELEASE(file);
	file = nullptr;
}

#pragma endregion