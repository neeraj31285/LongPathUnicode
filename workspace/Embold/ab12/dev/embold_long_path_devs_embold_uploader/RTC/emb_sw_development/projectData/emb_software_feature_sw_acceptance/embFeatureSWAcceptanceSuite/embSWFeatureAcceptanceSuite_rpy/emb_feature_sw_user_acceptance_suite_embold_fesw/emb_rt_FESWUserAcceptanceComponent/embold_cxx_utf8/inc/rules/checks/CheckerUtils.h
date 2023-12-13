#pragma once

#include <set>

#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/ParentMapContext.h"

class CheckerUtils
{
public:
	CheckerUtils() = delete;
	~CheckerUtils() = delete;

	inline static const bool isValidCxxRecord(const clang::CXXRecordDecl *pRecordDecl) {

		return (pRecordDecl && !pRecordDecl->isInvalidDecl() && (pRecordDecl->isStruct() || pRecordDecl->isClass()) &&
				pRecordDecl->hasDefinition() && !pRecordDecl->isImplicit() && isInUserCode(pRecordDecl));
	}

	inline static const bool isInUserCode(const clang::Decl *pNameDecl) {

		if (pNameDecl->getSourceRange().isValid()) {
			const clang::SourceManager &SM = pNameDecl->getASTContext().getSourceManager();
			return !(SM.isInSystemHeader(pNameDecl->getLocation()) || SM.isInSystemMacro(pNameDecl->getLocation()));
		}
		return false;
	}

	inline static const bool isInUserCode(const clang::SourceLocation& pSrcLocation, const clang::ASTContext& pAstContext) {

		const clang::SourceManager &SM = pAstContext.getSourceManager();
		return !(SM.isInSystemHeader(pSrcLocation) || SM.isInSystemMacro(pSrcLocation));
	}

	inline static const int getBaseCount(const clang::CXXRecordDecl* pCxxRecord, std::set<const clang::CXXRecordDecl*>& pVisited)
	{
		int baseCount = pCxxRecord->getNumBases();
		if (baseCount > 0) {
			pVisited.emplace(pCxxRecord);
			for (auto baseItr = pCxxRecord->bases_begin(); baseItr != pCxxRecord->bases_end(); ++baseItr) {
				const clang::CXXRecordDecl *cxxBaseRec = baseItr->getType()->getAsCXXRecordDecl();
				if (isValidCxxRecord(cxxBaseRec) && pVisited.find(cxxBaseRec) == pVisited.end()) {
					baseCount += getBaseCount(cxxBaseRec, pVisited);
				}
			}
		}
		return baseCount;
	}
};
