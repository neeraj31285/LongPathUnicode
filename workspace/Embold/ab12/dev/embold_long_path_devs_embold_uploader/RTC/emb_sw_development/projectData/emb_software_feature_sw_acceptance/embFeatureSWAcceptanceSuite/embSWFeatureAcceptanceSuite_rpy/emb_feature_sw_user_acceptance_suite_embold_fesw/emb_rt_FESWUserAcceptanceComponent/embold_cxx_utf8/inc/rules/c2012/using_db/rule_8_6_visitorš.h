#pragma once
#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)

class Rule_8_6_Visitor : public clang::RecursiveASTVisitor<Rule_8_6_Visitor> {

	std::vector<bool> m_isExternFlags;
	std::vector<clang::NamedDecl*> m_externalLinkDefs;
	
	const clang::ASTContext& m_astContext;

public:

	Rule_8_6_Visitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarD);
	bool VisitFunctionDecl(clang::FunctionDecl *pFuncD);

	const std::vector<clang::NamedDecl*>& getExternalDeclDefs();
	const std::vector<bool>& getIsExternFlags();
};
