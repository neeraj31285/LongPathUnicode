#pragma once


#include "GammaRuleCheckerı.hpp"
#include "BitwiseOprandsMismatch_visitorА.h"


class BitOprandsMismatchChecker : public GammaRuleChecker<BitOprandsMismatchVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, BitOprandsMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
