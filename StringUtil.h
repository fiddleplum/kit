#pragma once

#include <string>

//! Read filename into content. Returns true if successful.
bool readFile(std::string & content, std::string const & filename);

//! Convert a string s to a bool b. S can be either "true" or "false". Returns false on failure.
bool stringToBool(bool & b, std::string const & s);

//! Convert a string s to an int i. Base can be 2 to 36, or 0, in which case it is automatically determined. Returns false on failure.
bool stringToInt(int & i, std::string const & s, int base = 0);

//! Convert a string s to a long l. Base can be 2 to 36, or 0, in which case it is automatically determined. Returns false on failure.
bool stringToLong(long & l, std::string const & s, int base = 0);

//! Convert a string s to a float f. Returns false on failure.
bool stringToFloat(float & f, std::string const & s);

//! Convert a string s to a double d. Returns false on failure.
bool stringToDouble(double & d, std::string const & s);

/// THESE FUNCTIONS CAN BE DELETED ONCE std::to_string works!
#include <cstdio>

namespace std
{
	inline string to_string(int value) { char buf [256] {0}; std::sprintf(buf, "%d", value); return buf; }
	inline string to_string(long value) { char buf [256] {0}; std::sprintf(buf, "%ld", value); return buf; }
	inline string to_string(long long value) { char buf [256] {0}; std::sprintf(buf, "%lld", value); return buf; }
	inline string to_string(unsigned value) { char buf [256] {0}; std::sprintf(buf, "%u", value); return buf; }
	inline string to_string(unsigned long value) { char buf [256] {0}; std::sprintf(buf, "%lu", value); return buf; }
	inline string to_string(unsigned long long value) { char buf [256] {0}; std::sprintf(buf, "%llu", value); return buf; }
	inline string to_string(float value) { char buf [256] {0}; std::sprintf(buf, "%f", value); return buf; }
	inline string to_string(double value) { char buf [256] {0}; std::sprintf(buf, "%f", value); return buf; }
	inline string to_string(long double value) { char buf [256] {0}; std::sprintf(buf, "%Lf", value); return buf; }
}
