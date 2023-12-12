#pragma once


#include "GammaRuleCheckerı.hpp"
#include "UnconditionalPtrDeref_visitorď.h"


class UnconditionalPtrDerefChecker : public GammaRuleChecker<UnconditionalPtrDerefVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnconditionalPtrDerefVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
