#pragma once

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)

class InvalidIteratorVisitor : public clang::RecursiveASTVisitor<InvalidIteratorVisitor>
{

private:
	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	std::map<clang::ValueDecl*, clang::ValueDecl*> m_iteratorAssignmentMap;
	std::set<clang::ValueDecl*> m_boundsCheckedIterators;

public:
	
	InvalidIteratorVisitor(const clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* pCXXMemberCallExpr);

	bool VisitVarDecl(clang::VarDecl* pVarDecl);

	bool VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr);

	bool VisitFunctionDecl(clang::FunctionDecl* pFuncDecl);
};
