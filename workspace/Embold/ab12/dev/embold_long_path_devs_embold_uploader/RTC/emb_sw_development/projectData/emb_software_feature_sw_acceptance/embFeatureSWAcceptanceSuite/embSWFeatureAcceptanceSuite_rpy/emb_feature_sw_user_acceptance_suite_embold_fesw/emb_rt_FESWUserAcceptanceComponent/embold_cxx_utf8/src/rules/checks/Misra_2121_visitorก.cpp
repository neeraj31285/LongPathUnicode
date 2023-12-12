#include "CheckerUtilsг.h"
#include "Misra_2121_visitorг.h"
#include "llvm/Support/Path.h"

using namespace std;
using namespace clang;

Misra_2121Visitor::Misra_2121Visitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& Misra_2121Visitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool Misra_2121Visitor::VisitCallExpr(clang::CallExpr *pCallExpr)
{
	if(pCallExpr->getDirectCallee() != nullptr)
	{
		std::string restricted_func = "system";
		std::string restricted_header = "stdlib.h";
		std::string funct_name = pCallExpr->getDirectCallee()->getNameInfo().getAsString();

		auto& SM = m_astContext.getSourceManager();
		const string& fullPath = SM.getFilename(pCallExpr->getDirectCallee()->getLocation()).str();
		std::string fileName = llvm::sys::path::filename(fullPath).str();
		
		if((restricted_func == funct_name) && (fileName == restricted_header ))
		{
			m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
		}
	}
	return true;
}

bool Misra_2121Visitor::VisitVarDecl(clang::VarDecl *pVarDecl)
{
	if(pVarDecl->getName().str() == "system")
	{
		m_voilationsSrcLoc.emplace_back(pVarDecl->getBeginLoc());
	}
	return true;
}
