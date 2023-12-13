#pragma once
#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)

class Rule_5_9_Visitor : public clang::RecursiveASTVisitor<Rule_5_9_Visitor> {

	const clang::ASTContext& m_astContext;

	std::unordered_map<std::string, clang::NamedDecl*> m_nonInternalLinkedDecls;
	std::unordered_map<std::string, clang::NamedDecl*> m_internalLinkedDecls;

public:

	Rule_5_9_Visitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarD);
	bool VisitFunctionDecl(clang::FunctionDecl *pFuncD);

	const std::unordered_map<std::string, clang::NamedDecl*>& getNonInternalLinkedDeclsMap();
	const std::unordered_map<std::string, clang::NamedDecl*>& getInternalLinkedDeclsMap();
};