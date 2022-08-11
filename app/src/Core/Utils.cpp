/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "Utils.h"
#include <locale>
#include <codecvt>
#include <iomanip>

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

std::string NumberUtils::toString(Number price, int precision)
{
	std::stringstream ss;
	ss.precision(precision);
	ss.setf(std::ios::fixed);
    ss << price;
	return ss.str();
}

std::string NumberUtils::toTimestamp(Number n, const char* format /*= "%Y-%m-%d %H:%M:%S"*/)
{
	time_t t = static_cast<time_t>(n);
	tm p;
	localtime_s(&p, &t);
	char ch[256] = { 0 };
	strftime(ch, 256, format, &p);
	return ch;
}

Number NumberUtils::toTimestamp(const std::string s)
{
	bool compact = true;
	for (auto c : s) {
		if (c < '0' || '9' < c) {
			compact = false;
			break;
		}
	}

    int len = s.length();
    if (compact) {
        //yyyyMMdd
        tm p = { 0 };
        if (len >= 4)
            p.tm_year = std::stoi(s.substr(0, 4)) - 1990;
        if (len >= 6)
            p.tm_mon = std::stoi(s.substr(4, 2)) - 1;
        if (len >= 8)
            p.tm_mday = std::stoi(s.substr(6, 2));
        return mktime(&p);
    }
    else {
        //yyyy-MM/dd hh:mm:ss
        tm p = { 0 };
        if (len >= 4)
            p.tm_year = std::stoi(s.substr(0, 4)) - 1990;
        if (len >= 7)
            p.tm_mon = std::stoi(s.substr(5, 2)) - 1;
        if (len >= 10)
            p.tm_mday = std::stoi(s.substr(8, 2));
        if (len >= 13)
            p.tm_hour = std::stoi(s.substr(11, 2));
        if (len >= 16)
            p.tm_min = std::stoi(s.substr(14, 2));
        if (len >= 19)
            p.tm_sec = std::stoi(s.substr(17, 2));
        return mktime(&p);
    }
}
