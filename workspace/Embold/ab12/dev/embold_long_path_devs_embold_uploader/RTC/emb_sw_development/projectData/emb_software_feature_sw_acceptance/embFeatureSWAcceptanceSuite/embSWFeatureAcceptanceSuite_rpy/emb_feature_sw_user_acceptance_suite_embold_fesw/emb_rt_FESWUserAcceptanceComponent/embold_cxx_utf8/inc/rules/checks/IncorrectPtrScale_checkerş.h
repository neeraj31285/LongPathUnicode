#pragma once


#include "GammaRuleCheckerı.hpp"
#include "IncorrectPtrScale_visitorı.h"


class IncorrectPtrScaleChecker : public GammaRuleChecker<IncorrectPtrScaleVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, IncorrectPtrScaleVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
