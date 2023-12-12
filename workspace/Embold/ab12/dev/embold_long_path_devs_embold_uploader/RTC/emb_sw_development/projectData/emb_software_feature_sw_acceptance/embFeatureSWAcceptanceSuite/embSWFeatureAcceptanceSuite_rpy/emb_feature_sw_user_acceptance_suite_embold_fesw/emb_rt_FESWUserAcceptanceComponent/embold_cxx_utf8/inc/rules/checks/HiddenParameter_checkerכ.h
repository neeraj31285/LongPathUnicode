#pragma once


#include "GammaRuleCheckerı.hpp"
#include "HiddenParameter_visitorג.h"


class HiddenParameterChecker : public GammaRuleChecker<HiddenParameterVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, HiddenParameterVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
