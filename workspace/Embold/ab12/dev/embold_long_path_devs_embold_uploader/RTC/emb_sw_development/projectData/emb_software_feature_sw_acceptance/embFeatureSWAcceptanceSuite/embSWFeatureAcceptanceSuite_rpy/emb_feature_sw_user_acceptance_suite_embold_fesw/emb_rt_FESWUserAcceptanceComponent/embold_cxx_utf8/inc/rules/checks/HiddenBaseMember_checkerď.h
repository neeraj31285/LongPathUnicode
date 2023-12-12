#pragma once


#include "GammaRuleCheckerı.hpp"
#include "HiddenBaseMember_visitorア.h"


class HiddenBaseMemberChecker : public GammaRuleChecker<HiddenBaseMemberVisitor> {

	using CXXFieldMap = std::unordered_multimap<std::string, const clang::FieldDecl*>;
	using CXXMethodMap = std::unordered_multimap<std::string, const clang::CXXMethodDecl*>;

	std::vector<clang::SourceLocation> m_voilationsSrcLoc;

	void performChecksForHiddenMembers(std::vector<const clang::CXXRecordDecl *>& pRecordDecls,
									   std::map<const clang::CXXRecordDecl *, CXXFieldMap>& pCxxRecordFieldsMap,
									   std::map<const clang::CXXRecordDecl *, CXXMethodMap>& pCxxRecordMethodMaps);

	void checkForHiddenBaseFields(CXXFieldMap& pDerivedFields, const CXXFieldMap& pBaseCxxRec);

	void checkForHiddenBaseMethods(const clang::CXXRecordDecl* pDerivedCxxRecord, const clang::CXXRecordDecl* pBaseCxxRecord,
								   CXXMethodMap& pDerivedMethods, const CXXMethodMap& pBaseMethods);

public:

	static const bool initialize();

	void onEndOfTranslationUnit(clang::ASTContext& pContext, HiddenBaseMemberVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans) override;
};
