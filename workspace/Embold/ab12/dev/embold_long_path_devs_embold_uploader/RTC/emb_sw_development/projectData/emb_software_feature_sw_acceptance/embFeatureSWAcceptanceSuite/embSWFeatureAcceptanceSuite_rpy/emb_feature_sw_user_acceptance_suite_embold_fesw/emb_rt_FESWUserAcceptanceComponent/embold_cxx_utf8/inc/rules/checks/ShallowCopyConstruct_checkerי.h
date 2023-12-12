#pragma once


#include "GammaRuleCheckerı.hpp"
#include "ShallowCopyConstruct_visitor나.h"


class ShallowCopyConstructChecker : public GammaRuleChecker<ShallowCopyConstructVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, ShallowCopyConstructVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
