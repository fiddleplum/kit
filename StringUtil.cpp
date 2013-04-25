#include "StringUtil.h"
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <sstream>
#include <fstream>

bool readFile(std::string & content, std::string const & filename)
{
	std::fstream f(filename, std::ios::in);
	if(f.good() == false)
	{
		return false;
	}
	f.seekg(0, std::ios::end);
	size_t size = f.tellg();
	content.resize(size);
	f.seekg(0);
	f.read(&content[0], size); 
	return f.good();
}

bool stringToBool(bool & b, std::string const & s)
{
    bool b;
    if(s.compare("true") == 0)
    {
        b = true;
    }
    else if(s.compare("false") == 0)
    {
        b = false;
    }
    else
    {
        return false;
    }
    return true;    
}

bool stringToInt(int & i, std::string const & s, int base)
{
	long l;
	bool result = stringToLong(l, s, base);
	if(l < INT_MIN || INT_MAX < l)
	{
		return false;
	}
	i = (int)l;
	return result;
}

bool stringToLong(long & l, std::string const & s, int base)
{
	char * end;
	errno = 0;
	l = strtol(s.c_str(), &end, base);
	if(errno == ERANGE || s.size() == 0 || end != s.c_str() + s.size())
	{
		return false;
	}
	return true;
}

bool stringToFloat(float & f, std::string const & s)
{
	char * end;
	errno = 0;
	f = strtof(s.c_str(), &end);
	if(errno == ERANGE || s.size() == 0 || end != s.c_str() + s.size())
	{
		return false;
	}
	return true;
}

bool stringToDouble(double & d, std::string const & s)
{
	char * end;
	errno = 0;
	d = strtod(s.c_str(), &end);
	if(errno == ERANGE || s.size() == 0 || end != s.c_str() + s.size())
	{
		return false;
	}
	return true;
}

