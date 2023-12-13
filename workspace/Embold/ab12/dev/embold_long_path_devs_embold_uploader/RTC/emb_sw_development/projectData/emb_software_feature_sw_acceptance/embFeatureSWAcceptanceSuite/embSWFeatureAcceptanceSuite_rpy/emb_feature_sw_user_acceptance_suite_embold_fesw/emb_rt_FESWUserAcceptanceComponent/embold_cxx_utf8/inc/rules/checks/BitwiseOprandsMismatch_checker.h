#pragma once


#include "GammaRuleChecker.hpp"
#include "BitwiseOprandsMismatch_visitor.h"


class BitOprandsMismatchChecker : public GammaRuleChecker<BitOprandsMismatchVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, BitOprandsMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
