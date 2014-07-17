#include "file_system.h"
#include "platform.h"

#ifdef KIT_WINDOWS
#include <Windows.h>
#include <locale>
#include <codecvt>
#endif

namespace kit
{
	namespace fs
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> _converter;

		bool isFolder(std::string const & path)
		{
			std::wstring wpath = _converter.from_bytes(path);
#ifdef KIT_WINDOWS
			DWORD result = GetFileAttributes(wpath.c_str());
			if (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY))
			{
				return true;
			}
#endif
			return false;
		}

		std::vector<std::string> getFilesInFolder(std::string const & path)
		{
#ifdef KIT_WINDOWS
			WIN32_FIND_DATA ffd;
			HANDLE handle;

			std::vector<std::string> files;

			std::wstring wpath = _converter.from_bytes(path);
			handle = FindFirstFile((wpath + L"\\*.*").c_str(), &ffd);
			if (handle != INVALID_HANDLE_VALUE)
			{
				do
				{
					std::string file = _converter.to_bytes(ffd.cFileName);
					if (file == "." || file == "..")
					{
						continue;
					}
					files.push_back(file);
				} while (FindNextFile(handle, &ffd) != 0);
				FindClose(handle);
			}
			return files;
#endif
		}
	}
}

