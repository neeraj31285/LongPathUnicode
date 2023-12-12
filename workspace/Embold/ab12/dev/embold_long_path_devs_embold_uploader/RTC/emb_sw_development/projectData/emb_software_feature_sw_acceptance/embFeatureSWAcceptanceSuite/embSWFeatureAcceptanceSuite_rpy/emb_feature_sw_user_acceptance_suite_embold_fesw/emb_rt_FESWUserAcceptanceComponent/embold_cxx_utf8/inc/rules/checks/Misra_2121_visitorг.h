#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class Misra_2121Visitor : public clang::RecursiveASTVisitor<Misra_2121Visitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:
	Misra_2121Visitor(const clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool VisitCallExpr(clang::CallExpr *pCallExpr);
	bool VisitVarDecl(clang::VarDecl *pVarDecl);

};
