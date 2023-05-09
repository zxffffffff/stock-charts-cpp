/****************************************************************************
** MIT License
**
** Author   : xiaofeng.zhu
** Support  : zxffffffff@outlook.com, 1337328542@qq.com
**
****************************************************************************/
#pragma once
#include "../../Core/Utils.h"
#include "Parser/FunctionParser.h"
#include "Parser/KeywordParser.h"
#include "Parser/ColorParser.h"
#include "Parser/DrawingParser.h"

namespace StockCharts
{
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

    class IndicatorParser
    {
    public:
        IndicatorParser()
        {
            m_spFunction = std::make_unique<FunctionParser>();
            m_spKeyword = std::make_unique<KeywordParser>();
            m_spColor = std::make_unique<ColorParser>();
            m_spDrawing = std::make_unique<DrawingParser>();
        }
        ~IndicatorParser() = default;

        // [0]
        void setFormula(const IndexFormula& formula)
        {
            m_formula = formula;
        }

        // [1]
        void setStockCore(const StockCore& p)
        {
            setStockCore(std::make_shared<StockCore>(p));
        }
        void setStockCore(std::shared_ptr<const StockCore> p)
        {
            m_spFunction->setStockCore(p);
            m_spKeyword->setStockCore(p);
            m_spColor->setStockCore(p);
            m_spDrawing->setStockCore(p);
        }

        void setStockExt(const StockRelyData& p)
        {
            setStockExt(std::make_shared<StockRelyData>(p));
        }
        void setStockExt(std::shared_ptr<StockRelyData> p)
        {
            m_spFunction->setStockExt(p);
            m_spKeyword->setStockExt(p);
            m_spColor->setStockExt(p);
            m_spDrawing->setStockExt(p);
        }

        // [2]
        bool run()
        {
            m_result = IndexCore();
            m_stockRely.clear();
            m_bParseError = false;
            m_errWord.clear();

            std::vector<std::string> expressions = Utils::splitStr(m_formula.expression, ';');
            int i;
            for (i = 0; i < expressions.size(); ++i) {
                m_expression = Utils::to8bitStr(expressions[i]) + ";";
                if (Utils::checkEmpty(m_expression))
                    continue;
                if (m_expression.back() != '\0')
                    m_expression += '\0';

                m_iteCurrent = m_expression.begin();
                m_iteEnd = m_expression.end();
                m_eToken = EnParseToken::ParseInit;
                m_dValue = NumberNull;
                m_sValue.clear();
                m_expInfo = ExpInfo();
                m_expDrawing = ExpDrawingType();

                try {
                    parseTokenValue();
                    NumberCore core = parseFormula();
                    if (m_bParseError)
                        goto RunErr;

                    ExpColorType expColor = parseColor();
                    if (m_bParseError)
                        goto RunErr;

                    ExpCore exp;
                    exp.core = std::move(core);
                    exp.info = std::move(m_expInfo);
                    exp.colorType = std::move(expColor);
                    exp.drawingType = std::move(m_expDrawing);
                    m_result.exps.push_back(std::move(exp));
                }
                catch (...) {
                    goto RunErr;
                }
            }
            return true;

        RunErr:
            m_result.err = true;
            m_result.errExpression = expressions[i];
            m_result.errWord = m_errWord;
            return false;
        }

        // [3]
        const IndexCore& getResult()
        {
            return m_result;
        }

        const IndexFormula& getFormula()
        {
            return m_formula;
        }

        const std::set<EnStockRely>& getStockRely()
        {
            return m_stockRely;
        }

    private:
        NumberCore parseFormula()
        {
            NumberCore coreLeft = parseRelationOperate();

            if (m_bParseError)
                return coreLeft;

            for (;;) {
                switch (m_eToken)
                {
                case EnParseToken::TokenAnd:
                    parseTokenValue();
                    coreLeft = (coreLeft && parseRelationOperate());
                    break;
                case EnParseToken::TokenOr:
                    parseTokenValue();
                    coreLeft = (coreLeft || parseRelationOperate());
                    break;

                default:
                    return coreLeft;
                }

                if (m_bParseError)
                    return coreLeft;
            }
        }

        ExpColorType parseColor()
        {
            ExpColorType expColor;
            expColor.type = EnExpLineType::LINE;
            expColor.thick = EnExpLineThick::LINETHICK1;
            expColor.color = "8080FF";

            if (!*m_iteCurrent)
                return expColor;

            for (;;) {
                parseTokenValue();
                if (m_eToken != EnParseToken::String) {
                    m_bParseError = true;
                    return expColor;
                }

                std::string name = std::move(m_sValue);
                if (m_spColor->check(name)) {
                    bool ok;
                    ExpColorType expTemp;
                    std::tie(ok, expTemp) = m_spColor->process(name);
                    if (!ok) {
                        m_bParseError = true;
                        m_errWord = name;
                        return expColor;
                    }

                    if (expTemp.type != EnExpLineType::None)
                        expColor.type = expTemp.type;
                    if (expTemp.thick != EnExpLineThick::None)
                        expColor.thick = expTemp.thick;
                    if (!expTemp.color.empty())
                        expColor.color = expTemp.color;
                }
                else {
                    m_bParseError = true;
                    m_errWord = name;
                    return expColor;
                }

                parseTokenValue();
                if (m_eToken == EnParseToken::TokenComma)
                    continue;
                if (m_eToken == EnParseToken::TokenFinal)
                    return expColor;

                m_bParseError = true;
                return expColor;
            }
        }

        NumberCore parseRelationOperate()
        {
            NumberCore coreLeft = parseArithLastOperate();

            if (m_bParseError)
                return coreLeft;

            for (;;) {
                switch (m_eToken)
                {
                case EnParseToken::TokenLT:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft < coreRight;
                    break;
                }
                case EnParseToken::TokenLE:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft <= coreRight;
                    break;
                }
                case EnParseToken::TokenGT:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft > coreRight;
                    break;
                }
                case EnParseToken::TokenGE:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft >= coreRight;
                    break;
                }
                case EnParseToken::TokenEQ:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft == coreRight;
                    break;
                }
                case EnParseToken::TokenNEQ:
                {
                    parseTokenValue();
                    NumberCore coreRight = parseArithLastOperate();
                    coreLeft = coreLeft != coreRight;
                    break;
                }
                default:
                    return coreLeft;
                }

                if (m_bParseError)
                    return coreLeft;
            }
        }

        NumberCore parseArithLastOperate()
        {
            NumberCore coreLeft = parseArithFirstOperate();

            if (m_bParseError)
                return coreLeft;

            for (;;) {
                switch (m_eToken)
                {
                case EnParseToken::TokenPlus:
                    parseTokenValue();
                    coreLeft = coreLeft + parseArithFirstOperate();
                    break;
                case EnParseToken::TokenMinus:
                    parseTokenValue();
                    coreLeft = coreLeft - parseArithFirstOperate();
                    break;
                default:
                    return coreLeft;
                }

                if (m_bParseError)
                    return coreLeft;
            }
        }

        NumberCore parseArithFirstOperate()
        {
            NumberCore coreLeft = parsePrimTokenOperate();

            if (m_bParseError)
                return coreLeft;

            NumberCore coreTemp;
            for (;;) {
                switch (m_eToken)
                {
                case EnParseToken::TokenMul:
                    parseTokenValue();
                    coreTemp = parsePrimTokenOperate();
                    coreLeft = coreLeft * coreTemp;
                    break;

                case EnParseToken::TokenDiv:
                    parseTokenValue();
                    coreTemp = parsePrimTokenOperate();
                    coreLeft = coreLeft / coreTemp;
                    break;

                case EnParseToken::TokenMod:
                    parseTokenValue();
                    coreTemp = parsePrimTokenOperate();
                    coreLeft = coreLeft % coreTemp;
                    break;
                default:
                    return coreLeft;
                }

                if (m_bParseError)
                    return coreLeft;
            }
        }

        NumberCore parsePrimTokenOperate()
        {
            NumberCore coreToken;
            std::string name = std::move(m_sValue);

            switch (m_eToken)
            {
            case EnParseToken::Number:
                parseTokenValue();
                coreToken = m_dValue;
                m_dValue = NumberNull;
                return coreToken;

            case EnParseToken::String:
                parseTokenValue();

                if (m_eToken == EnParseToken::Rename || m_eToken == EnParseToken::RenameAssign) {
                    if (m_eToken == EnParseToken::RenameAssign)
                        m_expInfo.renameAssign = true;
                    m_expInfo.rename = name;
                    parseTokenValue();
                    return parseFormula();
                }
                else if (checkAssign(name)) {
                    return getAssign(name);
                }
                else if (checkInputParam(name)) {
                    return getInputParam(name);
                }
                else if (m_spKeyword->check(name)) {
                    auto rely = m_spKeyword->stockRely(name);
                    if (!rely.empty())
                        m_stockRely.insert(rely.begin(), rely.end());

                    bool ok;
                    NumberCore coreResult;
                    std::tie(ok, coreResult) = m_spKeyword->process(name);
                    if (!ok) {
                        m_bParseError = true;
                        m_errWord = name;
                        return coreResult;
                    }
                    return coreResult;
                }
                else if (m_spFunction->check(name)) {
                    NumberCore coreResult;
                    if (m_eToken == EnParseToken::TokenLP) {
                        std::vector<NumberCore> params;
                        parseTokenValue();
                        params.push_back(parseFormula());
                        while (m_eToken == EnParseToken::TokenComma)
                            params.push_back(parseFormula());

                        if (m_eToken != EnParseToken::TokenRP) {
                            m_bParseError = true;
                            m_errWord = name;
                            return coreResult;
                        }

                        bool ok;
                        std::tie(ok, coreResult) = m_spFunction->process(name, params);
                        if (!ok) {
                            m_bParseError = true;
                            m_errWord = name;
                            return coreResult;
                        }

                        if (*m_iteCurrent) {
                            parseTokenValue();
                        }
                        return coreResult;
                    }
                    else {
                        return coreResult;
                    }
                }
                else if (m_spDrawing->check(name)) {
                    NumberCore coreResult;
                    if (m_eToken == EnParseToken::TokenLP) {
                        std::vector<NumberCore> params;
                        parseTokenValue();
                        params.push_back(parseFormula());
                        while (m_eToken == EnParseToken::TokenComma)
                            params.push_back(parseFormula());

                        if (m_eToken != EnParseToken::TokenRP) {
                            m_bParseError = true;
                            m_errWord = name;
                            return coreResult;
                        }

                        bool ok;
                        ExpDrawingType drawingType;
                        std::tie(ok, drawingType, coreResult) = m_spDrawing->process(name, params);
                        if (!ok) {
                            m_bParseError = true;
                            m_errWord = name;
                            return coreResult;
                        }
                        m_expDrawing = drawingType;

                        if (*m_iteCurrent) {
                            parseTokenValue();
                        }
                        return coreResult;
                    }
                    else {
                        return coreResult;
                    }
                }
                else {
                    m_bParseError = true;
                    m_errWord = name;
                    return NumberCore();
                }

            case EnParseToken::TokenMinus:
                parseTokenValue();
                return parsePrimTokenOperate() * NumberCore(-1.0);

            case EnParseToken::TokenPlus:
                parseTokenValue();
                return parsePrimTokenOperate();

            case EnParseToken::TokenNOT:
                parseTokenValue();
                return !parsePrimTokenOperate();

            case EnParseToken::TokenAnd:
            case EnParseToken::TokenOr:
                return coreToken;

            case EnParseToken::TokenLP:
            {
                parseTokenValue();
                NumberCore coreResult = parseFormula();
                if (m_eToken != EnParseToken::TokenRP) {
                    return coreResult;
                }
                parseTokenValue();
                return coreResult;
            }

            case EnParseToken::TokenComma:
                parseTokenValue();
                return parsePrimTokenOperate();

            default:
                m_bParseError = true;
                return coreToken;
            }
        }

        EnParseToken parseTokenValue()
        {
            switch (*m_iteCurrent)
            {
            case ';':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenFinal;

            case ':':
                m_iteCurrent++;
                if (*m_iteCurrent == '=') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::RenameAssign;
                }
                return m_eToken = EnParseToken::Rename;

            case '\r':
            case '\n':
            case '\t':
            case '\v':
            case ' ':
                m_iteCurrent++;
                return m_eToken = parseTokenValue();

            case '*':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenMul;
            case '/':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenDiv;
            case '+':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenPlus;
            case '-':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenMinus;
            case '(':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenLP;
            case ')':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenRP;
            case '%':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenMod;
            case ',':
                m_iteCurrent++;
                return m_eToken = EnParseToken::TokenComma;

            case '<':
                m_iteCurrent++;
                if (*m_iteCurrent == '=') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenLE;
                }
                return m_eToken = EnParseToken::TokenLT;
            case '>':
                m_iteCurrent++;
                if (*m_iteCurrent == '=') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenGE;
                }
                return m_eToken = EnParseToken::TokenGT;
            case '=':
                m_iteCurrent++;
                if (*m_iteCurrent == '=') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenEQ;
                }
                return m_eToken = EnParseToken::TokenEQ;

            case '&':
                m_iteCurrent++;
                if (*m_iteCurrent == '&') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenAnd;
                }
                return m_eToken = EnParseToken::TokenAnd;
            case '|':
                m_iteCurrent++;
                if (*m_iteCurrent == '|') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenOr;
                }
                return m_eToken = EnParseToken::TokenOr;
            case '!':
                m_iteCurrent++;
                if (*m_iteCurrent == '=') {
                    m_iteCurrent++;
                    return m_eToken = EnParseToken::TokenNEQ;
                }
                return m_eToken = EnParseToken::TokenNOT;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                m_dValue = parseNumberValue();
                return m_eToken = EnParseToken::Number;

            default:
                if (std::isalpha(*m_iteCurrent) || *m_iteCurrent == '_') {
                    m_sValue.clear();
                    while (std::isalpha(*m_iteCurrent) || std::isdigit(*m_iteCurrent) || *m_iteCurrent == '_') {
                        m_sValue += *m_iteCurrent++;
                    }

                    if (m_sValue == "AND") {
                        m_sValue.clear();
                        return m_eToken = EnParseToken::TokenAnd;
                    }
                    else if (m_sValue == "OR") {
                        m_sValue.clear();
                        return m_eToken = EnParseToken::TokenOr;
                    }
                    else if (m_sValue == "NOT") {
                        m_sValue.clear();
                        return m_eToken = EnParseToken::TokenNOT;
                    }

                    return m_eToken = EnParseToken::String;
                }
                else if (!*m_iteCurrent) {
                    return m_eToken = EnParseToken::TokenFinal;
                }
                m_bParseError = true;
                return m_eToken;
            }
        }

        Number parseNumberValue()
        {
            std::stringstream ss;
            while (std::isdigit(*m_iteCurrent) || *m_iteCurrent == '.')
                ss << *m_iteCurrent++;
            Number dValue = std::stold(ss.str());
            return dValue;
        }

        bool checkAssign(const std::string& name)
        {
            for (auto& result : m_result.exps) {
                if (result.info.rename == name)
                    return true;
            }
            return false;
        }
        NumberCore getAssign(const std::string& name)
        {
            for (auto& result : m_result.exps) {
                if (result.info.rename == name)
                    return result.core;
            }
            return NumberCore();
        }

        bool checkInputParam(const std::string& name)
        {
            auto ite = m_formula.params.find(name);
            return (ite != m_formula.params.end());
        }
        NumberCore getInputParam(const std::string& name)
        {
            NumberCore coreResult;
            auto ite = m_formula.params.find(name);
            if (ite != m_formula.params.end())
                coreResult = Number(ite->second);
            return coreResult;
        }

    private:
        IndexFormula    m_formula;

        std::unique_ptr<class FunctionParser>	m_spFunction;
        std::unique_ptr<class KeywordParser>	m_spKeyword;
        std::unique_ptr<class ColorParser>		m_spColor;
        std::unique_ptr<class DrawingParser>	m_spDrawing;

        IndexCore				m_result;
        std::set<EnStockRely>	m_stockRely;
        bool					m_bParseError = false;
        std::string				m_errWord;

        std::string				m_expression;
        std::string::iterator	m_iteCurrent, m_iteEnd;
        EnParseToken		    m_eToken = EnParseToken::ParseInit;
        Number				    m_dValue = NumberNull;
        std::string				m_sValue;
        ExpInfo				    m_expInfo;
        ExpDrawingType			m_expDrawing;
    };
}
