#pragma once
#include "SubParser.h"

namespace StockCharts
{
	class ColorParser : public HSLib
	{
	public:
		ColorParser();

		bool check(const String& name);
		std::tuple<bool, ExpColor> process(const String& name);

	private:
		std::tuple<bool, ExpColor> emptyRet(bool ok = false)
		{
			return { ok, ExpColor() };
		}

		bool checkCustomColor(const String& name);

		std::tuple<bool, ExpColor> colorType(const String& name);
		std::tuple<bool, ExpColor> lineThick(const String& name);
		std::tuple<bool, ExpColor> colorString(const String& name);

	private:
		using F = std::function<std::tuple<bool, ExpColor>(const String&)>;
		std::vector<F>							m_binds;
		std::map<String, EnExpColorType>	m_types;
		std::map<String, EnExpLineThick>	m_thicks;
		std::map<String, String>		m_colors;
	};
}
