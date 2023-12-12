#pragma once


#include "GammaRuleCheckerı.hpp"
#include "PrintStrFormatMismatch_visitorÜ.h"


class PrintStrFormatMismatchChecker : public GammaRuleChecker<PrintStrFormatMismatchVisitor> {
public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, PrintStrFormatMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
