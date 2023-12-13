#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class MainReturnsVoidVisitor : public clang::RecursiveASTVisitor<MainReturnsVoidVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	
public:

	MainReturnsVoidVisitor(const clang::ASTContext& pAstContext);

	bool VisitFunctionDecl(clang::FunctionDecl *pFuncDecl);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};