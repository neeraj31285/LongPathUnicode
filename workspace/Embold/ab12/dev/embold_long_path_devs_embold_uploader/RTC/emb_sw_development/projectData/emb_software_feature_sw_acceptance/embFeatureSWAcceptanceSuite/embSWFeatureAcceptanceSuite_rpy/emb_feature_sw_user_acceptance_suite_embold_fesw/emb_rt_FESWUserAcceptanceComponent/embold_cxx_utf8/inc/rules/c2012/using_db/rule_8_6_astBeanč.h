#pragma once

#include "GammaASTBeanæ.h"

class Rule_8_6_astBean : public GammaASTBean
{
	PROPERTY(Line)
	PROPERTY(Symbol)
	PROPERTY(IsExtern)
	PROPERTY(FileName)

	Rule_8_6_astBean() : GammaASTBean (
	{
		Bind("line").setter(&Rule_8_6_astBean::setLine).getter(&Rule_8_6_astBean::getLine),
		Bind("symbol").setter(&Rule_8_6_astBean::setSymbol).getter(&Rule_8_6_astBean::getSymbol),
		Bind("isExtern").setter(&Rule_8_6_astBean::setIsExtern).getter(&Rule_8_6_astBean::getIsExtern),
		Bind("fileName").setter(&Rule_8_6_astBean::setFileName).getter(&Rule_8_6_astBean::getFileName)

	}) { }
};
