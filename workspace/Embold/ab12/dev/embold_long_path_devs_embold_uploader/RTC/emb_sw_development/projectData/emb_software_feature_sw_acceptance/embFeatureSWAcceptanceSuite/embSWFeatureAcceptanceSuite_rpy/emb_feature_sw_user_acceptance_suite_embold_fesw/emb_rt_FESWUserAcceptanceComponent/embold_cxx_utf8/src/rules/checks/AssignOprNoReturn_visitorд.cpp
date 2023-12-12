#include "AssignOprNoReturn_visitorû.h"

using namespace clang;

static const bool isInUserCode(const NamedDecl *D) {

	if (D->getSourceRange().isValid()) {
		const SourceManager &SM = D->getASTContext().getSourceManager();
		return !(SM.isInSystemHeader(D->getLocation()) || SM.isInSystemMacro(D->getLocation()));
	}
	return false;
}


static const bool isValidCxxRecord(const CXXRecordDecl *recordDecl) {

	return (recordDecl && !recordDecl->isInvalidDecl() && (recordDecl->isStruct() || recordDecl->isClass()) &&
		recordDecl->hasDefinition() && !recordDecl->isImplicit() && isInUserCode(recordDecl));
}

const std::vector<const clang::FunctionDecl*>& AssignOprNoReturnVisitor::getAssignOperatorsDecl()
{
	return m_assignOprDecl;
}

AssignOprNoReturnVisitor::AssignOprNoReturnVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

bool AssignOprNoReturnVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!isValidCxxRecord(pCxxRecord)) {
		return true;
	}

	const bool userDefinedAssignOpr = pCxxRecord->hasUserDeclaredCopyAssignment();
	
	if (userDefinedAssignOpr) 
	{
		for (const CXXMethodDecl *cxxMethod : pCxxRecord->methods()) 
		{
			const bool isReturnTypeVoid = cxxMethod->getDeclaredReturnType().getTypePtr()->isVoidType();
			if (cxxMethod->isCopyAssignmentOperator() && isReturnTypeVoid) {
				m_assignOprDecl.push_back(cxxMethod);
			}
		}
	}

	return true;
}
