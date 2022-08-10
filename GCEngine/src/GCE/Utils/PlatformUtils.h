#pragma once

#include <string>

namespace  GCE
{
	class FileDialogs
	{
	public:
		static std::string openFile(const char* filter);
		static std::string saveFile(const char* filter);
	};
}
