#pragma once
#include "ResultExp.h"
#include <map>
#include <set>

namespace StockCharts
{
	enum class EnStockRely
	{
		None,
		VOLV,
		IV,
	};
	using StockRelyData = std::map<EnStockRely, NumberCore>; //todo

	struct IndexFormula
	{
		std::string					name;
		bool						sub = true;
		std::string					expression;
		std::map<std::string, int>	params;
	};

	struct ResultIndex
	{
		std::vector<ResultExp>	exps;
		bool					err = false;
		std::string				errExpression;
		std::string				errWord;
	};

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
}
