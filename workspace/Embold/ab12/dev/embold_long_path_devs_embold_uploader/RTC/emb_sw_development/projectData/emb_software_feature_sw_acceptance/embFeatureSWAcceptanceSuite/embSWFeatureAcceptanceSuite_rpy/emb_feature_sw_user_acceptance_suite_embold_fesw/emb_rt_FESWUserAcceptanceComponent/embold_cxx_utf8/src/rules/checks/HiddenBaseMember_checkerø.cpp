
#include "HiddenBaseMember_checker.h"

#pragma warning(push,0)

#include "CheckerUtils.h"

#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "low";
static const char* const RULE_KEY = "hidden base member";
static const char* const ISSUE_DESC = "Superclass and subclass have a member with the same name, so that the subclass \"hides\" the superclass's member. At the very least, this is confusing and therefore bad style. The maintainability is reduced.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, HiddenBaseMemberChecker)


void HiddenBaseMemberChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, HiddenBaseMemberVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	performChecksForHiddenMembers(pVisitor.getCxxRecordDecls(), pVisitor.getCxxRecordFieldsMap(), pVisitor.getCxxRecordMethodsMap());

	const auto& SM = pContext.getSourceManager();
	for (const SourceLocation& srcLoc : m_voilationsSrcLoc) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		if (!relPath.empty()) {
			
		}
	}
}


void HiddenBaseMemberChecker::checkForHiddenBaseFields(CXXFieldMap& pDerivedFields, const CXXFieldMap& pBaseFields) {

	vector<string> inheritedFieldId;
	vector<const FieldDecl *> inheritedFieldMeta;
	for (const auto &baseFieldItr : pBaseFields) {
		const auto &baseFieldId = baseFieldItr.first;
		const FieldDecl *baseField = baseFieldItr.second;
		if (baseField->getAccess() != AccessSpecifier::AS_private) {

			const auto &fieldRange = pDerivedFields.equal_range(baseFieldId);
			if (fieldRange.first == fieldRange.second) {
				inheritedFieldId.push_back(baseFieldId);
				inheritedFieldMeta.push_back(baseField);
			}
			else {
				for (auto itr = fieldRange.first; itr != fieldRange.second; ++itr) {
					const FieldDecl *drvdField = itr->second;
					m_voilationsSrcLoc.push_back(drvdField->getSourceRange().getBegin());
				}
			}
		}
	}
	for (int index = 0; index < inheritedFieldId.size(); index++) {
		pDerivedFields.emplace(make_pair(inheritedFieldId[index], inheritedFieldMeta[index]));
	}
}


void HiddenBaseMemberChecker::checkForHiddenBaseMethods(const CXXRecordDecl* pDerivedCxxRecord, const CXXRecordDecl* pBaseCxxRecord,
														CXXMethodMap& pDerivedMethods, const CXXMethodMap& pBaseMethods) {
	vector<string> inheritedMethodId;
	vector<const CXXMethodDecl*> inheritedMethodMeta;
	for (const auto &baseMethodItr : pBaseMethods) {

		const auto &baseMethodId = baseMethodItr.first;
		const CXXMethodDecl* baseCxxMethod = baseMethodItr.second;
		if (baseCxxMethod->getAccess() != AccessSpecifier::AS_private) {

			const auto &methodRange = pDerivedMethods.equal_range(baseMethodId);
			if (methodRange.first == methodRange.second) {
				inheritedMethodId.emplace_back(baseMethodId);
				inheritedMethodMeta.emplace_back(baseCxxMethod);
			}
			else {
				for (auto itr = methodRange.first; itr != methodRange.second; ++itr) {

					const CXXMethodDecl* drvdCxxMethod = itr->second;
					const CXXRecordDecl* ownerCxxBase = baseCxxMethod->getParent();
					const CXXRecordDecl* ownerCxxDerived = drvdCxxMethod->getParent();
					if (!pDerivedCxxRecord->isVirtuallyDerivedFrom(ownerCxxBase)) {
						m_voilationsSrcLoc.push_back(drvdCxxMethod->getSourceRange().getBegin());
					}
				}
			}
		}
	}
	for (size_t index = 0; index < inheritedMethodId.size(); index++) {
		pDerivedMethods.emplace(make_pair(inheritedMethodId[index], inheritedMethodMeta[index]));
	}
}


void HiddenBaseMemberChecker::performChecksForHiddenMembers(vector<const CXXRecordDecl *>& pRecordDecls,
															std::map<const clang::CXXRecordDecl *, CXXFieldMap>& pCxxRecordFieldsMap,
															std::map<const clang::CXXRecordDecl *, CXXMethodMap>& pCxxRecordMethodMaps)
{
	sort(pRecordDecls.begin(), pRecordDecls.end(),
		[](const CXXRecordDecl *pCxxa, const CXXRecordDecl *pCxxb)->const bool {
		set<const CXXRecordDecl*> visitedSetA, visitedSetB;
		const int baseCountA = CheckerUtils::getBaseCount(pCxxa, visitedSetA);
		const int baseCountB = CheckerUtils::getBaseCount(pCxxb, visitedSetB);
		return baseCountA < baseCountB;
	});


	for (const CXXRecordDecl *cxxDerivedRec : pRecordDecls) {

		const auto& fieldMapItr = pCxxRecordFieldsMap.find(cxxDerivedRec);
		const auto& methodMapItr = pCxxRecordMethodMaps.find(cxxDerivedRec);
		set<const CXXRecordDecl*> visitedSet;

		if (cxxDerivedRec->getNumBases() == 0) {
			continue;
		}

		for (auto baseItr = cxxDerivedRec->bases_begin(); baseItr != cxxDerivedRec->bases_end(); ++baseItr) {

			const CXXRecordDecl *cxxBaseRec = baseItr->getType()->getAsCXXRecordDecl();

			if (!CheckerUtils::isValidCxxRecord(cxxBaseRec)) {
				continue;
			}

			if (fieldMapItr != pCxxRecordFieldsMap.end()) {
				const auto& baseFieldMapItr = pCxxRecordFieldsMap.find(cxxBaseRec);
				if (baseFieldMapItr != pCxxRecordFieldsMap.end()) {
					checkForHiddenBaseFields(fieldMapItr->second, pCxxRecordFieldsMap.find(cxxBaseRec)->second);
				}
			}

			if (methodMapItr != pCxxRecordMethodMaps.end()) {
				const auto& baseMethodMapItr = pCxxRecordMethodMaps.find(cxxBaseRec);
				if (baseMethodMapItr != pCxxRecordMethodMaps.end()) {
					checkForHiddenBaseMethods(cxxDerivedRec, cxxBaseRec, methodMapItr->second, pCxxRecordMethodMaps.find(cxxBaseRec)->second);
				}
			}
		}
	}
}

