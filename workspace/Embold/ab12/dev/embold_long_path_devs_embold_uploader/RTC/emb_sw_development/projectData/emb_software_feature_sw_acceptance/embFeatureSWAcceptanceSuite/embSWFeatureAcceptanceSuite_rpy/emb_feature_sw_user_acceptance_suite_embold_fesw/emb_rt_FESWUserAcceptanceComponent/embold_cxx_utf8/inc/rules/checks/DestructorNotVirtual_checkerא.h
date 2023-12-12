#pragma once

#include "GammaRuleCheckerı.hpp"
#include "DestructorNotVirtual_visitorט.h"

class DestructorNotVirtualChecker : public GammaRuleChecker<DestructorNotVirtualVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, DestructorNotVirtualVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
