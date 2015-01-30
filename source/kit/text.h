#pragma once

#include <string>

// UTF-8 Encoded Text
class Text
{
public:
	Text();
	Text(std::string const & chars);

	std::string const & str() const;

	void clear();

	void operator += (unsigned int c);
	void operator += (std::string const & s);
	void operator += (Text const & text);

	Text operator + (unsigned int c) const;
	Text operator + (std::string const & s) const;
	Text operator + (Text const & text) const;

	bool operator == (Text const & text) const;
	bool operator != (Text const & text) const;
	bool operator < (Text const & text) const;

	class iterator;
	class const_iterator;
	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	class iterator
	{
	public:
		iterator(int i_, Text * s_) : i(i_), s(s_) {}
		void operator ++ ();
		void operator ++ (int);
		unsigned int operator * () const;
		bool operator != (iterator const & it);
	private:
		int i;
		Text * s;
		friend const_iterator;
	};
	class const_iterator
	{
	public:
		const_iterator(int i_, Text const * s_) : i(i_), s(s_) {}
		const_iterator(iterator const & iter) : i(iter.i), s(iter.s) {}
		void operator ++ ();
		void operator ++ (int);
		unsigned int operator * () const;
		bool operator != (const_iterator const & it);
	private:
		int i;
		Text const * s;
	};

private:
	void getChar(unsigned int i, unsigned int & numBytes, unsigned int & codePoint) const;
	void putChar(unsigned int i, unsigned int codePoint);

	std::string s;
};

