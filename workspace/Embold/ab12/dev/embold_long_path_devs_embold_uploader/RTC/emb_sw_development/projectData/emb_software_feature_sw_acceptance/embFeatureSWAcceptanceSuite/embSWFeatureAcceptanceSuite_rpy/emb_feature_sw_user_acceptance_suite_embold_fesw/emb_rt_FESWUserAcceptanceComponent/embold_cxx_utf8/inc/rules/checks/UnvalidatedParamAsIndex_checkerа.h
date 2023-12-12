#pragma once

#include "GammaRuleCheckerı.hpp"
#include "UnvalidatedParamAsIndex_visitorঅ.h"


class UnvalidatedParamAsIndexChecker : public GammaRuleChecker<UnvalidatedParamAsIndexVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedParamAsIndexVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
