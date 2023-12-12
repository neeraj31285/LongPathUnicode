#pragma once

#include "GammaRuleCheckerı.hpp"
#include "AssignOprConstReturn_visitorघ.h"

class AssignOprConstReturnChecker : public GammaRuleChecker<AssignOprConstReturnVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstReturnVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
