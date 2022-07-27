#include "Utils.h"
#include <locale>
#include <codecvt>

using namespace StockCharts;

std::wstring Utils::toWStr(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}

std::string Utils::toStr(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

std::string Utils::to8bitStr(const std::string& str)
{
	if (str.empty())
		return str;

	std::wstring wstr = toWStr(str);
	std::wstringstream wss;
	for (int i = 0; i < wstr.size(); ++i) {
		wchar_t wc = wstr[i];
		if (wc < 0 || wc > 127) {
			wss << "%u" << std::hex << std::uppercase << uint64_t(wc);
			continue;
		}
		else {
			wss << wc;
		}
	}
	return toStr(wss.str()) + ";";
}

std::vector<std::string> Utils::splitStr(const std::string str, const char separator)
{
	std::vector<std::string> ret;
	std::size_t start = 0, end = 0;
	while ((end = str.find(separator, start)) != std::string::npos) {
		if (end != start) {
			ret.push_back(str.substr(start, end - start));
		}
		start = end + 1;
	}
	if (end != start) {
		ret.push_back(str.substr(start));
	}
	return ret;
}

bool Utils::checkEmpty(const::std::string& str)
{
	if (str.empty())
		return true;

	for (auto& c : str) {
		switch (c)
		{
		case '\r':
		case '\n':
		case '\t':
		case '\v':
		case ' ':
		case ';':
			continue;

		default:
			return false;
		}
	}
	return true;
}
