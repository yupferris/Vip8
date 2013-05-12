#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <string>

class Exception
{
public:
	Exception(const std::string& msg, const std::string& file, int line);
	std::string GetMsg() const;
	std::string GetFile() const;
	int GetLine() const;

private:
	std::string msg, file;
	int line;
};

#define EXCEPTION(x) Exception((x), __FILE__, __LINE__)

#endif
