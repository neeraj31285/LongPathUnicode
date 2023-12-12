#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class DynamicCastVisitor : public clang::RecursiveASTVisitor<DynamicCastVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	
public:

	DynamicCastVisitor(const clang::ASTContext& pAstContext);

	bool VisitCXXDynamicCastExpr(clang::CXXDynamicCastExpr *pDynCastExpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
