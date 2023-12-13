#pragma once

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)

class InvalidIteratorDereferencingVisitor : public clang::RecursiveASTVisitor<InvalidIteratorDereferencingVisitor>
{

private:
	clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_violationsSrcLoc;
	std::map<clang::ValueDecl*, clang::ValueDecl*> m_iteratorAssignmentMap;
	std::map<clang::ValueDecl*, clang::SourceRange> m_boundsCheckedIterators;
	clang::SourceRange m_sourceRange;

public:
	
	InvalidIteratorDereferencingVisitor(clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool VisitVarDecl(clang::VarDecl* pVarDecl);

	bool VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr);

	bool VisitFunctionDecl(clang::FunctionDecl* pFuncDecl);

	bool VisitIfStmt(clang::IfStmt* ifStmt);

	bool VisitWhileStmt(clang::WhileStmt* whileStmt);

	bool VisitForStmt(clang::ForStmt* forStmt);
};