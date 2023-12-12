#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class CharSignedDeclarationVisitor : public clang::RecursiveASTVisitor<CharSignedDeclarationVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	
public:

	CharSignedDeclarationVisitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl *pVarDecl);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
