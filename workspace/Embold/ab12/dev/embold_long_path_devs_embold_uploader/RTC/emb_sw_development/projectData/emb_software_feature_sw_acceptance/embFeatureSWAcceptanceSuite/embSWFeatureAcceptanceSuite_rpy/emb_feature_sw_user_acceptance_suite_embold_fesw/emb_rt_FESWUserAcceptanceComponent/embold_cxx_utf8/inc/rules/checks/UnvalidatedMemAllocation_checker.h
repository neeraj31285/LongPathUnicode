#pragma once


#include "GammaRuleChecker.hpp"
#include "UnvalidatedMemAllocation_visitor.h"


class UnvalidatedMemAllocChecker : public GammaRuleChecker<UnvalidatedMemAllocVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedMemAllocVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};