#include "AssignOprConstReturn_visitor.h"

using namespace clang;
using namespace std;

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


const std::vector<const clang::FunctionDecl*>& AssignOprConstReturnVisitor::getAssignOperatorsDecl()
{
	return m_assignOprDecl;
}

AssignOprConstReturnVisitor::AssignOprConstReturnVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

bool AssignOprConstReturnVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
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
			if (cxxMethod->isCopyAssignmentOperator() && !isReturnTypeVoid) 
			{
				const string& returnStr = cxxMethod->getReturnType().getAsString();
				const bool isNonConstReturnType = (returnStr.find("const") == string::npos);
				if (isNonConstReturnType) 
				{
					m_assignOprDecl.push_back(cxxMethod);
				}
			}
		}
	}

	return true;
}
