#pragma once

#include "GammaRuleChecker.hpp"
#include "HardCodedCredentials_visitor.h"


class HardCodedCredentialsChecker : public GammaRuleChecker<HardCodedCredentialsVisitor> {

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, HardCodedCredentialsVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};