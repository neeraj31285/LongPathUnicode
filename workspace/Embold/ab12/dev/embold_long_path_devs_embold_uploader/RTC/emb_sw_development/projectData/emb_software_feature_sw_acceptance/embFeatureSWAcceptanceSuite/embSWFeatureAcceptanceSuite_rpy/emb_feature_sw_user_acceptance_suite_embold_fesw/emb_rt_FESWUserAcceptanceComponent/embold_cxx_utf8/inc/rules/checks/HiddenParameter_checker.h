#pragma once


#include "GammaRuleChecker.hpp"
#include "HiddenParameter_visitor.h"


class HiddenParameterChecker : public GammaRuleChecker<HiddenParameterVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, HiddenParameterVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
