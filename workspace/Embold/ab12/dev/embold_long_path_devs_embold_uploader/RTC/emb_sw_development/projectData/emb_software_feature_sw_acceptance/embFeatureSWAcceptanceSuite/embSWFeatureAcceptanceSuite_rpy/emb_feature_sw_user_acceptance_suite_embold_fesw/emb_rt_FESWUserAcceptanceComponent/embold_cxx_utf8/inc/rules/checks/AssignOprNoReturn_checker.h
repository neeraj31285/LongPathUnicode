#pragma once

#include "GammaRuleChecker.hpp"
#include "AssignOprNoReturn_visitor.h"

class AssignOprNoReturnChecker : public GammaRuleChecker<AssignOprNoReturnVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprNoReturnVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
