#include "KeywordParser.h"

using namespace StockCharts;

KeywordParser::KeywordParser()
{
	m_binds = {
		{ "OPEN",      std::bind(&KeywordParser::OPEN,      this)},
		{ "O",         std::bind(&KeywordParser::OPEN,      this)},
		{ "HIGH",      std::bind(&KeywordParser::HIGH,      this)},
		{ "H",         std::bind(&KeywordParser::HIGH,      this)},
		{ "LOW",       std::bind(&KeywordParser::LOW,       this)},
		{ "L",         std::bind(&KeywordParser::LOW,       this)},
		{ "CLOSE",     std::bind(&KeywordParser::CLOSE,     this)},
		{ "C",         std::bind(&KeywordParser::CLOSE,     this)},
		{ "VOL",       std::bind(&KeywordParser::VOL,       this)},
		{ "V",         std::bind(&KeywordParser::VOL,       this)},
		{ "AMOUNT",    std::bind(&KeywordParser::VOL,       this)},
		{ "ISLASTBAR", std::bind(&KeywordParser::ISLASTBAR, this)},
		{ "VOLV",      std::bind(&KeywordParser::VOLV,      this)},
		{ "IV",		   std::bind(&KeywordParser::IV,        this)},
	};
}

EnStockRely KeywordParser::stockRely(const std::string& name)
{
	if (name == "VOLV")
		return EnStockRely::VOLV;
	if (name == "IV")
		return EnStockRely::IV;
	return EnStockRely::None;
}

bool KeywordParser::check(const std::string& name)
{
	return (m_binds.find(name) != m_binds.end());
}

std::tuple<bool, NumberCore> KeywordParser::process(const std::string& name)
{
	auto ite = m_binds.find(name);
	if (ite == m_binds.end())
		return emptyRet();
	F& f = ite->second;
	return f();
}

std::tuple<bool, NumberCore> KeywordParser::OPEN()
{
	if (!m_spStockCore)
		return emptyRet(true);

	return { true, m_spStockCore->open };
}

std::tuple<bool, NumberCore> KeywordParser::HIGH()
{
	if (!m_spStockCore)
		return emptyRet(true);

	return { true, m_spStockCore->high };
}

std::tuple<bool, NumberCore> KeywordParser::LOW()
{
	if (!m_spStockCore)
		return emptyRet(true);

	return { true, m_spStockCore->low };
}

std::tuple<bool, NumberCore> KeywordParser::CLOSE()
{
	if (!m_spStockCore)
		return emptyRet(true);

	return { true, m_spStockCore->close };
}

std::tuple<bool, NumberCore> KeywordParser::VOL()
{
	if (!m_spStockCore)
		return emptyRet(true);

	return { true, m_spStockCore->vol };
}

std::tuple<bool, NumberCore> KeywordParser::ISLASTBAR()
{
	if (!m_spStockCore)
		return emptyRet(true);

	int rates_total = m_spStockCore->close.size();

	NumberCore buffer(rates_total, 0.0);
	buffer.back() = 1.0;
	return { true, buffer };
}

std::tuple<bool, NumberCore> KeywordParser::VOLV()
{
	if (!m_spStockExt)
		return emptyRet(true);

	return { true, m_spStockExt->at(EnStockRely::VOLV) };
}

std::tuple<bool, NumberCore> KeywordParser::IV()
{
	if (!m_spStockExt)
		return emptyRet(true);

	return { true, m_spStockExt->at(EnStockRely::IV) };
}
