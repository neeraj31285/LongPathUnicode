#pragma once
#include <unordered_set>
#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class UnvalidatedParamAsIndexVisitor : public clang::RecursiveASTVisitor<UnvalidatedParamAsIndexVisitor> {

	clang::FunctionDecl* m_parentFunction;
	std::unordered_map<clang::FunctionDecl*, unsigned int> m_unvalidatedParameters;
	std::unordered_set<clang::ValueDecl*> m_boundsCheckedParameters;
	std::unordered_set<clang::ValueDecl*> m_boundsCheckedVariables;

	clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	UnvalidatedParamAsIndexVisitor(clang::ASTContext& pAstContext);

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);
	bool VisitArraySubscriptExpr(clang::ArraySubscriptExpr* arraySubscriptExpr);
	bool VisitBinaryOperator(clang::BinaryOperator* binaryOperator);
	bool VisitCallExpr(clang::CallExpr* callExpr);
};
