#pragma once
#include "Core/ResultIndex.h"

namespace StockCharts
{
	class IndicatorParser
	{
	public:
		IndicatorParser();
		~IndicatorParser();

		// [0]
		void setFormula(const IndexFormula& formula);

		// [1]
		void setStockCore(const StockCore& p);
		void setStockCore(std::shared_ptr<StockCore> p);
		void setStockExt(const StockRelyData& p);
		void setStockExt(std::shared_ptr<StockRelyData> p);

		// [2]
		bool run();

		// [3]
		const ResultIndex&				getResult();
		const std::set<EnStockRely>&	getStockRely();
		const IndexFormula&				getFormula();

	private:
		NumberCore		parseFormula();
		ExpColor		parseColor();

		NumberCore		parseRelationOperate();
		NumberCore		parseArithLastOperate();
		NumberCore		parseArithFirstOperate();
		NumberCore		parsePrimTokenOperate();

		EnParseToken	parseTokenValue();
		Number			parseNumberValue();

		bool			checkAssign(const std::string& name);
		NumberCore		getAssign(const std::string& name);

		bool			checkInputParam(const std::string& name);
		NumberCore		getInputParam(const std::string& name);

	private:
		std::unique_ptr<class FunctionParser>	m_spFunction;
		std::unique_ptr<class KeywordParser>	m_spKeyword;
		std::unique_ptr<class ColorParser>		m_spColor;
		std::unique_ptr<class DrawingParser>	m_spDrawing;

		IndexFormula			m_formula;

		ResultIndex				m_result;
		std::set<EnStockRely>	m_stockRely;
		bool					m_bParseError = false;
		std::string				m_errWord;

		std::string				m_expression;
		std::string::iterator	m_iteCurrent, m_iteEnd;
		EnParseToken			m_eToken = EnParseToken::ParseInit;
		Number					m_dValue = NumberCore::EmptyValue;
		std::string				m_sValue;
		ExpInfo					m_expInfo;
		ExpDrawing				m_expDrawing;
	};
}
