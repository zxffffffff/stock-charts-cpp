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

		bool check(const String& name);
		std::tuple<bool, ExpDrawing, NumberCore> process(const String& name, const std::vector<NumberCore>& inputs);

	private:
		static std::tuple<bool, ExpDrawing, NumberCore> emptyRet(bool ok = false)
		{
			return { ok, ExpDrawing(), NumberCore() };
		}

		std::tuple<bool, ExpDrawing, NumberCore> number(const std::vector<NumberCore>& inputs);
		std::tuple<bool, ExpDrawing, NumberCore> text(const std::vector<NumberCore>& inputs);
		std::tuple<bool, ExpDrawing, NumberCore> stickLine(const std::vector<NumberCore>& inputs);

	private:
		using F = std::function<std::tuple<bool, ExpDrawing, NumberCore>(const std::vector<NumberCore>&)>;
		std::map<String, F> m_binds;
	};
}
