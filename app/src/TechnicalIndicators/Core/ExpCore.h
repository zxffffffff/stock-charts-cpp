#pragma once
#include "../../Core/StockCore.h"

namespace StockCharts
{
	struct ExpInfo
	{
		String	rename;
		bool	renameAssign = false;
	};

	enum class EnExpColorType
	{
		None,
		LINE,
		DOTLINE,
		STICK,
		COLORSTICK,
	};

	enum class EnExpLineThick : int
	{
		None = 0,
		LINETHICK1,
		LINETHICK2,
		LINETHICK3,
		LINETHICK4,
		LINETHICK5,
		LINETHICK6,
		LINETHICK7,
		LINETHICK8,
		LINETHICK9,
	};

	struct ExpColor
	{
		EnExpColorType	type = EnExpColorType::None;
		EnExpLineThick	thick = EnExpLineThick::None;
		String			color;
	};

	enum class EnDrawingType
	{
		None,
		Number,
		Text,
		StickLine,
	};

	struct ExpDrawing
	{
		EnDrawingType	type = EnDrawingType::None;
		Number			stickWidth = 0.0;
		Number			stickEmpty = 0.0;
	};

	struct ExpCore
	{
		NumberCore	core;
		ExpInfo		info;
		ExpColor	color;
		ExpDrawing	drawing;
	};
}
