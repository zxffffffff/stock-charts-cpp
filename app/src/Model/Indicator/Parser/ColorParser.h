#pragma once
#include "SubParser.h"

namespace StockCharts
{
	class ColorParser : public HSLib
	{
	public:
		ColorParser();

		bool check(const std::string& name);
		std::tuple<bool, ExpColor> process(const std::string& name);

	private:
		std::tuple<bool, ExpColor> emptyRet(bool ok = false)
		{
			return { ok, ExpColor() };
		}

		bool checkCustomColor(const std::string& name);

		std::tuple<bool, ExpColor> colorType(const std::string& name);
		std::tuple<bool, ExpColor> lineThick(const std::string& name);
		std::tuple<bool, ExpColor> colorString(const std::string& name);

	private:
		using F = std::function<std::tuple<bool, ExpColor>(const std::string&)>;
		std::vector<F>							m_binds;
		std::map<std::string, EnExpColorType>	m_types;
		std::map<std::string, EnExpLineThick>	m_thicks;
		std::map<std::string, std::string>		m_colors;
	};
}
