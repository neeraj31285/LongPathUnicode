#pragma once

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class PrintStrFormatMismatchVisitor : public clang::RecursiveASTVisitor<PrintStrFormatMismatchVisitor> {

	const clang::ASTContext& m_astContext;
	clang::FunctionDecl* m_parentFuncDecl;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	enum BuiltInID
	{
		PRINTF = 707, FPRINTF = 708, SNPRINTF = 709, SPRINTF = 710
	};

public:

	PrintStrFormatMismatchVisitor(const clang::ASTContext& pAstContext);

	bool VisitCallExpr(clang::CallExpr *pCallExpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};