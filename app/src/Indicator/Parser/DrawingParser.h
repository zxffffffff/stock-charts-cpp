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
	class DrawingParser : public HSLib
	{
	public:
		DrawingParser();

		bool check(const std::string& name);
		std::tuple<bool, ExpDrawingType, NumberCore> process(const std::string& name, const std::vector<NumberCore>& inputs);

	private:
		static std::tuple<bool, ExpDrawingType, NumberCore> emptyRet(bool ok = false)
		{
			return { ok, ExpDrawingType(), NumberCore() };
		}

		std::tuple<bool, ExpDrawingType, NumberCore> number(const std::vector<NumberCore>& inputs);
		std::tuple<bool, ExpDrawingType, NumberCore> text(const std::vector<NumberCore>& inputs);
		std::tuple<bool, ExpDrawingType, NumberCore> stickLine(const std::vector<NumberCore>& inputs);

	private:
		using F = std::function<std::tuple<bool, ExpDrawingType, NumberCore>(const std::vector<NumberCore>&)>;
		std::map<std::string, F> m_binds;
	};
}
