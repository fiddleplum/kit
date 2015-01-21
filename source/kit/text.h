#pragma once

#include <string>

// UTF-8 Text String
class Text
{
public:
	Text();
	Text(std::string const & chars);

	class Iterator;
	class IteratorConst;
	Iterator begin();
	IteratorConst begin() const;
	Iterator end();
	IteratorConst end() const;

	class Iterator
	{
	public:
		Iterator(int i_, Text * s_) : i(i_), s(s_) {}
		void operator ++ ();
		int operator * () const;
		bool operator != (Iterator const & it);
		int i;
		Text * s;
	};
	class IteratorConst
	{
	public:
		IteratorConst(int i_, Text const * s_) : i(i_), s(s_) {}
		void operator ++ ();
		int operator * () const;
		bool operator != (IteratorConst const & it);
		int i;
		Text const * s;
	};

	void getChar(int i, int & numBytes, int & codePoint) const;

	std::string s;
};

