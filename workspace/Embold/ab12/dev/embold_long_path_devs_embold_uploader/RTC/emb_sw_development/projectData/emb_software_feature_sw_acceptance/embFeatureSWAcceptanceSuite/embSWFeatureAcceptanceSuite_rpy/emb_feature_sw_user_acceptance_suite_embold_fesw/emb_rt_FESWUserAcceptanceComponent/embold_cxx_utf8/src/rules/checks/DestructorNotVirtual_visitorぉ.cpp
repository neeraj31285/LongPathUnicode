#include "DestructorNotVirtual_visitorט.h"

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

const std::vector<const clang::CXXDestructorDecl*>& DestructorNotVirtualVisitor::getNonCompliantDestructors()
{
	return m_nonCompliantDtor;
}

DestructorNotVirtualVisitor::DestructorNotVirtualVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

bool DestructorNotVirtualVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!isValidCxxRecord(pCxxRecord) || !pCxxRecord->isPolymorphic() ||
		pCxxRecord->bases_begin() == pCxxRecord->bases_end()) {
		return true;
	}

	const CXXDestructorDecl* dtor =  pCxxRecord->getDestructor();

	if (!(dtor && dtor->hasBody() && dtor->isUserProvided())) {
		return true;
	}

	const Stmt* dtorBody = dtor->getBody();
	const bool bodyEmpty = (dtorBody->child_begin() == dtorBody->child_end());

	if (!bodyEmpty && !dtor->isVirtual()) {
		m_nonCompliantDtor.push_back(dtor);
	}

	return true;
}
