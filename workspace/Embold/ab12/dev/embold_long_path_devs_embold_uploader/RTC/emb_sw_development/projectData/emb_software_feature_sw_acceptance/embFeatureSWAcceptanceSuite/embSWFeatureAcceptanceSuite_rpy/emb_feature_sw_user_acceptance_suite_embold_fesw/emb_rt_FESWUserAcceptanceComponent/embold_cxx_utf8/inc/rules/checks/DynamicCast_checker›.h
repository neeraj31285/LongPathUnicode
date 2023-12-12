#pragma once


#include "GammaRuleCheckerı.hpp"
#include "DynamicCast_visitorഘ.h"


class DynamicCastChecker : public GammaRuleChecker<DynamicCastVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, DynamicCastVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
