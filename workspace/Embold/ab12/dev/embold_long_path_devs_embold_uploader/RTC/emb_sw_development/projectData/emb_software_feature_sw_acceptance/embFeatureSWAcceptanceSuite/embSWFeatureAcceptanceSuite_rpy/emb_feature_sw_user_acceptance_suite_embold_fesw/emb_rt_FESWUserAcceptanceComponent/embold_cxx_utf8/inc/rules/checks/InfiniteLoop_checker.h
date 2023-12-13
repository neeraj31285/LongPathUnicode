#pragma once


#include "GammaRuleChecker.hpp"
#include "InfiniteLoop_visitor.h"


class InfiniteLoopChecker : public GammaRuleChecker<InfiniteLoopVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InfiniteLoopVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};