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

JSON_value* JSON_value::getValueFromArray(int index)
{
	if (value->IsArray() && value->Size() > index)
	{
		rapidjson::Value& trueValue = value->operator[](index);
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

void JSON_value::AddFloat(const char* name, float value)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);
	this->value->AddMember(index, value, *alloc);
}

void JSON_value::AddBool(const char* name, bool value)
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

void JSON_value::AddVec(const char* name, float* vec, int vector_size)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);

	rapidjson::Value a(rapidjson::kArrayType);
	for (int i = 0; i < vector_size; i++)
	{
		a.PushBack(vec[i], *alloc);
	}

	this->value->AddMember(index, a, *alloc);
}

void JSON_value::AddVec3(const char* name, math::float3 vec)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(vec.x, *alloc);
	a.PushBack(vec.y, *alloc);
	a.PushBack(vec.z, *alloc);

	this->value->AddMember(index, a, *alloc);
}

void JSON_value::AddVec4(const char* name, math::float4 vec)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(vec.x, *alloc);
	a.PushBack(vec.y, *alloc);
	a.PushBack(vec.z, *alloc);
	a.PushBack(vec.w, *alloc);

	this->value->AddMember(index, a, *alloc);
}

void JSON_value::AddQuat(const char* name, math::Quat quat)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);

	rapidjson::Value a(rapidjson::kArrayType);
	a.PushBack(quat.x, *alloc);
	a.PushBack(quat.y, *alloc);
	a.PushBack(quat.z, *alloc);
	a.PushBack(quat.w, *alloc);

	this->value->AddMember(index, a, *alloc);
}

void JSON_value::AddVec4x4(const char* name, math::float4x4 mat)
{
	std::string str = name;
	rapidjson::Value index(str.c_str(), str.size(), *alloc);

	rapidjson::Value a(rapidjson::kArrayType);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a.PushBack(mat.v[i][j], *alloc);
		}
	}

	this->value->AddMember(index, a, *alloc);
}

int JSON_value::GetInt(const char* name)
{
	if (value->HasMember(name))
		return value->operator[](name).GetInt();
	else
		return 0;
}

unsigned JSON_value::GetUnsigned(const char * name)
{
	if (value->HasMember(name))
		return value->operator[](name).GetUint();
	else
		return 0;
}

float JSON_value::GetFloat(const char * name)
{
	if (value->HasMember(name))
		return value->operator[](name).GetFloat();
	else
		return 0.0f;
}

bool JSON_value::GetBool(const char* name)
{
	if (value->HasMember(name))
		return value->operator[](name).GetBool();
	else
		return false;
}

const char* JSON_value::GetString(const char* name)
{
	if (value->HasMember(name))
		return value->operator[](name).GetString();
	else
		return "";
}

float* JSON_value::GetVec(const char* name, int vector_size)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& a = value->operator[](name);
		if (a.IsArray() && a.Size() >= vector_size)
		{
			float* ret = new float[vector_size]; //MEMLEAK
			for (int i = 0; i < vector_size; i++)
			{
				ret[i] = a[i].GetFloat();
			}

			return ret;
		}
	}

	return nullptr;
}

math::float3 JSON_value::GetVec3(const char* name)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& a = value->operator[](name);
		if (a.IsArray() && a.Size() >= 3)
		{
			float3 ret;
			ret.x = a[0].GetFloat();
			ret.y = a[1].GetFloat();
			ret.z = a[2].GetFloat();

			return ret;
		}
	}
	return float3();
}

math::float4 JSON_value::GetVec4(const char* name)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& a = value->operator[](name);
		if (a.IsArray() && a.Size() >= 4)
		{
			float4 ret;
			ret.x = a[0].GetFloat();
			ret.y = a[1].GetFloat();
			ret.z = a[2].GetFloat();
			ret.w = a[3].GetFloat();

			return ret;
		}
	}
	return math::float4();
}

math::Quat JSON_value::GetQuat(const char* name)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& a = value->operator[](name);
		if (a.IsArray() && a.Size() >= 4)
		{
			Quat ret;
			ret.x = a[0].GetFloat();
			ret.y = a[1].GetFloat();
			ret.z = a[2].GetFloat();
			ret.w = a[3].GetFloat();

			return ret;
		}
	}
	return Quat();
}

math::float4x4 JSON_value::GetVec4x4(const char* name)
{
	if (value->HasMember(name))
	{
		rapidjson::Value& a = value->operator[](name);
		if (a.IsArray() && a.Size() >= 16)
		{
			float4x4 ret;
			int count = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					ret.v[i][j] = a[count].GetFloat();
					count++;
				}
			}
			return ret;
		}
	}
	return math::float4x4();
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

	//Check fopen error
	if (fp == nullptr)
		return nullptr;

	char readBuffer[65536];

	return new JSON_file(new rapidjson::FileReadStream(fp, readBuffer, sizeof(readBuffer)), fp);
}

JSON_file* JSON::openWriteFile(const char* path)
{
	FILE* fp = fopen(path, "wb");

	//Check fopen error
	if (fp == nullptr)
		return nullptr;

	char writeBuffer[65536];

	return new JSON_file(new rapidjson::FileWriteStream(fp, writeBuffer, sizeof(writeBuffer)), fp);
}

void JSON::closeFile(JSON_file* file)
{
	file->closeFile();

	RELEASE(file);
	file = nullptr;
}

bool JSON::deleteFile(const char* path)
{
	int ret = remove(path);

	//Check deletion error
	if (ret = 0)
		return true;
	else
		return false;
}

#pragma endregion