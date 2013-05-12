#include "Exception.h"

using namespace std;

Exception::Exception(const string& msg, const string& file, int line)
{
	this->msg = msg;
	this->file = file;
	this->line = line;
}

string Exception::GetMsg() const
{
	return msg;
}

string Exception::GetFile() const
{
	return file;
}

int Exception::GetLine() const
{
	return line;
}
