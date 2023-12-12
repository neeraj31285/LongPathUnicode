#pragma once

#include <vector>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#pragma warning(pop)


class AssignOprNoReturnVisitor : public clang::RecursiveASTVisitor<AssignOprNoReturnVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<const clang::FunctionDecl*> m_assignOprDecl;

public:

	AssignOprNoReturnVisitor(const clang::ASTContext& pAstContext);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);

	const std::vector<const clang::FunctionDecl*>& getAssignOperatorsDecl();
};
