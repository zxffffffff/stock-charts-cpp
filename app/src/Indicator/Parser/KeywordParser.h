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
	class KeywordParser : public HSLib
	{
	public:
		KeywordParser();

		std::vector<EnStockRely> stockRely(const std::string& name);

		bool check(const std::string& name);
		std::tuple<bool, NumberCore> process(const std::string& name);

	private:
		static std::tuple<bool, NumberCore> emptyRet(bool ok = false)
		{
			return { ok, NumberCore() };
		}

		std::tuple<bool, NumberCore> OPEN();
		std::tuple<bool, NumberCore> HIGH();
		std::tuple<bool, NumberCore> LOW();
		std::tuple<bool, NumberCore> CLOSE();
		std::tuple<bool, NumberCore> VOL();

		std::tuple<bool, NumberCore> ISLASTBAR();

		std::tuple<bool, NumberCore> VOLV();
		std::tuple<bool, NumberCore> IV();

	private:
		using F = std::function<std::tuple<bool, NumberCore>()>;
		std::map<std::string, F> m_binds;
	};
}
