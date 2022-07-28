#pragma once
#include "NumberCore.h"
#include <memory>

namespace StockCharts
{
	struct StockCore
	{
		NumberCore open;	// necessary
		NumberCore high;	// necessary
		NumberCore low;		// necessary
		NumberCore close;	// necessary
		NumberCore vol;
		NumberCore timestamp;

		int getSize() const;
		std::pair<Number, Number> getMinMax(int begin = 0, int end = -1) const;
	};
	using SpStockCore = std::shared_ptr<StockCore>;
	using CpStockCore = std::shared_ptr<const StockCore>;
}
