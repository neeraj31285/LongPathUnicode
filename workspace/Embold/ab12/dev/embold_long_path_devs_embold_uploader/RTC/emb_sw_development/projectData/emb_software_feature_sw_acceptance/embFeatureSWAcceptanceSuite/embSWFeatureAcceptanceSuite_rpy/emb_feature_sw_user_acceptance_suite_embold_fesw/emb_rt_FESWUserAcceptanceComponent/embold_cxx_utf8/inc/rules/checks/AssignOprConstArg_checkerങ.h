#pragma once

#include "GammaRuleCheckerı.hpp"
#include "AssignOprConstArg_visitorą.h"

class AssignOprConstArgChecker : public GammaRuleChecker<AssignOprConstArgVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstArgVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
