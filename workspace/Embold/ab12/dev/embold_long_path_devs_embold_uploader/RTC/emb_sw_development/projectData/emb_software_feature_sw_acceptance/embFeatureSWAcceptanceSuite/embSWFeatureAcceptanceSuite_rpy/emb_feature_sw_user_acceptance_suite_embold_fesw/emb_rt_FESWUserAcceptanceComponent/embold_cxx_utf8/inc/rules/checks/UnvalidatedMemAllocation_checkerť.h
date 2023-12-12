#pragma once


#include "GammaRuleCheckerı.hpp"
#include "UnvalidatedMemAllocation_visitorå.h"


class UnvalidatedMemAllocChecker : public GammaRuleChecker<UnvalidatedMemAllocVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedMemAllocVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
