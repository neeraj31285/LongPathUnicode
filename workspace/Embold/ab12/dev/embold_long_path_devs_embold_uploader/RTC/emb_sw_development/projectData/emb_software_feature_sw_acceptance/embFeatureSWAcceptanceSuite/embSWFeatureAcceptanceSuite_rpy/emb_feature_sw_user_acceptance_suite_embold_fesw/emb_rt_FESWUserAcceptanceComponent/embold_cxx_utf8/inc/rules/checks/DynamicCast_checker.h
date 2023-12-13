#pragma once


#include "GammaRuleChecker.hpp"
#include "DynamicCast_visitor.h"


class DynamicCastChecker : public GammaRuleChecker<DynamicCastVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, DynamicCastVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
