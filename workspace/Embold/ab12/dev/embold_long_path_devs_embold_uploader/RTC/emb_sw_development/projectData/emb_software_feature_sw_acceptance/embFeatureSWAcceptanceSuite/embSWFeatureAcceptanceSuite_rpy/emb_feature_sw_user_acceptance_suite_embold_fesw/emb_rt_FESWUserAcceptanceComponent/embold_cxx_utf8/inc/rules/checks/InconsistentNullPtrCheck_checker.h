#pragma once


#include "GammaRuleChecker.hpp"
#include "InconsistentNullPtrCheck_visitor.h"


class InconsistentNullPtrCheckChecker : public GammaRuleChecker<InconsistentNullPtrCheckVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InconsistentNullPtrCheckVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
