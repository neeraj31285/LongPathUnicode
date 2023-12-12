#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class NonPODMemOperationVisitor : public clang::RecursiveASTVisitor<NonPODMemOperationVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:
	NonPODMemOperationVisitor(const clang::ASTContext& pAstContext);

	bool isInUserCode(const clang::SourceLocation &loc);
	bool VisitCallExpr(clang::CallExpr *pCallExpr);
	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

};
