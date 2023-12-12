#pragma once

#include <unordered_map>
#include <fstream>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class BitOprandsMismatchVisitor : public clang::RecursiveASTVisitor<BitOprandsMismatchVisitor> {

	const clang::ASTContext& m_astContext;

	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

public:

	BitOprandsMismatchVisitor(const clang::ASTContext& pAstContext);

	bool VisitBinaryOperator(clang::BinaryOperator *pBinOpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
