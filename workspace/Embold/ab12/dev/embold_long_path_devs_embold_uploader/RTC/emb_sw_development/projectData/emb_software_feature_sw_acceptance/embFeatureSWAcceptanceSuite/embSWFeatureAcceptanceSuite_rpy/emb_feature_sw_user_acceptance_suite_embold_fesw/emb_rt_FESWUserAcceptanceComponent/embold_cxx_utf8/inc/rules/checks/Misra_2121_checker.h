#pragma once


#include "GammaRuleChecker.hpp"
#include "Misra_2121_visitor.h"


class Misra_2121Checker : public GammaRuleChecker<Misra_2121Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2121Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
