/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#include "DrawingParser.h"

using namespace StockCharts;

DrawingParser::DrawingParser()
{
	m_binds = {
		{ "DRAWNUMBER", std::bind(&DrawingParser::number,    this, std::placeholders::_1)},
		{ "DRAWTEXT",   std::bind(&DrawingParser::text,      this, std::placeholders::_1)},
		{ "STICKLINE",  std::bind(&DrawingParser::stickLine, this, std::placeholders::_1)},
	};
}

bool DrawingParser::check(const String& name)
{
	return (m_binds.find(name) != m_binds.end());
}

std::tuple<bool, ExpDrawing, NumberCore> DrawingParser::process(const String& name, const std::vector<NumberCore>& inputs)
{
	auto ite = m_binds.find(name);
	if (ite == m_binds.end())
		return emptyRet();
	F& f = ite->second;
	return f(inputs);
}

std::tuple<bool, ExpDrawing, NumberCore> DrawingParser::number(const std::vector<NumberCore>& inputs)
{
	if (inputs.size() != 3)
		return emptyRet();

	const NumberCore& cond = inputs[0];
	const NumberCore& price = inputs[1];
	const NumberCore& num = inputs[2];

	if (cond.isEmpty() || price.isEmpty() || num.isEmpty())
		return emptyRet(true);

	int rates_total = price.size();
	int num_total = num.size();
	NumberCore buffer(rates_total);

	for (int i = 0; i < rates_total; ++i) {
		Number b = cond.safeAt(i);
		if (b == 0.0 || b == NumberCore::EmptyNumber)
			continue;
		buffer[i] = price[i];

		Number n = (num_total == 1 ? num[0] : num.safeAt(i));
		buffer.setOther(i, std::to_string(int64_t(n)));
	}

	ExpDrawing drawing;
	drawing.type = EnDrawingType::Number;
	return { true, std::move(drawing), buffer };
}

std::tuple<bool, ExpDrawing, NumberCore> DrawingParser::text(const std::vector<NumberCore>& inputs)
{
	if (inputs.size() != 3)
		return emptyRet();

	const NumberCore& cond = inputs[0];
	const NumberCore& price = inputs[1];
	const NumberCore& text = inputs[2];

	if (cond.isEmpty() || price.isEmpty() || text.isEmpty())
		return emptyRet(true);

	int rates_total = price.size();
	int text_total = text.size();
	NumberCore buffer(rates_total);

	ExpDrawing drawing;
	drawing.type = EnDrawingType::Text;
	return { true, std::move(drawing), buffer };
}

std::tuple<bool, ExpDrawing, NumberCore> DrawingParser::stickLine(const std::vector<NumberCore>& inputs)
{
	if (inputs.size() != 5)
		return emptyRet();

	const NumberCore& cond = inputs[0];
	const NumberCore& price = inputs[1];
	const NumberCore& price2 = inputs[2];
	Number width = inputs[3].safeAt(0);
	Number empty = inputs[4].safeAt(0);

	if (cond.isEmpty() || price.isEmpty() || price2.isEmpty() || width == NumberCore::EmptyNumber || empty == NumberCore::EmptyNumber)
		return emptyRet(true);

	int rates_total = std::max(price.size(), price2.size());
	NumberCore buffer(rates_total);

	for (int i = 0; i < rates_total; ++i) {
		Number b = cond.safeAt(i);
		if (!b || b == NumberCore::EmptyNumber)
			continue;
		buffer[i] = price.safeAt(i);
		buffer.setOther(i, std::to_string(price2[i]));
	}

	ExpDrawing drawing;
	drawing.type = EnDrawingType::StickLine;
	drawing.stickWidth = width;
	drawing.stickEmpty = empty;
	return { true, std::move(drawing), buffer };
}
