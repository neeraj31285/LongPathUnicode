#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class InfiniteLoopVisitor : public clang::RecursiveASTVisitor<InfiniteLoopVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:
	InfiniteLoopVisitor(const clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool isInUserCode(const clang::SourceLocation &loc);
	bool getChildren(const clang::Stmt *S);
	bool TraverseForStmt(clang::ForStmt *pForStmt);
	bool VisitStmt(clang::Stmt *Stmt);

};