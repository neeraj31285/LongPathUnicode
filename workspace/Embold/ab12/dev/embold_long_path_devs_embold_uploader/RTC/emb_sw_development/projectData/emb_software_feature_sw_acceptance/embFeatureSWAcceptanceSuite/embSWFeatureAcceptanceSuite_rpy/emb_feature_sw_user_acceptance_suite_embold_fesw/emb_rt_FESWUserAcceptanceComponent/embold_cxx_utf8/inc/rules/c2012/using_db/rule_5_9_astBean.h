#pragma once

#include "GammaASTBean.h"

class Rule_5_9_astBean : public GammaASTBean
{
	PROPERTY(Line)
	PROPERTY(Symbol)
	PROPERTY(IsStatic)
	PROPERTY(FileName)

	Rule_5_9_astBean() : GammaASTBean (
	{
		Bind("line").setter(&Rule_5_9_astBean::setLine).getter(&Rule_5_9_astBean::getLine),
		Bind("isStatic").setter(&Rule_5_9_astBean::setIsStatic).getter(&Rule_5_9_astBean::getIsStatic),
		Bind("symbol").setter(&Rule_5_9_astBean::setSymbol).getter(&Rule_5_9_astBean::getSymbol),
		Bind("fileName").setter(&Rule_5_9_astBean::setFileName).getter(&Rule_5_9_astBean::getFileName)

	}) { }
};