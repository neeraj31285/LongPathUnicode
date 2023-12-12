#include "CheckerUtilsг.h"
#include "MainReturnsVoid_visitorה.h"

#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

MainReturnsVoidVisitor::MainReturnsVoidVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& MainReturnsVoidVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool MainReturnsVoidVisitor::VisitFunctionDecl(clang::FunctionDecl *pFuncDecl)
{
	if (CheckerUtils::isInUserCode(pFuncDecl) && pFuncDecl->isMain() && 
		pFuncDecl->getReturnType().getAsString() == string("void")) {
		m_voilationsSrcLoc.emplace_back(pFuncDecl->getLocation());
	}
	return true;
}
