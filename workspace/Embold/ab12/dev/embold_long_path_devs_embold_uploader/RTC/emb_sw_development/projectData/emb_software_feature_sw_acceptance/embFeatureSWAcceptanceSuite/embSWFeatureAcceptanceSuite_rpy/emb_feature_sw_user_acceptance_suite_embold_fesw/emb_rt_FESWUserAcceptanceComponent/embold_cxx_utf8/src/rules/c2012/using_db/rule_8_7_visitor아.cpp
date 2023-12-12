#include "rule_8_7_visitoræ.h"

using namespace std;
using namespace clang;


static bool isInUserCode(clang::NamedDecl* D)
{
	clang::SourceManager& SM = D->getASTContext().getSourceManager();
	return !(SM.isInSystemHeader(D->getLocation()) || SM.isInSystemMacro(D->getLocation()));
}

const std::unordered_map<std::string, bool>& Rule_8_7_Visitor::getFormalLinkageRefMap()
{
	return m_formalLinkRefs;
}

const std::unordered_map<std::string, clang::NamedDecl*>& Rule_8_7_Visitor::getExternDeclsMap()
{
	return m_externDecls;
}

bool Rule_8_7_Visitor::VisitFunctionDecl(clang::FunctionDecl * pFnDecl)
{
	if (   pFnDecl != nullptr 
		&& isInUserCode(pFnDecl) 
		&& pFnDecl->hasExternalFormalLinkage() 
		&& pFnDecl->getStorageClass() == clang::StorageClass::SC_Extern) {		

		const std::string& name = pFnDecl->getName().str();
		m_externDecls.emplace(make_pair(name, pFnDecl));
	}
	return true;
}


Rule_8_7_Visitor::Rule_8_7_Visitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

bool Rule_8_7_Visitor::VisitVarDecl(clang::VarDecl * pVarD)
{
	if (   pVarD != nullptr 
		&& isInUserCode(pVarD)
		&& pVarD->hasExternalFormalLinkage() 
		&& pVarD->hasExternalStorage())
	{
		const std::string& name = pVarD->getName().str();
		m_externDecls.emplace(make_pair(name, pVarD));
	}
	return true;
}



bool Rule_8_7_Visitor::VisitCallExpr(clang::CallExpr * pCallExpr)
{
	clang::FunctionDecl* fnDecl = pCallExpr->getDirectCallee();

	if (   fnDecl != nullptr 
		&& isInUserCode(fnDecl) 
		&& fnDecl->hasExternalFormalLinkage()) {

		const std::string& name = fnDecl->getName().str();
		const bool isExtern = (fnDecl->getStorageClass() == clang::StorageClass::SC_Extern);
		m_formalLinkRefs.insert(make_pair(name, isExtern));
	}

	return true;
}



bool Rule_8_7_Visitor::VisitDeclRefExpr(clang::DeclRefExpr * pRfExpr)
{
	clang::NamedDecl* namedD = pRfExpr->getDecl();

	if (   namedD != nullptr 
		&& isInUserCode(namedD) 
		&& namedD->hasExternalFormalLinkage() 
		&& clang::isa<clang::VarDecl>(namedD))
	{
		clang::VarDecl* varD = static_cast<clang::VarDecl*>(namedD);
		const string& name = varD->getName().str();
		const  bool isExtern = varD->hasExternalStorage();
		m_formalLinkRefs.insert(make_pair(name, isExtern));
	}
	return true;
}
