#pragma once


#include "GammaRuleChecker.hpp"
#include "UnconditionalPtrDeref_visitor.h"


class UnconditionalPtrDerefChecker : public GammaRuleChecker<UnconditionalPtrDerefVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnconditionalPtrDerefVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
