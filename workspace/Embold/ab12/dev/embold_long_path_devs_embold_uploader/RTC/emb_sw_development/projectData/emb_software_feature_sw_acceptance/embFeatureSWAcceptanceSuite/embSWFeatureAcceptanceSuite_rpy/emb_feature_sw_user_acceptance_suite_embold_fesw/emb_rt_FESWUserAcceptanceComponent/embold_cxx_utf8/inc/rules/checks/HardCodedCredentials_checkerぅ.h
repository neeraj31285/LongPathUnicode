#pragma once

#include "GammaRuleCheckerı.hpp"
#include "HardCodedCredentials_visitorż.h"


class HardCodedCredentialsChecker : public GammaRuleChecker<HardCodedCredentialsVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, HardCodedCredentialsVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
