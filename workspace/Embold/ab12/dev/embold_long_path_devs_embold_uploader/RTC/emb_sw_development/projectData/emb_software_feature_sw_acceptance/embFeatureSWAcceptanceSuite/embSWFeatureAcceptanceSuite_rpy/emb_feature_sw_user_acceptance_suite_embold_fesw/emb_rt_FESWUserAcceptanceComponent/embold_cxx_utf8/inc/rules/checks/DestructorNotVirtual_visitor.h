#pragma once

#include <vector>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#pragma warning(pop)


class DestructorNotVirtualVisitor : public clang::RecursiveASTVisitor<DestructorNotVirtualVisitor> {

	const clang::ASTContext& m_astContext;

	std::vector<const clang::CXXDestructorDecl*> m_nonCompliantDtor;

public:

	DestructorNotVirtualVisitor(const clang::ASTContext& pAstContext);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);

	const std::vector<const clang::CXXDestructorDecl*>& getNonCompliantDestructors();
};
