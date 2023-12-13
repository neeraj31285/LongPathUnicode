#pragma once


#include "GammaRuleChecker.hpp"
#include "ConstOverrideMismatch_visitor.h"


class ConstOverrideMismatchChecker : public GammaRuleChecker<ConstOverrideMismatchVisitor> {

	bool isOverrideDeclMismatch(const clang::CXXRecordDecl* pRecordDecl, const std::string& pMethodName, 
								const bool pIsConst, ConstOverrideMismatchVisitor& pVisitor);

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, ConstOverrideMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
