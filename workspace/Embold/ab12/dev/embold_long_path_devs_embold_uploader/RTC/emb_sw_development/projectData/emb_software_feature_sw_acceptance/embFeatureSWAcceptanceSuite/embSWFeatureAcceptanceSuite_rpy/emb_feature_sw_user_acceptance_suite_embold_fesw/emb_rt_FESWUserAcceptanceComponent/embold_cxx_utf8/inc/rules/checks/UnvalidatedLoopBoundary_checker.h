#pragma once


#include "GammaRuleChecker.hpp"
#include "UnvalidatedLoopBoundary_visitor.h"


class UnvalidatedLoopBoundaryChecker : public GammaRuleChecker<UnvalidatedLoopBoundaryVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedLoopBoundaryVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};