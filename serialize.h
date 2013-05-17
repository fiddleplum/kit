#pragma once

#include <istream>
#include <ostream>
#include <cstdint>

inline bool serialize(std::ostream & out, void const * v, int numBytes)
{
	out.write((char const *)v, numBytes);
	return out.good();
}

inline bool deserialize(std::istream & in, void * v, int numBytes)
{
	in.read((char *)v, numBytes);
	return in.good();
}

inline bool serialize(std::ostream & out, bool const & v)
{
	return serialize(out, (void const *)&v, 1);
}

inline bool serialize(std::ostream & out, char const & v)
{
	return serialize(out, (void const *)&v, 1);
}

inline bool serialize(std::ostream & out, unsigned char const & v)
{
	return serialize(out, (void const *)&v, 1);
}

inline bool serialize(std::ostream & out, short const & v)
{
	return serialize(out, (void const *)&v, 2);
}

inline bool serialize(std::ostream & out, unsigned short const & v)
{
	return serialize(out, (void const *)&v, 2);
}

inline bool serialize(std::ostream & out, int const & v)
{
	return serialize(out, (void const *)&v, 4);
}

inline bool serialize(std::ostream & out, unsigned int const & v)
{
	return serialize(out, (void const *)&v, 4);
}

inline bool serialize(std::ostream & out, long const & v)
{
	return serialize(out, (void const *)&v, 4);
}

inline bool serialize(std::ostream & out, unsigned long const & v)
{
	return serialize(out, (void const *)&v, 4);
}

inline bool serialize(std::ostream & out, long long const & v)
{
	return serialize(out, (void const *)&v, 8);
}

inline bool serialize(std::ostream & out, unsigned long long const & v)
{
	return serialize(out, (void const *)&v, 8);
}

inline bool serialize(std::ostream & out, float const & v)
{
	return serialize(out, (void const *)&v, 4);
}

inline bool serialize(std::ostream & out, double const & v)
{
	return serialize(out, (void const *)&v, 8);
}

inline bool deserialize(std::istream & in, bool & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 1);
}

inline bool deserialize(std::istream & in, char & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 1);
}

inline bool deserialize(std::istream & in, unsigned char & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 1);
}

inline bool deserialize(std::istream & in, short & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 2);
}

inline bool deserialize(std::istream & in, unsigned short & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 2);
}

inline bool deserialize(std::istream & in, int & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 4);
}

inline bool deserialize(std::istream & in, unsigned int & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 4);
}

inline bool deserialize(std::istream & in, long & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 4);
}

inline bool deserialize(std::istream & in, unsigned long & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 4);
}

inline bool deserialize(std::istream & in, long long & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 8);
}

inline bool deserialize(std::istream & in, unsigned long long & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 8);
}

inline bool deserialize(std::istream & in, float & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 4);
}

inline bool deserialize(std::istream & in, double & v)
{
	v = 0;
	return deserialize(in, (void *)&v, 8);
}

