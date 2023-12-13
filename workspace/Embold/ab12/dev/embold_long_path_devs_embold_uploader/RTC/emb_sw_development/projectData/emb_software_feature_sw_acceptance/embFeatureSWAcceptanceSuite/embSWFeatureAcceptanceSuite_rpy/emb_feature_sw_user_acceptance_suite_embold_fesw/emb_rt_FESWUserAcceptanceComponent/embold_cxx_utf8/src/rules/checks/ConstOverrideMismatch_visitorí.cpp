#include "CheckerUtils.h"
#include "ConstOverrideMismatch_visitor.h"

using namespace std;
using namespace clang;

const map<const CXXRecordDecl *, ConstOverrideMismatchVisitor::CXXMethodMap>& 
ConstOverrideMismatchVisitor::getCxxRecordToMethodMap()
{
	return m_cxxRecMethodsMap;
}

ConstOverrideMismatchVisitor::ConstOverrideMismatchVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

void ConstOverrideMismatchVisitor::sortCxxRecordDecls()
{
	const auto& comparator = [](const CXXRecordDecl *pCxxa, const CXXRecordDecl *pCxxb)->const bool {
		
		set<const CXXRecordDecl*> visitedSetA, visitedSetB;
		const int baseCountA = CheckerUtils::getBaseCount(pCxxa, visitedSetA);
		const int baseCountB = CheckerUtils::getBaseCount(pCxxb, visitedSetB);
		return baseCountA < baseCountB;
	};

	sort(m_cxxRecords.begin(), m_cxxRecords.end(), comparator);
}

bool ConstOverrideMismatchVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!CheckerUtils::isValidCxxRecord(pCxxRecord) || !pCxxRecord->isPolymorphic()) {
		return true;
	}

	set<const CXXMethodDecl*> cxxCtors;
	for (auto ctorItr : pCxxRecord->ctors()) {
		cxxCtors.insert(ctorItr);
	}

	CXXMethodMap cxxMethodMetaMap;
	for (const CXXMethodDecl *cxxMethod : pCxxRecord->methods()) {
		
		if (cxxMethod && !cxxMethod->isInvalidDecl() && cxxMethod->isUserProvided() && 
			cxxMethod->getAccess() != AccessSpecifier::AS_private && cxxCtors.find(cxxMethod) == cxxCtors.end()) {

			const string &methodName = cxxMethod->getDeclName().getAsString();
			cxxMethodMetaMap.emplace(make_pair(methodName, cxxMethod));
		}
	}

	m_cxxRecords.push_back(pCxxRecord);
	m_cxxRecMethodsMap.emplace(make_pair(pCxxRecord, cxxMethodMetaMap));
	return true;
}
