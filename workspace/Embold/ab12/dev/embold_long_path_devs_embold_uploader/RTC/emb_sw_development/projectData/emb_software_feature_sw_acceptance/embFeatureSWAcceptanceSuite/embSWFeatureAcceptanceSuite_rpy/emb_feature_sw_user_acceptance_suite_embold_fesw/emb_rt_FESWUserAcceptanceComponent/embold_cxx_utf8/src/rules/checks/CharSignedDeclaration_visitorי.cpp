#include "CheckerUtilsг.h"
#include "CharSignedDeclaration_visitorח.h"

#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

static const char* const RAW_CHAR = "char";

CharSignedDeclarationVisitor::CharSignedDeclarationVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& CharSignedDeclarationVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool CharSignedDeclarationVisitor::VisitVarDecl(VarDecl *pVarDecl)
{
	if (!pVarDecl || !CheckerUtils::isInUserCode(pVarDecl) || pVarDecl->getKind() == Decl::Kind::ParmVar) {
		return true;
	}

	QualType type = pVarDecl->getType();

	if (!type.isNull())
	{
		const QualType &qualType = (type->isPointerType() || type->isReferenceType()) ?
									type->getPointeeType().getCanonicalType() : type.getCanonicalType();

		if (qualType.getUnqualifiedType().getAsString() == RAW_CHAR)
		{
			m_voilationsSrcLoc.emplace_back(pVarDecl->getBeginLoc());
		}
	}

	return true;
}
