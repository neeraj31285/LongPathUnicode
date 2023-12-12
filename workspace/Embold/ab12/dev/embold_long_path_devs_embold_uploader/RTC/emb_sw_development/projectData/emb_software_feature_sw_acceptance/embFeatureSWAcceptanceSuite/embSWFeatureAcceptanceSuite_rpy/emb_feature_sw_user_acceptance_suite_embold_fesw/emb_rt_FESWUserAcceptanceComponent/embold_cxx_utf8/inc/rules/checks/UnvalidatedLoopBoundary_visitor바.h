#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class UnvalidatedLoopBoundaryVisitor : public clang::RecursiveASTVisitor<UnvalidatedLoopBoundaryVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:
	UnvalidatedLoopBoundaryVisitor(const clang::ASTContext& pAstContext);

	bool isInUserCode(const clang::SourceLocation &loc);
	bool VisitCallExpr(clang::CallExpr * pCallExpr);
	bool VisitStmt(clang::Stmt *pStmt);
	bool hasParentIfStmt(const clang::Stmt * S);
	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

};
