#pragma once

#include "GammaRuleChecker.hpp"
#include "UnvalidatedParamAsIndex_visitor.h"


class UnvalidatedParamAsIndexChecker : public GammaRuleChecker<UnvalidatedParamAsIndexVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedParamAsIndexVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};