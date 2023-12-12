#pragma once


#include "GammaRuleCheckerı.hpp"
#include "Misra_2116_visitorź.h"


class Misra_2116Checker : public GammaRuleChecker<Misra_2116Visitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2116Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
