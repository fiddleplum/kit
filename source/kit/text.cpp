#include "text.h"

Text::Text()
{
}

Text::Text(std::string const & chars)
{
	s = chars;
}

Text::Iterator Text::begin()
{
	return {0, this};
}

Text::IteratorConst Text::begin() const
{
	return {0, this};
}

Text::Iterator Text::end()
{
	return {(int)s.size(), this};
}

Text::IteratorConst Text::end() const
{
	return {(int)s.size(), this};
}

void Text::Iterator::operator ++ ()
{
	int numBytes;
	int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

int Text::Iterator::operator * () const
{
	int numBytes;
	int codePoint;
	s->getChar(i, numBytes, codePoint);
	return codePoint;
}

bool Text::Iterator::operator != (Text::Iterator const & it)
{
	return i != it.i;
}

void Text::IteratorConst::operator ++ ()
{
	int numBytes;
	int codePoint;
	s->getChar(i, numBytes, codePoint);
	i += numBytes;
}

int Text::IteratorConst::operator * () const
{
	int numBytes;
	int codePoint;
	s->getChar(i, numBytes, codePoint);
	return codePoint;
}

bool Text::IteratorConst::operator != (Text::IteratorConst const & it)
{
	return i != it.i;
}

void Text::getChar(int i, int & numBytes, int & codePoint) const
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

