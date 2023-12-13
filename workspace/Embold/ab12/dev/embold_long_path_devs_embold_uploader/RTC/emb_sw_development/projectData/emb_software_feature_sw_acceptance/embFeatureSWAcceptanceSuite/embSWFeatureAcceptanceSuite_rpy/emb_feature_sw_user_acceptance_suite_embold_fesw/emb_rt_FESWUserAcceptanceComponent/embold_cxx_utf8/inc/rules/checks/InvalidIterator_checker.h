#pragma once

#include "GammaRuleChecker.hpp"
#include "InvalidIterator_visitor.h"


class InvalidIteratorChecker : public GammaRuleChecker<InvalidIteratorVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InvalidIteratorVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};