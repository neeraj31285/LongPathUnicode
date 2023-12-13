#pragma once


#include "GammaRuleChecker.hpp"
#include "UnusedParameter_visitor.h"


class UnusedParameterChecker : public GammaRuleChecker<UnusedParameterVisitor> {
public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnusedParameterVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
