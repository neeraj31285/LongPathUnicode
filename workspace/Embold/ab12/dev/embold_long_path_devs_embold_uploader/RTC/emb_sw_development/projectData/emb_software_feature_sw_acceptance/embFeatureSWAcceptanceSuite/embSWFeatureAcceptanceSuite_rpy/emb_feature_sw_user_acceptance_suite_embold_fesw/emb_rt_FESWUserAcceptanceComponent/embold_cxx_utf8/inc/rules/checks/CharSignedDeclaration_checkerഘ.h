#pragma once


#include "GammaRuleCheckerı.hpp"
#include "CharSignedDeclaration_visitorח.h"


class CharSignedDeclarationChecker : public GammaRuleChecker<CharSignedDeclarationVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, CharSignedDeclarationVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
