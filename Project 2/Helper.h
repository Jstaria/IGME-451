#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

std::string ToLower(std::string str)
{
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

std::vector<std::string> SplitArgs(const std::string& input)
{
	std::istringstream iss(input);
	std::vector<std::string> args;
	std::string token;
	while (iss >> token)
		args.push_back(ToLower(token));
	return args;
}