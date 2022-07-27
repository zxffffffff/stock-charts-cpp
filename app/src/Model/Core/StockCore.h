#pragma once
#include "NumberCore.h"

namespace StockCharts
{
	struct StockCore
	{
		NumberCore open;
		NumberCore high;
		NumberCore low;
		NumberCore close;
		NumberCore vol;
		NumberCore timestamp;
	};
}
