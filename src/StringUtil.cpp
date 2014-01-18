#include "StringUtil.h"
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

bool readFile(std::string & content, std::string const & filename)
{
	std::fstream f (filename, std::ios::in | std::ios::binary);
	if(f.is_open() == false)
	{
		return false;
	}
	f.seekg(0, std::ios::end);
	content.resize((unsigned int)f.tellg());
	f.seekg(0, std::ios::beg);
	f.read(&content[0], content.size());
	f.close();
	return true;
}

bool stringToBool(bool & b, std::string const & s)
{
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
	long l (0);
	bool result (stringToLong(l, s, base));
	if(l < INT_MIN || INT_MAX < l)
	{
		return false;
	}
	i = (int)l;
	return result;
}

bool stringToLong(long & l, std::string const & s, int base)
{
	char * end (0);
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
	char * end (0);
	errno = 0;
	if(s.size() == 0)
	{
		return false;
	}
#ifndef _MSC_VER
	f = strtof(s.c_str(), &end);
	if(errno == ERANGE || end != s.c_str() + s.size())
	{
		return false;
	}
#else
	std::istringstream ss (s);
	ss >> f;
#endif
	return true;
}

bool stringToDouble(double & d, std::string const & s)
{
	char * end (0);
	errno = 0;
	d = strtod(s.c_str(), &end);
	if(errno == ERANGE || s.size() == 0 || end != s.c_str() + s.size())
	{
		return false;
	}
	return true;
}

bool beginsWith(std::string const & s, std::string const & pattern)
{
	if(s.compare(0, pattern.size(), pattern) == 0)
	{
		return true;
	}
	return false;
}

bool endsWith(std::string const & s, std::string const & pattern)
{
	if(pattern.size() <= s.size() && s.compare(s.size() - pattern.size(), pattern.size(), pattern) == 0)
	{
		return true;
	}
	return false;
}

std::string trim(std::string const & s, bool left, bool right)
{
	std::string r (s);
	if(left)
	{
		r.erase(r.begin(), std::find_if(r.begin(), r.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	}
	if(right)
	{
		r.erase(std::find_if(r.rbegin(), r.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), r.end());
	}
	return r;
}

std::vector<std::string> split(std::string const & s, char delimiter, bool trim)
{
	std::vector<std::string> tokens;
	std::stringstream ss (s);
	std::string token;
	do
	{
		std::getline(ss, token, delimiter);
		if(trim)
		{
			token = ::trim(token);
		}
		tokens.push_back(token);
		
	} while(ss.good());
	return tokens;
}

