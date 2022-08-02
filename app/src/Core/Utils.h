/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace StockCharts
{
    namespace Utils
    {
        std::wstring	toWStr(const std::string& str);
        std::string		toStr(const std::wstring& wstr);

        std::string		to8bitStr(const std::string& str);

        std::vector<std::string> splitStr(const std::string str, const char separator);

        bool			checkEmpty(const std::string& str);
    }
}
