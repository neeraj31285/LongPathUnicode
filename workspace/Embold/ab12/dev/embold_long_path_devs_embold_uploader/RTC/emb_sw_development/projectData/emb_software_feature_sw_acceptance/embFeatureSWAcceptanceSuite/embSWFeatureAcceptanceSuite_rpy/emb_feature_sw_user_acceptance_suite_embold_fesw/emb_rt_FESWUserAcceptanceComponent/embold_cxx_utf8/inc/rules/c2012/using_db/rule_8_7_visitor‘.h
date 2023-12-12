#pragma once
#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class Rule_8_7_Visitor : public clang::RecursiveASTVisitor<Rule_8_7_Visitor> {

	std::unordered_map<std::string, bool> m_formalLinkRefs;
	std::unordered_map<std::string, clang::NamedDecl*> m_externDecls;

	const clang::ASTContext& m_astContext;

public:

	Rule_8_7_Visitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarD);
	bool VisitCallExpr(clang::CallExpr* pCallExpr);
	bool VisitDeclRefExpr(clang::DeclRefExpr * pRfExpr);
	bool VisitFunctionDecl(clang::FunctionDecl *pFuncD);

	const std::unordered_map<std::string, bool>& getFormalLinkageRefMap();
	const std::unordered_map<std::string, clang::NamedDecl*>& getExternDeclsMap();
};
