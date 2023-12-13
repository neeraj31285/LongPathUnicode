#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class HiddenParameterVisitor : public clang::RecursiveASTVisitor<HiddenParameterVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	std::vector<std::string> m_currentfuncParams;

public:

	HiddenParameterVisitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarDecl);

	bool VisitFunctionDecl(clang::FunctionDecl *pFuncDecl);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};