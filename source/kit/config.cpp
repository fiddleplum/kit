#include "config.h"
#include "string_util.h"
#include <fstream>
#include <sstream>

Config::Config()
{
}

Config::Config(std::string const & filename)
{
	Text text = {readFile(filename, false)};
	Text::const_iterator iter = text.begin();
	root.parse(iter, text.end());
}

void Config::save(std::string const & filename)
{
	Text text;
	root.toText(text);
	std::ofstream file{filename};
	file.write(text.str().c_str(), text.str().size());
}

Config::Value & Config::getRoot()
{
	return root;
}

Config::Value const & Config::getRoot() const
{
	return root;
}

void Config::Value::parse(Text::const_iterator & iter, Text::const_iterator const & end)
{
	type = 0;
	text.clear();
	children.clear();
	unsigned int c = 0;
	while(iter != end)
	{
		c = *iter;
		if(c == ' ' || c == '\t' || c == '\n') // White Space
		{
			iter++;
		}
		else
		{
			break;
		}
	}
	c = *iter;
	if(c == '{') // Dictionary
	{
		iter++;
		type = Value::Dictionary;
		while(iter != end)
		{
			c = *iter;
			if(c == ' ' || c == '\t' || c == '\n') // White Space
			{
				iter++;
			}
			else if(c == '}')
			{
				iter++;
				break;
			}
			else
			{
				Text key;
				unsigned int lastC = 0;
				while(iter != end)
				{
					c = *iter;
					if((c == ':' || c == ' ' || c == '\t' || c == '\n') && lastC != '\\') // Break on ':' or whitespace except with preceding '\'
					{
						break;
					}
					else
					{
						key += c;
						lastC = c;
						iter++;
					}
				}
				while(iter != end)
				{
					c = *iter;
					if(c == ':') // Break on ':'
					{
						iter++;
						break;
					}
					iter++;
				}
				children[key] = Value();
				children[key].parse(iter, end);
			}
		}
	}
	else if(c == '[') // List
	{
		iter++;
		type = Value::List;
		int count = 0;
		while(iter != end)
		{
			c = *iter;
			if(c == ' ' || c == '\t' || c == '\n') // White Space
			{
				iter++;
			}
			else if(c == ']')
			{
				iter++;
				break;
			}
			else
			{
				Text key = std::to_string(count);
				children[key] = Value();
				children[key].parse(iter, end);
				count++;
			}
		}
	}
	else // String
	{
		type = Value::String;
		int lastC = 0;
		while(iter != end)
		{
			c = *iter;
			if(c == '\n' && lastC != '\\') // Break on '\n' except with preceding '\'
			{
				iter++;
				break;
			}
			else
			{
				text += c;
				lastC = c;
				iter++;
			}
		}
	}
}

void Config::Value::toText(Text & t, unsigned int tabDepth) const
{
	if(type == Dictionary)
	{
		t += "{\n";
		for(auto iter : children)
		{
			for(unsigned int i = 0; i < tabDepth + 1; i++)
			{
				t += '\t';
			}
			t += iter.first;
			t += " : ";
			iter.second.toText(t, tabDepth + 1);
		}
		for(unsigned int i = 0; i < tabDepth; i++)
		{
			t += '\t';
		}
		t += "}\n";
	}
	else if(type == List)
	{
		t += "[\n";
		unsigned int count = 0;
		while(true)
		{
			auto iter = children.find(std::to_string(count));
			if(iter == children.end())
			{
				break;
			}
			for(unsigned int i = 0; i < tabDepth + 1; i++)
			{
				t += '\t';
			}
			iter->second.toText(t, tabDepth + 1);
			count++;
		}
		for(unsigned int i = 0; i < tabDepth; i++)
		{
			t += '\t';
		}
		t += "]\n";
	}
	else if(type == String)
	{
		t += text;
		t += '\n';
	}
}

