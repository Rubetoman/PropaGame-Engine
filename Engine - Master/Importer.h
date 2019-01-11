#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include <string>

class Importer
{
public:
	Importer();
	virtual ~Importer();

	virtual bool Import(const char* file, const char* path, std::string& output_file) = 0;
};

#endif /*__IMPORTER_H__*/