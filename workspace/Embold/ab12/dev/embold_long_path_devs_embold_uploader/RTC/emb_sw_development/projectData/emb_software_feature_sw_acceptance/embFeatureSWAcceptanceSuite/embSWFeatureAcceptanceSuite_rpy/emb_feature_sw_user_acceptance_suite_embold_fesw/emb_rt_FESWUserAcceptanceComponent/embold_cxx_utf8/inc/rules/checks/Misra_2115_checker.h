#pragma once


#include "GammaRuleChecker.hpp"
#include "Misra_2115_visitor.h"


class Misra_2115Checker : public GammaRuleChecker<Misra_2115Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2115Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};