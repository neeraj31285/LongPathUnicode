
#include <iostream>
#include "CheckerUtils.h"
#include "UnusedParameter_visitor.h"

#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

UnusedParameterVisitor::UnusedParameterVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext){
}

const std::vector<clang::SourceLocation>& UnusedParameterVisitor::getViolationSrcLocations(){
	return m_voilationsSrcLoc;
}


bool UnusedParameterVisitor::VisitFunctionDecl(clang::FunctionDecl *pFuncDecl)
{
	if (!CheckerUtils::isInUserCode(pFuncDecl) || !(pFuncDecl->isThisDeclarationADefinition())) {
		return true;
	}

	if (pFuncDecl)
	{
		if (!pFuncDecl->isInvalidDecl() && !pFuncDecl->isImplicit())
		{
			if (!pFuncDecl->hasWrittenPrototype() || pFuncDecl->isTemplateInstantiation()){
				return true;
			}

			if (const auto *Method = dyn_cast<CXXMethodDecl>(pFuncDecl)){
				if (Method->isLambdaStaticInvoker()) {
					return true;
				}
			}

			for (unsigned i = 0, e = pFuncDecl->getNumParams(); i != e; ++i){

				const auto *Param = pFuncDecl->getParamDecl(i);
				if (Param->isUsed() || Param->isReferenced() || !Param->getDeclName() || Param->hasAttr<UnusedAttr>()){
					continue;
				}
				m_voilationsSrcLoc.push_back(Param->getLocation());
			}
		}
	}
	return true;
}