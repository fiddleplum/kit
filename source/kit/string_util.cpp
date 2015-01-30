#include "string_util.h"
#include <cstdlib>
#include <climits>
#include <cerrno>
#include <sstream>
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

std::string readFile(std::string const & filename, bool binary)
{
	std::string s;
	std::fstream f(filename, std::ios::in | (binary ? std::ios::binary : 0));
	if(f.is_open() == false)
	{
		throw std::runtime_error("The file '" + filename + "' could not be opened. ");
	}
	f.seekg(0, std::ios::end);
	s.resize((unsigned int)f.tellg());
	f.seekg(0, std::ios::beg);
	f.read(&s[0], s.size());
	f.close();
	return s;
}

void toUnicode(std::vector<unsigned int> & codes, std::string const & s)
{
	codes.clear();
	unsigned int i = 0;
	while(i < (int)s.size())
	{
		unsigned int numBytes = 0;
		unsigned int codePoint = 0;
		if(s[i] <= 0x7f && i < (int)s.size()) // 0 to 7f
		{
			numBytes = 1;
			codePoint = s[i] & 0x7f;
		}
		else if((s[i] & 0xe0) == 0xc0 && i + 1 < (int)s.size()) // 80 to 7ff
		{
			numBytes = 2;
			codePoint = ((s[i] & 0x1f) << 6) + (s[i + 1] & 0x3f);
		}
		else if((s[i] & 0xf0) == 0xe0 && i + 2 < (int)s.size()) // 800 to ffff
		{
			numBytes = 3;
			codePoint = ((s[i] & 0x0f) << 12) + ((s[i + 1] & 0x3f) << 6) + (s[i + 2] & 0x3f);
		}
		else if((s[i] & 0xf8) == 0xf0 && i + 3 < (int)s.size()) // 10000 to 1fffff
		{
			numBytes = 4;
			codePoint = ((s[i] & 0x07) << 18) + ((s[i + 1] & 0x3f) << 12) + ((s[i + 2] & 0x3f) << 6) + (s[i + 3] & 0x3f);
		}
		else if((s[i] & 0xfc) == 0xf8 && i + 4 < (int)s.size()) // 200000 to 3ffffff
		{
			numBytes = 5;
			codePoint = ((s[i] & 0x03) << 24) + ((s[i + 1] & 0x3f) << 18) + ((s[i + 2] & 0x3f) << 12) + ((s[i + 3] & 0x3f) << 6) + (s[i + 4] & 0x3f);
		}
		else if((s[i] & 0xfe) == 0xfc && i + 5 < (int)s.size()) // 4000000 to 7fffffff
		{
			numBytes = 6;
			codePoint = ((s[i] & 0x01) << 30) + ((s[i + 1] & 0x3f) << 24) + ((s[i + 2] & 0x3f) << 18) + ((s[i + 3] & 0x3f) << 12) + ((s[i + 4] & 0x3f) << 6) + (s[i + 5] & 0x3f);
		}
		if(numBytes > 0)
		{
			codes.push_back(codePoint);
			i += numBytes;
		}
		else
		{
			break;
		}
	}
}

void fromUnicode(std::string & s, std::vector<unsigned int> const & codes)
{
	s.clear();
	for(unsigned int i = 0; i < codes.size(); i++)
	{
		if(codes[i] <= 0x7f)
		{
			s.push_back(codes[i] & 0x7f);
		}
		else if(codes[i] <= 0x7ff)
		{
			s.push_back(0xc0 | ((codes[i] >> 6) & 0x1f));
			s.push_back(0x80 | (codes[i] & 0x3f));
		}
		else if(codes[i] <= 0xffff)
		{
			s.push_back(0xe0 | ((codes[i] >> 12) & 0x0f));
			s.push_back(0x80 | ((codes[i] >> 6) & 0x3f));
			s.push_back(0x80 | (codes[i] & 0x3f));
		}
		else if(codes[i] <= 0x1fffff)
		{
			s.push_back(0xf0 | ((codes[i] >> 18) & 0x07));
			s.push_back(0x80 | ((codes[i] >> 12) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 6) & 0x3f));
			s.push_back(0x80 | (codes[i] & 0x3f));
		}
		else if(codes[i] <= 0x3ffffff)
		{
			s.push_back(0xf8 | ((codes[i] >> 24) & 0x03));
			s.push_back(0x80 | ((codes[i] >> 18) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 12) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 6) & 0x3f));
			s.push_back(0x80 | (codes[i] & 0x3f));
		}
		else if(codes[i] <= 0x7fffffff)
		{
			s.push_back(0xfc | ((codes[i] >> 30) & 0x01));
			s.push_back(0x80 | ((codes[i] >> 24) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 18) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 12) & 0x3f));
			s.push_back(0x80 | ((codes[i] >> 6) & 0x3f));
			s.push_back(0x80 | (codes[i] & 0x3f));
		}
	}
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
	long l(0);
	bool result(stringToLong(l, s, base));
	if(l < INT_MIN || INT_MAX < l)
	{
		return false;
	}
	i = (int)l;
	return result;
}

bool stringToLong(long & l, std::string const & s, int base)
{
	char * end(0);
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
	errno = 0;
	if(s.size() == 0)
	{
		return false;
	}
#ifndef _MSC_VER
	char * end (0);
	f = strtof(s.c_str(), &end);
	if(errno == ERANGE || end != s.c_str() + s.size())
	{
		return false;
	}
#else
	std::istringstream ss(s);
	ss >> f;
#endif
	return true;
}

bool stringToDouble(double & d, std::string const & s)
{
	char * end(0);
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
	std::string r(s);
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

std::vector<std::string> split(std::string const & s, char delimiter, bool trimTokens)
{
	std::vector<std::string> tokens;
	std::stringstream ss(s);
	std::string token;
	do
	{
		std::getline(ss, token, delimiter);
		if(trimTokens)
		{
			token = trim(token);
		}
		tokens.push_back(token);

	}
	while(ss.good());
	return tokens;
}

