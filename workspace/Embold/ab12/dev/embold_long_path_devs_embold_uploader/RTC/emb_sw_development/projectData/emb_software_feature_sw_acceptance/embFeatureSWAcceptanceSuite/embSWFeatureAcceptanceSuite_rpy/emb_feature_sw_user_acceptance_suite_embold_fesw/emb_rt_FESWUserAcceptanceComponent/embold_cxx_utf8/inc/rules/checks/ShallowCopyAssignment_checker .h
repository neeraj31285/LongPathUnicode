#pragma once

#include "GammaRuleCheckerı.hpp"
#include "ShallowCopyAssignment_visitorß.h"


class ShallowCopyAssignmentChecker : public GammaRuleChecker<ShallowCopyAssignmentVisitor> {

public:
	static const bool initialize();
	void onEndOfTranslationUnit(clang::ASTContext& pContext, ShallowCopyAssignmentVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
