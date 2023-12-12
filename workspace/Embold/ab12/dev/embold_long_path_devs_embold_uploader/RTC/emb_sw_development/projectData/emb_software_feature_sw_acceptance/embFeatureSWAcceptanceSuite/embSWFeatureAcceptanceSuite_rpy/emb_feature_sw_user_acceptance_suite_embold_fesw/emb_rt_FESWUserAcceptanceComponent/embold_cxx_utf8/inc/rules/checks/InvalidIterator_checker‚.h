#pragma once

#include "GammaRuleCheckerı.hpp"
#include "InvalidIterator_visitorങ.h"


class InvalidIteratorChecker : public GammaRuleChecker<InvalidIteratorVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InvalidIteratorVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
