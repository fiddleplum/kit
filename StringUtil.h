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

