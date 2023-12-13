#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class ShallowCopyConstructVisitor : public clang::RecursiveASTVisitor<ShallowCopyConstructVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:

	ShallowCopyConstructVisitor(const clang::ASTContext& pAstContext);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);
};