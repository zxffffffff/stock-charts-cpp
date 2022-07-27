#pragma once
#include "../Core/ResultIndex.h"

namespace StockCharts
{
	class HSLib
	{
	public:
		virtual ~HSLib() {}

		void setStockCore(std::shared_ptr<StockCore> p)
		{
			m_spStockCore = p;
		}
		void setStockExt(std::shared_ptr<StockRelyData> p)
		{
			m_spStockExt = p;
		}

	protected:
		std::shared_ptr<StockCore> m_spStockCore;
		std::shared_ptr<StockRelyData> m_spStockExt;
	};
}
