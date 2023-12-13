#pragma once

#include "GammaRuleChecker.hpp"
#include "AssignOprConstArg_visitor.h"

class AssignOprConstArgChecker : public GammaRuleChecker<AssignOprConstArgVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstArgVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
