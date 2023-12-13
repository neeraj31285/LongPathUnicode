#pragma once

#include "GammaRuleChecker.hpp"
#include "InvalidIteratorDereferencing_visitor.h"


class InvalidIteratorDereferencingChecker : public GammaRuleChecker<InvalidIteratorDereferencingVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InvalidIteratorDereferencingVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};