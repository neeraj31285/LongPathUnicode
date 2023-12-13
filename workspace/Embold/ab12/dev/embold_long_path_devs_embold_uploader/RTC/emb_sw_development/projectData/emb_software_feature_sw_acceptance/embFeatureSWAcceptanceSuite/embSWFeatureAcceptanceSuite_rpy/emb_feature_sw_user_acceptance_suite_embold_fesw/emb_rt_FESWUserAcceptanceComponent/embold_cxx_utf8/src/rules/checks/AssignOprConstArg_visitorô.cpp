#include "AssignOprConstArg_visitor.h"

using namespace std;
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


const std::vector<const clang::FunctionDecl*>& AssignOprConstArgVisitor::getAssignOperatorsDecl()
{
	return m_assignOprDecl;
}

AssignOprConstArgVisitor::AssignOprConstArgVisitor(clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

bool AssignOprConstArgVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!isValidCxxRecord(pCxxRecord)) {
		return true;
	}

	const bool userDefinedAssignOpr = pCxxRecord->hasUserDeclaredCopyAssignment();
	const bool assignWithConstParm = pCxxRecord->hasCopyAssignmentWithConstParam();
	if (userDefinedAssignOpr && !assignWithConstParm) 
	{
		for (const CXXMethodDecl *cxxMethod : pCxxRecord->methods()) 
		{
			if (cxxMethod->isCopyAssignmentOperator()) 
			{
				m_assignOprDecl.push_back(cxxMethod);
			}
		}
	}
	return true;
}
