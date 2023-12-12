#pragma once

#pragma warning(push,0)
#include "GammaASTBeanæ.h"
#pragma warning(pop)

class Rule_8_7_astBean : public GammaASTBean
{
	PROPERTY(Line)
	PROPERTY(Symbol)
	PROPERTY(IsDecl)
	PROPERTY(FileName)

	Rule_8_7_astBean() : GammaASTBean (
	{
		Bind("line").setter(&Rule_8_7_astBean::setLine).getter(&Rule_8_7_astBean::getLine),
		Bind("isDecl").setter(&Rule_8_7_astBean::setIsDecl).getter(&Rule_8_7_astBean::getIsDecl),
		Bind("symbol").setter(&Rule_8_7_astBean::setSymbol).getter(&Rule_8_7_astBean::getSymbol),
		Bind("fileName").setter(&Rule_8_7_astBean::setFileName).getter(&Rule_8_7_astBean::getFileName)

	}) { }
};
