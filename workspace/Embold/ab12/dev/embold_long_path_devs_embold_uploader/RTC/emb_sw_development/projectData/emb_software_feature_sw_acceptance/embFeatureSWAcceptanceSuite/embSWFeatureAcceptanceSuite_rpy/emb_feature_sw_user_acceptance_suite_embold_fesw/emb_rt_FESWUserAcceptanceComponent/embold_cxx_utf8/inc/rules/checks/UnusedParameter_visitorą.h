#pragma once

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class UnusedParameterVisitor : public clang::RecursiveASTVisitor<UnusedParameterVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:

	UnusedParameterVisitor(const clang::ASTContext& pAstContext);

	bool VisitFunctionDecl(clang::FunctionDecl *pFuncDecl);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
