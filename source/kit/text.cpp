#include "text.h"

Text::Text()
{
}

Text::Text(std::string const & chars)
{
	s = chars;
}

std::string const & Text::str() const
{
	return s;
}

void Text::clear()
{
	s.clear();
}

void Text::operator += (unsigned int c)
{
	putChar(s.size(), c);
}

void Text::operator += (std::string const & s_)
{
	s += s_;
}

void Text::operator += (Text const & text)
{
	s += text.s;
}

Text Text::operator + (unsigned int c) const
{
	Text rText = *this;
	rText += c;
	return rText;
}

Text Text::operator + (std::string const & s) const
{
	Text rText = *this;
	rText += s;
	return rText;
}

Text Text::operator + (Text const & text) const
{
	Text rText = *this;
	rText += text;
	return rText;
}

bool Text::operator == (Text const & text) const
{
	return s == text.s;
}

bool Text::operator != (Text const & text) const
{
	return s != text.s;
}

bool Text::operator < (Text const & text) const
{
	return s < text.s;
}

Text::iterator Text::begin()
{
	return{0, this};
}

Text::const_iterator Text::begin() const
{
	return{0, this};
}

Text::iterator Text::end()
{
	return{(int)s.size(), this};
}

Text::const_iterator Text::end() const
{
	return{(int)s.size(), this};
}

void Text::iterator::operator ++ ()
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

void Text::iterator::operator ++ (int)
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

unsigned int Text::iterator::operator * () const
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	return codePoint;
}

bool Text::iterator::operator != (Text::iterator const & it)
{
	return i != it.i;
}

void Text::const_iterator::operator ++ ()
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

void Text::const_iterator::operator ++ (int)
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

unsigned int Text::const_iterator::operator * () const
{
	unsigned int numBytes;
	unsigned int codePoint;
	s->getChar(i, numBytes, codePoint);
	return codePoint;
}

bool Text::const_iterator::operator != (Text::const_iterator const & it)
{
	return i != it.i;
}

void Text::getChar(unsigned int i, unsigned int & numBytes, unsigned int & codePoint) const
{
	numBytes = 0;
	codePoint = 0;
	if(i < (int)s.size())
	{
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
	}
}

void Text::putChar(unsigned int i, unsigned int codePoint)
{
	std::string::iterator iter = s.begin() + i;
	if(codePoint <= 0x7f)
	{
		s.insert(s.begin() + i + 0, codePoint & 0x7f);
	}
	else if(codePoint <= 0x7ff)
	{
		s.insert(s.begin() + i + 0, 0xc0 | ((codePoint >> 6) & 0x1f));
		s.insert(s.begin() + i + 1, 0x80 | (codePoint & 0x3f));
	}
	else if(codePoint <= 0xffff)
	{
		s.insert(s.begin() + i + 0, 0xe0 | ((codePoint >> 12) & 0x0f));
		s.insert(s.begin() + i + 1, 0x80 | ((codePoint >> 6) & 0x3f));
		s.insert(s.begin() + i + 2, 0x80 | (codePoint & 0x3f));
	}
	else if(codePoint <= 0x1fffff)
	{
		s.insert(s.begin() + i + 0, 0xf0 | ((codePoint >> 18) & 0x07));
		s.insert(s.begin() + i + 1, 0x80 | ((codePoint >> 12) & 0x3f));
		s.insert(s.begin() + i + 2, 0x80 | ((codePoint >> 6) & 0x3f));
		s.insert(s.begin() + i + 3, 0x80 | (codePoint & 0x3f));
	}
	else if(codePoint <= 0x3ffffff)
	{
		s.insert(s.begin() + i + 0, 0xf8 | ((codePoint >> 24) & 0x03));
		s.insert(s.begin() + i + 1, 0x80 | ((codePoint >> 18) & 0x3f));
		s.insert(s.begin() + i + 2, 0x80 | ((codePoint >> 12) & 0x3f));
		s.insert(s.begin() + i + 3, 0x80 | ((codePoint >> 6) & 0x3f));
		s.insert(s.begin() + i + 4, 0x80 | (codePoint & 0x3f));
	}
	else if(codePoint <= 0x7fffffff)
	{
		s.insert(s.begin() + i + 0, 0xfc | ((codePoint >> 30) & 0x01));
		s.insert(s.begin() + i + 1, 0x80 | ((codePoint >> 24) & 0x3f));
		s.insert(s.begin() + i + 2, 0x80 | ((codePoint >> 18) & 0x3f));
		s.insert(s.begin() + i + 3, 0x80 | ((codePoint >> 12) & 0x3f));
		s.insert(s.begin() + i + 4, 0x80 | ((codePoint >> 6) & 0x3f));
		s.insert(s.begin() + i + 5, 0x80 | (codePoint & 0x3f));
	}
}

