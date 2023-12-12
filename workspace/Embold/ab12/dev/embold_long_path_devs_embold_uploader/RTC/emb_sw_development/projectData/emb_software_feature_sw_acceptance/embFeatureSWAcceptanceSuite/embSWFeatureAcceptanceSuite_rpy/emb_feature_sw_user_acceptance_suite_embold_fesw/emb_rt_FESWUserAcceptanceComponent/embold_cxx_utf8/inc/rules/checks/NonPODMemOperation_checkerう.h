#pragma once


#include "GammaRuleCheckerı.hpp"
#include "NonPODMemOperation_visitorघ.h"


class NonPODMemOperationChecker : public GammaRuleChecker<NonPODMemOperationVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, NonPODMemOperationVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
