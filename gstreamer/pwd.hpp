#pragma once
#include <string>

inline std::string pwd()
{
	char buf[1024];
	getcwd(buf, sizeof(buf));
	return buf;
}
