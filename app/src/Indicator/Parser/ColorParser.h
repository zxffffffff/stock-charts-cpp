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

		bool check(const String& name);
		std::tuple<bool, ExpColorType> process(const String& name);

	private:
		std::tuple<bool, ExpColorType> emptyRet(bool ok = false)
		{
			return { ok, ExpColorType() };
		}

		bool checkCustomColor(const String& name);

		std::tuple<bool, ExpColorType> colorType(const String& name);
		std::tuple<bool, ExpColorType> lineThick(const String& name);
		std::tuple<bool, ExpColorType> colorString(const String& name);

	private:
		using F = std::function<std::tuple<bool, ExpColorType>(const String&)>;
		std::vector<F>							m_binds;
		std::map<String, EnExpLineType>	m_types;
		std::map<String, EnExpLineThick>	m_thicks;
		std::map<String, String>		m_colors;
	};
}
