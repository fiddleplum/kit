#include "Csv.h"

void parseCsv(std::vector<std::vector<std::string>> & rows, std::string const & content)
{
  rows.clear();
  rows.push_back(std::vector<std::string>());
  rows.back().push_back(std::string());
  bool inQuote = false;
  unsigned int i = 0;
  while (i < content.size())
  {
    if (inQuote == false)
    {
      if (content[i] == ',')
      {
        rows.back().push_back(std::string());
      }
      else if (content[i] == '\n' || (content[i] == '\r' && i < content.size() - 1 && content[i + 1] == '\n'))
      {
        rows.push_back(std::vector<std::string>());
        rows.back().push_back(std::string());
        if (content[i] == '\r')
        {
          i++;
        }
      }
      else if (content[i] == '"')
      {
        inQuote = true;
      }
      else
      {
        rows.back().back().push_back(content[i]);
      }
    }
    else
    {
      if (content[i] == '\\' && i < content.size() - 1 && content[i + 1] == '"')
      {
        rows.back().back().push_back(content[i + 1]);
        i++;
      }
      else if (content[i] == '"')
      {
        inQuote = false;
      }
      else
      {
        rows.back().back().push_back(content[i]);
      }
    }
    i++;
  }
}

