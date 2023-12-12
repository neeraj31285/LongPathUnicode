#pragma once

#include <vector>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#pragma warning(pop)


class AssignOprConstReturnVisitor : public clang::RecursiveASTVisitor<AssignOprConstReturnVisitor> {

	const clang::ASTContext& m_astContext;

	std::vector<const clang::FunctionDecl*> m_assignOprDecl;

public:

	AssignOprConstReturnVisitor(const clang::ASTContext& pAstContext);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);

	const std::vector<const clang::FunctionDecl*>& getAssignOperatorsDecl();
};
