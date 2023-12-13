#pragma once


#include "GammaRuleChecker.hpp"
#include "ShallowCopyConstruct_visitor.h"


class ShallowCopyConstructChecker : public GammaRuleChecker<ShallowCopyConstructVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, ShallowCopyConstructVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
