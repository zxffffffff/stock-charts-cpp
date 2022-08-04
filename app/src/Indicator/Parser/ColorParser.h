/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "SubParser.h"

namespace StockCharts
{
	class ColorParser : public HSLib
	{
	public:
		ColorParser();

		bool check(const std::string& name);
		std::tuple<bool, ExpColorType> process(const std::string& name);

	private:
		std::tuple<bool, ExpColorType> emptyRet(bool ok = false)
		{
			return { ok, ExpColorType() };
		}

		bool checkCustomColor(const std::string& name);

		std::tuple<bool, ExpColorType> colorType(const std::string& name);
		std::tuple<bool, ExpColorType> lineThick(const std::string& name);
		std::tuple<bool, ExpColorType> colorString(const std::string& name);

	private:
		using F = std::function<std::tuple<bool, ExpColorType>(const std::string&)>;
		std::vector<F>							m_binds;
		std::map<std::string, EnExpLineType>	m_types;
		std::map<std::string, EnExpLineThick>	m_thicks;
		std::map<std::string, std::string>		m_colors;
	};
}
