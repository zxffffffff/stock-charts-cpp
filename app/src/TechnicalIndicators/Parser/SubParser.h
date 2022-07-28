#pragma once
#include "../Core/IndexCore.h"

namespace StockCharts
{
	class HSLib
	{
	public:
		virtual ~HSLib() {}

		void setStockCore(SpStockCore p)
		{
			m_spStockCore = p;
		}
		void setStockExt(std::shared_ptr<StockRelyData> p)
		{
			m_spStockExt = p;
		}

	protected:
		SpStockCore m_spStockCore;
		std::shared_ptr<StockRelyData> m_spStockExt;
	};
}
