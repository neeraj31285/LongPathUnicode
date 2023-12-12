#pragma once


#include "GammaRuleCheckerı.hpp"
#include "UnusedParameter_visitorą.h"


class UnusedParameterChecker : public GammaRuleChecker<UnusedParameterVisitor> {
public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnusedParameterVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
