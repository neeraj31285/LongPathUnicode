#pragma once

#include <regex>
#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class HardCodedCredentialsVisitor : public clang::RecursiveASTVisitor<HardCodedCredentialsVisitor> {
	clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	//static std::regex username_pattern;
	//static std::regex password_pattern;

public:

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	HardCodedCredentialsVisitor(clang::ASTContext& pAstContext);

	bool VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr);
	bool VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* pCXXMemberCallExpr);
	bool VisitCallExpr(clang::CallExpr* pCallExpr);
	bool VisitVarDecl(clang::VarDecl* pVarDecl);
	clang::StringLiteral* GetInitStringLiteral(clang::Expr* initExpr);
};
