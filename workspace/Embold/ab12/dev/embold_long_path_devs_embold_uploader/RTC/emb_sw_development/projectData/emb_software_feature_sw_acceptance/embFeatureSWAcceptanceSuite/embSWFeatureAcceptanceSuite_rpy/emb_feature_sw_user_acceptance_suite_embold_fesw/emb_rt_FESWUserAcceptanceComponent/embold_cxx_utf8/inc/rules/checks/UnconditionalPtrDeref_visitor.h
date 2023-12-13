#pragma once

#include <set>
#include <unordered_map>
#include "clang/AST/RecursiveASTVisitor.h"


class UnconditionalPtrDerefVisitor : public clang::RecursiveASTVisitor<UnconditionalPtrDerefVisitor> {

	const clang::ASTContext& m_astContext;

	std::string m_currVisitingFuncName;
	std::set<std::string> m_nullCheckedReturns;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	std::vector<std::pair<std::string, std::string>> m_funcCallsWithReturnPtr;
	std::unordered_map<std::string, clang::ValueDecl*> m_ptrAssignedFrmCallee;
	std::unordered_map<std::string, std::unordered_map<std::string, clang::SourceLocation>> m_unconditionalDrefFuncArgs;

	clang::Expr* getNullPtrCheckdDeclRefExprFrom(const clang::IfStmt* pIfStmt);

	bool isPointerNullCheckedInIfStmt(const clang::IfStmt* pStmt, clang::ValueDecl* pVarDecl);

	void updateUnconditionalDrefFuncArgsMap(const clang::DynTypedNodeList& pRootParent, clang::ValueDecl* pValDecl, 
											const clang::SourceLocation& pDrefOpLoc, bool pIsIfCondFound);

	clang::DynTypedNodeList getRootParentCheckIfStmt(clang::DynTypedNodeList pCurParent, bool& pIsIfStmtFound);

public:

	UnconditionalPtrDerefVisitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarDecl);

	bool VisitCallExpr(clang::CallExpr *pCallExpr);

	bool VisitFunctionDecl(clang::FunctionDecl *D);

	bool VisitReturnStmt(clang::ReturnStmt* pRetStmt);

	bool VisitUnaryOperator(clang::UnaryOperator* pUnOpr);

	bool VisitBinaryOperator(clang::BinaryOperator *pBinOpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	const std::vector<std::pair<std::string, std::string>>& getFuncCallsWithReturnedPtr();

	const std::unordered_map<std::string, std::unordered_map<std::string, clang::SourceLocation>>& getUnconditionalDerefFuncArgs();
};
