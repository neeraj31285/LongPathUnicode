#pragma once

#include <vector>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#pragma warning(pop)


class AssignOprConstArgVisitor : public clang::RecursiveASTVisitor<AssignOprConstArgVisitor> {

	const clang::ASTContext& m_astContext;

	std::vector<const clang::FunctionDecl*> m_assignOprDecl;

public:

	AssignOprConstArgVisitor(clang::ASTContext& pAstContext);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);

	const std::vector<const clang::FunctionDecl*>& getAssignOperatorsDecl();
};
