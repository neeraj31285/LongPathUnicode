#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class InconsistentNullPtrCheckVisitor : public clang::RecursiveASTVisitor<InconsistentNullPtrCheckVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	std::vector<clang::ValueDecl*> m_drefsWithoutNullCheck;

public:

	InconsistentNullPtrCheckVisitor(const clang::ASTContext& pAstContext);

	bool isPointerNullChecked(const clang::IfStmt* pStmt, clang::ValueDecl* pVarDecl);

	bool VisitFunctionDecl(clang::FunctionDecl *D);

	clang::DynTypedNodeList getRootParentCheckIfStmt(clang::DynTypedNodeList pCurParent, bool& pIsIfStmtFound);

	bool VisitUnaryOperator(clang::UnaryOperator* pUnOpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
