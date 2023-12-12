#pragma once


#include "GammaRuleCheckerı.hpp"
#include "Misra_2115_visitorങ.h"


class Misra_2115Checker : public GammaRuleChecker<Misra_2115Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2115Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
