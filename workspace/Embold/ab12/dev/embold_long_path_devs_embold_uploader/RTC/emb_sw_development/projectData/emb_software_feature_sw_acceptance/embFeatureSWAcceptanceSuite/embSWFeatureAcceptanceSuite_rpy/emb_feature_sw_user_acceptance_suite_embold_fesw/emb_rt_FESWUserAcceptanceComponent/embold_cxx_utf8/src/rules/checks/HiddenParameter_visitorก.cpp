#include "CheckerUtilsг.h"
#include "HiddenParameter_visitorג.h"

#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

HiddenParameterVisitor::HiddenParameterVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& HiddenParameterVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool HiddenParameterVisitor::VisitFunctionDecl(clang::FunctionDecl *pFuncDecl)
{
	if (!pFuncDecl || pFuncDecl->isInvalidDecl() || pFuncDecl->isImplicit() || 
		!CheckerUtils::isInUserCode(pFuncDecl) || !pFuncDecl->isThisDeclarationADefinition() || 
		!pFuncDecl->hasWrittenPrototype() || pFuncDecl->isTemplateInstantiation()) {
		return true;
	}
	
	m_currentfuncParams.clear();
	for (unsigned index = 0, argCount = pFuncDecl->getNumParams(); index < argCount; ++index) {
		const auto *Param = pFuncDecl->getParamDecl(index);
		m_currentfuncParams.push_back(Param->getNameAsString());
	}

	return true;
}


bool HiddenParameterVisitor::VisitVarDecl(clang::VarDecl* pVarDecl)
{
	if (!pVarDecl || !CheckerUtils::isInUserCode(pVarDecl) || pVarDecl->getKind() == Decl::Kind::ParmVar) {
		return true;
	}

	const auto& varNameStr = pVarDecl->getNameAsString();

	if (find(m_currentfuncParams.begin(), m_currentfuncParams.end(), varNameStr) != m_currentfuncParams.end()) {
		m_voilationsSrcLoc.emplace_back(pVarDecl->getBeginLoc());
	}
}
