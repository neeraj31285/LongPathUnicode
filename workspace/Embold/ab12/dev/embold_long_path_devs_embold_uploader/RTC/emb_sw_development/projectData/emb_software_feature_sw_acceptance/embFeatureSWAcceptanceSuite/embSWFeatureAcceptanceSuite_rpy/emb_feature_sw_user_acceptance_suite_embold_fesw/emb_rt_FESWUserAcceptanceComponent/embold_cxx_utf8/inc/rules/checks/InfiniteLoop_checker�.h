#pragma once


#include "GammaRuleCheckerı.hpp"
#include "InfiniteLoop_visitorž.h"


class InfiniteLoopChecker : public GammaRuleChecker<InfiniteLoopVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InfiniteLoopVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
