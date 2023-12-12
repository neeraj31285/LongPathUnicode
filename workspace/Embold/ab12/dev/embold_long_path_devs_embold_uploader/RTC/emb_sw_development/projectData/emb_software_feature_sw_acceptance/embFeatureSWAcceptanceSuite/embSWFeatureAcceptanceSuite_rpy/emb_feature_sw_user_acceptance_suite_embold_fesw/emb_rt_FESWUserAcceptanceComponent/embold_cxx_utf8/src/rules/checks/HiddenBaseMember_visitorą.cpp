
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "CheckerUtilsг.h"
#include "HiddenBaseMember_visitorア.h"
#include <clang/AST/Comment.h>
#include <clang/AST/Attr.h>
#include <clang/AST/DeclCXX.h>

using namespace std;
using namespace clang;


HiddenBaseMemberVisitor::HiddenBaseMemberVisitor(const ASTContext& pAstContext)
    : m_astContext(pAstContext) {
}

vector<const CXXRecordDecl*>& HiddenBaseMemberVisitor::getCxxRecordDecls() {
	return m_cxxRecords;
}

map<const CXXRecordDecl*, HiddenBaseMemberVisitor::CXXFieldMap>& HiddenBaseMemberVisitor::getCxxRecordFieldsMap(){
	return m_cxxRecFieldsMap;
}

map<const CXXRecordDecl*, HiddenBaseMemberVisitor::CXXMethodMap>& HiddenBaseMemberVisitor::getCxxRecordMethodsMap(){
	return m_cxxRecMethodsMap;
}


bool HiddenBaseMemberVisitor::VisitCXXRecordDecl(CXXRecordDecl *pRecordDecl) {

	if (!CheckerUtils::isValidCxxRecord(pRecordDecl) || pRecordDecl->isPolymorphic()) {
		return true;
	}

	CXXFieldMap cxxFieldMetaMap;
	for (const FieldDecl *cxxField : pRecordDecl->fields()) {

		if (cxxField && !cxxField->isInvalidDecl()) {
			const string &fieldId = cxxField->getDeclName().getAsString();
			cxxFieldMetaMap.emplace(make_pair(fieldId, cxxField));
		}
	}

	set<const CXXMethodDecl*> cxxCtors;
	for (auto ctorItr : pRecordDecl->ctors()) {
		cxxCtors.insert(ctorItr);
	}

	CXXMethodMap cxxMethodMetaMap;
	for (const CXXMethodDecl *cxxMethod : pRecordDecl->methods()) {

		if (cxxMethod && !cxxMethod->isInvalidDecl() && cxxMethod->isUserProvided() &&
			!cxxMethod->isVirtual() && cxxCtors.find(cxxMethod) == cxxCtors.end()) {

			const string &methodId = cxxMethod->getDeclName().getAsString();
			cxxMethodMetaMap.emplace(make_pair(methodId, cxxMethod));
		}
	}

	m_cxxRecords.push_back(pRecordDecl);
	m_cxxRecFieldsMap.emplace(make_pair(pRecordDecl, cxxFieldMetaMap));
	m_cxxRecMethodsMap.emplace(make_pair(pRecordDecl, cxxMethodMetaMap));
	return true;
}
