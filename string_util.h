#pragma once

#include <string>

//! Read \b filename into \b content.
bool readFile (std::string & content, std::string const & filename);

//! Convert an int \b i to a string. (REMOVE when std::toString is fully supported)
std::string toString (int i);

//! Convert a string \b s to an int \b i. Base can be 2 to 36, or 0, in which case it is automatically determined. Returns false on failure.
bool stringToInt (int & i, std::string const & s, int base = 0);

//! Convert a string \b s to a long \b l. Base can be 2 to 36, or 0, in which case it is automatically determined. Returns false on failure.
bool stringToLong (long & l, std::string const & s, int base = 0);

//! Convert a string \b s to a float \b f. Returns false on failure.
bool stringToFloat (float & f, std::string const & s);

//! Convert a string \b s to a double \b d. Returns false on failure.
bool stringToDouble (double & d, std::string const & s);

