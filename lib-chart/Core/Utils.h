/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "NumberCore.h"
#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <iomanip>
#include <cassert>

namespace StockCharts
{
    class Utils
    {
    public:
        static std::wstring toWStr(const std::string &str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.from_bytes(str);
        }
        static std::string toStr(const std::wstring &wstr)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.to_bytes(wstr);
        }

        static std::string to8bitStr(const std::string &str)
        {
            if (str.empty())
                return str;

            std::wstring wstr = toWStr(str);
            std::wstringstream wss;
            for (int i = 0; i < wstr.size(); ++i)
            {
                wchar_t wc = wstr[i];
                if (wc < 0 || wc > 127)
                {
                    wss << "%u" << std::hex << std::uppercase << uint64_t(wc);
                    continue;
                }
                else
                {
                    wss << wc;
                }
            }
            return toStr(wss.str());
        }

        static std::vector<std::string> splitStr(const std::string &str, const char separator)
        {
            std::vector<std::string> ret;
            std::size_t start = 0, end = 0;
            while ((end = str.find(separator, start)) != std::string::npos)
            {
                if (end != start)
                {
                    ret.push_back(str.substr(start, end - start));
                }
                start = end + 1;
            }
            if (end != start)
            {
                ret.push_back(str.substr(start));
            }
            return ret;
        }

        static bool checkEmpty(const std::string &str)
        {
            if (str.empty())
                return true;

            for (const auto &c : str)
            {
                if (c != '\r' && c != '\n' && c != '\t' && c != '\v' && c != ' ' && c != ';')
                {
                    return false;
                }
            }
            return true;
        }
    };

    class NumberUtils
    {
    public:
        static std::string toString(Number price, int precision = 2, std::string sNull = "--")
        {
            if (price == NumberNull)
                return sNull;
            std::stringstream ss;
            ss.precision(precision);
            ss.setf(std::ios::fixed);
            ss << price;
            return ss.str();
        }

        static std::string toTimestamp(Number n, const char *format = "%Y-%m-%d %H:%M:%S")
        {
            std::time_t t = static_cast<time_t>(n);
            std::tm tm = *std::localtime(&t);
            std::ostringstream ss;
            ss << std::put_time(&tm, format);
            return ss.str();
        }
        static Number toTimestamp(const std::string s, const char *format = "%Y-%m-%d %H:%M:%S")
        {
            std::tm tm = {};
            std::istringstream ss(s);
            ss >> std::get_time(&tm, format);
            if (ss.fail())
            {
                return NumberNull;
            }
            std::time_t t = std::mktime(&tm);
            return t;
        }

        template <typename... Args>
        static bool isDoubleValid(double value, Args... args)
        {
            if (std::isnan(value) || std::isinf(value))
            {
                return false;
            }
            return isDoubleValid(args...);
        }

        static bool isDoubleValid(double value)
        {
            return !(std::isnan(value) || std::isinf(value));
        }
    };
}
