/****************************************************************************
** MIT License
** 
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
** 
****************************************************************************/
#pragma once
#include "../../Core/StockCore.h"
#include "../Core/IndexCore.h"

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
        void setStockCore(std::shared_ptr<const StockCore> p);
        void setStockExt(const StockRelyData& p);
        void setStockExt(std::shared_ptr<StockRelyData> p);

        // [2]
        bool run();

        // [3]
        const IndexCore& getResult();
        const IndexFormula& getFormula();
        const std::set<EnStockRely>& getStockRely();

    private:
        enum class EnParseToken
        {
            ParseInit,
            String,
            Number,
            Rename,			// :
            RenameAssign,	// :=
            TokenLT,		// <
            TokenLE,		// <=
            TokenGT,		// >
            TokenGE,		// >=
            TokenEQ,		// =, ==
            TokenNEQ,		// !=
            TokenNOT,		// !
            TokenPlus,		// +
            TokenMinus,		// -
            TokenMul,		// *
            TokenDiv,		// /
            TokenMod,		// %
            TokenPow,		// ^
            TokenAnd,		// &&
            TokenOr,		// ||
            TokenComma,		// ,
            TokenLP,		// (
            TokenRP,		// )
            TokenFinal,		// ;
        };

    private:
        NumberCore		parseFormula();
        ExpColor		parseColor();

        NumberCore		parseRelationOperate();
        NumberCore		parseArithLastOperate();
        NumberCore		parseArithFirstOperate();
        NumberCore		parsePrimTokenOperate();

        EnParseToken	parseTokenValue();
        Number			parseNumberValue();

        bool			checkAssign(const String& name);
        NumberCore		getAssign(const String& name);

        bool			checkInputParam(const String& name);
        NumberCore		getInputParam(const String& name);

    private:
        IndexFormula    m_formula;

        std::unique_ptr<class FunctionParser>	m_spFunction;
        std::unique_ptr<class KeywordParser>	m_spKeyword;
        std::unique_ptr<class ColorParser>		m_spColor;
        std::unique_ptr<class DrawingParser>	m_spDrawing;

        IndexCore				m_result;
        std::set<EnStockRely>	m_stockRely;
        bool					m_bParseError = false;
        String					m_errWord;

        String				m_expression;
        String::iterator	m_iteCurrent, m_iteEnd;
        EnParseToken		m_eToken = EnParseToken::ParseInit;
        Number				m_dValue = NumberCore::EmptyNumber;
        String				m_sValue;
        ExpInfo				m_expInfo;
        ExpDrawing			m_expDrawing;
    };
}
