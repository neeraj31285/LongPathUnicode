#pragma once

#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class ConstOverrideMismatchVisitor : public clang::RecursiveASTVisitor<ConstOverrideMismatchVisitor> {

	using CXXMethodMap = std::unordered_multimap<std::string, const clang::CXXMethodDecl*>;

	const clang::ASTContext& m_astContext;

	std::vector<const clang::CXXRecordDecl *> m_cxxRecords;
	std::map<const clang::CXXRecordDecl *, CXXMethodMap> m_cxxRecMethodsMap;

public:

	ConstOverrideMismatchVisitor(const clang::ASTContext& pAstContext);

	void sortCxxRecordDecls();

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pCxxRecord);

	const std::map<const clang::CXXRecordDecl *, CXXMethodMap>& getCxxRecordToMethodMap();
};
