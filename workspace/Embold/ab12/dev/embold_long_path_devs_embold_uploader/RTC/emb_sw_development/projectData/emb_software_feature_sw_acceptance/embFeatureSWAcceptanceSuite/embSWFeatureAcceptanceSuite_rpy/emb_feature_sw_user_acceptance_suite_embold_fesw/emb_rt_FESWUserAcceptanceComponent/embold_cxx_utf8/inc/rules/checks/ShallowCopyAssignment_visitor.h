#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class ShallowCopyAssignmentVisitor : public clang::RecursiveASTVisitor<ShallowCopyAssignmentVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:

	ShallowCopyAssignmentVisitor(const clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);
};