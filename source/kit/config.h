#pragma once

#include "text.h"
#include <map>

/*
Config is a format similar but simpler than JSON. The syntax is as follows.
Root => Value
Value => Dictionary | List | String
Dictionary => '{' (Key ':' Value)* '}'
List => '[' Value* ']'
String => string ended by '\n'
Key => string ended by ':' or whitespace
All 'ended-by' characters can be used by having a preceding '\'
All white space is trimmed.
*/
class Config
{
public:
	struct Value;

	Config();
	Config(std::string const & filename);
	void save(std::string const & filename);
	Value & getRoot();
	Value const & getRoot() const;

	struct Value
	{
		void parse(Text::const_iterator & iter, Text::const_iterator const & end);
		void toText(Text & text, unsigned int tabDepth = 0) const;

		enum { Dictionary, List, String };
		int type;
		Text text;
		std::map<Text, Value> children;
	};

private:
	Value root;
};

