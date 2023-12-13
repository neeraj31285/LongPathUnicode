#pragma once

#include "GammaRuleChecker.hpp"
#include "AssignOprConstReturn_visitor.h"

class AssignOprConstReturnChecker : public GammaRuleChecker<AssignOprConstReturnVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstReturnVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
