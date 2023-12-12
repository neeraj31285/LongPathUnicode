#pragma once


#include "GammaRuleCheckerı.hpp"
#include "InconsistentNullPtrCheck_visitorń.h"


class InconsistentNullPtrCheckChecker : public GammaRuleChecker<InconsistentNullPtrCheckVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, InconsistentNullPtrCheckVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
