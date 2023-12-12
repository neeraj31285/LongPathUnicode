#pragma once


#include "GammaRuleCheckerı.hpp"
#include "Misra_2121_visitorг.h"


class Misra_2121Checker : public GammaRuleChecker<Misra_2121Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2121Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
