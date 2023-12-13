#pragma once


#include "GammaRuleChecker.hpp"
#include "NonPODMemOperation_visitor.h"


class NonPODMemOperationChecker : public GammaRuleChecker<NonPODMemOperationVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, NonPODMemOperationVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};