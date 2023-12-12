#pragma once


#include "GammaRuleCheckerı.hpp"
#include "MainReturnsVoid_visitorה.h"


class MainReturnsVoidChecker : public GammaRuleChecker<MainReturnsVoidVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, MainReturnsVoidVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
