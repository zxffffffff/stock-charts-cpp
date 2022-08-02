/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "ColorParser.h"

using namespace StockCharts;

ColorParser::ColorParser()
{
	m_binds = {
		std::bind(&ColorParser::colorType,   this, std::placeholders::_1),
		std::bind(&ColorParser::lineThick,   this, std::placeholders::_1),
		std::bind(&ColorParser::colorString, this, std::placeholders::_1),
	};

	m_types = {
		{ "DOTLINE",    EnExpColorType::DOTLINE    },
		{ "STICK",      EnExpColorType::STICK      },
		{ "COLORSTICK", EnExpColorType::COLORSTICK },
	};
	
	m_thicks = {
		{ "LINETHICK1", EnExpLineThick::LINETHICK1 },
		{ "LINETHICK2", EnExpLineThick::LINETHICK2 },
		{ "LINETHICK3", EnExpLineThick::LINETHICK3 },
		{ "LINETHICK4", EnExpLineThick::LINETHICK4 },
		{ "LINETHICK5", EnExpLineThick::LINETHICK5 },
		{ "LINETHICK6", EnExpLineThick::LINETHICK6 },
		{ "LINETHICK7", EnExpLineThick::LINETHICK7 },
		{ "LINETHICK8", EnExpLineThick::LINETHICK8 },
		{ "LINETHICK9", EnExpLineThick::LINETHICK9 },
	};

	m_colors = {
		{ "COLORBLACK",   "000000" },
		{ "COLORWHITE",   "FFFFFF" },
		{ "COLORRED",     "FF0000" },
		{ "COLORGREEN",   "00FF00" },
		{ "COLORBLUE",    "0000FF" },
		{ "COLORGRAY",    "808080" },
		{ "COLORMAGENTA", "E04080" },
		{ "COLORLICYAN",  "2F4640" },
	};
}

bool ColorParser::check(const String& name)
{
	if (m_types.find(name) != m_types.end())
		return true;
	
	if (m_thicks.find(name) != m_thicks.end())
		return true;

	if (m_colors.find(name) != m_colors.end())
		return true;
	if (checkCustomColor(name))
		return true;

	return false;
}

std::tuple<bool, ExpColor> ColorParser::process(const String& name)
{
	bool ok = false;
	ExpColor expColor;

	for (F& f : m_binds) {
		std::tie(ok, expColor) = f(name);
		if (ok)
			return { true, expColor };
	}
	return emptyRet();
}

bool ColorParser::checkCustomColor(const String& name)
{
	//COLORE123456
	if (name.length() != 11)
		return false;
	if (name.compare(0, 5, "COLOR") != 0)
		return false;
	for (int i = 5; i < 11; ++i) {
		char c = name[i];
		if (c >= '0' && c <= '9')
			continue;
		if (c >= 'A' && c <= 'F')
			continue;
		return false;
	}
	return true;
}

std::tuple<bool, ExpColor> ColorParser::colorType(const String& name)
{
	auto ite = m_types.find(name);
	if (ite == m_types.end())
		return emptyRet();

	ExpColor expColor;
	expColor.type = ite->second;
	return { true, std::move(expColor) };
}

std::tuple<bool, ExpColor> ColorParser::lineThick(const String& name)
{
	auto ite = m_thicks.find(name);
	if (ite == m_thicks.end())
		return emptyRet();

	ExpColor expColor;
	expColor.thick = ite->second;
	return { true, std::move(expColor) };
}

std::tuple<bool, ExpColor> ColorParser::colorString(const String& name)
{
	auto ite = m_colors.find(name);
	if (ite == m_colors.end()) {
		if (checkCustomColor(name)) {
			ExpColor expColor;
			expColor.color = name.substr(5);
			return { true, std::move(expColor) };
		}
		return emptyRet();
	}
	ExpColor expColor;
	expColor.color = ite->second;
	return { true, std::move(expColor) };
}
