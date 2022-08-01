#pragma once
#include "SubParser.h"

namespace StockCharts
{
	class FunctionParser : public HSLib
	{
	public:
		FunctionParser();

		bool check(const String& name);
		std::tuple<bool, NumberCore> process(const String& name, const std::vector<NumberCore>& inputs);

	private:
		static std::tuple<bool, NumberCore> emptyRet(bool ok = false)
		{
			return { ok, NumberCore() };
		}

		std::tuple<bool, NumberCore> MA(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> EMA(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> SMA(const std::vector<NumberCore>& inputs);

		std::tuple<bool, NumberCore> LLV(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> HHV(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> REF(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> REFX(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> STD(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> STDP(const std::vector<NumberCore>& inputs);

		std::tuple<bool, NumberCore> processMAX(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> processMIN(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> processABS(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> processSUM(const std::vector<NumberCore>& inputs);

		std::tuple<bool, NumberCore> BARSLASTCOUNT(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> BETWEEN(const std::vector<NumberCore>& inputs);
		std::tuple<bool, NumberCore> BACKSET(const std::vector<NumberCore>& inputs);

	private:
		using F = std::function<std::tuple<bool, NumberCore>(const std::vector<NumberCore>&)>;
		std::map<String, F> m_binds;
	};
}
