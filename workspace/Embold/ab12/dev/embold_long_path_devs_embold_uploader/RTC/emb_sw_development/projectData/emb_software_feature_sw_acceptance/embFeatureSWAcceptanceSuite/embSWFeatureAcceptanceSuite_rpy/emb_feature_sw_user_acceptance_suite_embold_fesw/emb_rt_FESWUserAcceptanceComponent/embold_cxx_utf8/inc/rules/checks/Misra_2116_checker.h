#pragma once


#include "GammaRuleChecker.hpp"
#include "Misra_2116_visitor.h"


class Misra_2116Checker : public GammaRuleChecker<Misra_2116Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2116Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};