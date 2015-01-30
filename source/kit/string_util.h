#pragma once

#include <string>
#include <vector>

// Read a file into a string. Returns true if successful.
std::string readFile(std::string const & filename, bool binary);

void toUnicode(std::vector<unsigned int> & codes, std::string const & s);
void fromUnicode(std::string & s, std::vector<unsigned int> const & codes);

// These return true on success.
bool stringToBool(bool & b, std::string const & s); // s can be "true" or "false"
bool stringToInt(int & i, std::string const & s, int base = 0); // base can be 2 to 36, or 0, in which case it is automatically determined
bool stringToLong(long & l, std::string const & s, int base = 0);
bool stringToFloat(float & f, std::string const & s);
bool stringToDouble(double & d, std::string const & s);

bool beginsWith(std::string const & s, std::string const & pattern);
bool endsWith(std::string const & s, std::string const & pattern);
std::string trim(std::string const & s, bool left = true, bool right = true);
std::vector<std::string> split(std::string const & s, char delimiter, bool trimTokens = true); // if s is empty, then vector still contains one empty string

#ifdef __GNUC__
/// THESE FUNCTIONS CAN BE DELETED ONCE std::to_string works in GCC!
#include <cstdio>

namespace std
{
	inline string to_string(int value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%d", value);
		return buf;
	}
	inline string to_string(long value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%ld", value);
		return buf;
	}
	inline string to_string(long long value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%lld", value);
		return buf;
	}
	inline string to_string(unsigned value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%u", value);
		return buf;
	}
	inline string to_string(unsigned long value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%lu", value);
		return buf;
	}
	inline string to_string(unsigned long long value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%llu", value);
		return buf;
	}
	inline string to_string(float value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%f", value);
		return buf;
	}
	inline string to_string(double value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%f", value);
		return buf;
	}
	inline string to_string(long double value)
	{
		char buf [256] {0};
		std::sprintf(buf, "%Lf", value);
		return buf;
	}
}
#endif

