#pragma once

#include <map>
#include <unordered_map>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class HiddenBaseMemberVisitor : public clang::RecursiveASTVisitor<HiddenBaseMemberVisitor> {

	using CXXFieldMap = std::unordered_multimap<std::string, const clang::FieldDecl*>;
	using CXXMethodMap = std::unordered_multimap<std::string, const clang::CXXMethodDecl*>;

	const clang::ASTContext& m_astContext;
	std::vector<const clang::CXXRecordDecl *> m_cxxRecords;
	
	std::map<const clang::CXXRecordDecl *, CXXFieldMap> m_cxxRecFieldsMap;
	std::map<const clang::CXXRecordDecl *, CXXMethodMap> m_cxxRecMethodsMap;

public:
	
	HiddenBaseMemberVisitor(const clang::ASTContext& pAstContext);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl *pRecorDecl);

	std::vector<const clang::CXXRecordDecl *>& getCxxRecordDecls();

	std::map<const clang::CXXRecordDecl *, CXXFieldMap>& getCxxRecordFieldsMap();

	std::map<const clang::CXXRecordDecl *, CXXMethodMap>& getCxxRecordMethodsMap();

};
