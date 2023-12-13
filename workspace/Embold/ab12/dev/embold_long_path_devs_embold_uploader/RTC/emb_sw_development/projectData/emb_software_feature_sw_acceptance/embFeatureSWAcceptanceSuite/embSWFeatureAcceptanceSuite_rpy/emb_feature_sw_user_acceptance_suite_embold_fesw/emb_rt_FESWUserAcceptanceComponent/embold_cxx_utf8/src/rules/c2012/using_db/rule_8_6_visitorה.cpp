#include "rule_8_6_visitor.h"

using namespace std;
using namespace clang;


static bool isInUserCode(clang::NamedDecl* D)
{
	clang::SourceManager& SM = D->getASTContext().getSourceManager();
	return !(SM.isInSystemHeader(D->getLocation()) || SM.isInSystemMacro(D->getLocation()));
}


const std::vector<bool>& Rule_8_6_Visitor::getIsExternFlags()
{
	return m_isExternFlags;
}


const std::vector<clang::NamedDecl*>& Rule_8_6_Visitor::getExternalDeclDefs()
{
	return m_externalLinkDefs;
}


bool Rule_8_6_Visitor::VisitFunctionDecl(clang::FunctionDecl * pFnDecl)
{
	if (   pFnDecl != nullptr 
		&& isInUserCode(pFnDecl) 
		&& pFnDecl->hasExternalFormalLinkage()
		&& pFnDecl->isThisDeclarationADefinition()) {

		const bool isExtern = pFnDecl->getStorageClass() == clang::StorageClass::SC_Extern;
		const std::string& name = pFnDecl->getName().str();
		m_externalLinkDefs.push_back(pFnDecl);
		m_isExternFlags.push_back(isExtern);
	}
	return true;
}


Rule_8_6_Visitor::Rule_8_6_Visitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

bool Rule_8_6_Visitor::VisitVarDecl(clang::VarDecl * pVarD)
{
	if (   pVarD != nullptr 
		&& isInUserCode(pVarD)
		&& pVarD->hasExternalFormalLinkage()
		&& pVarD->isThisDeclarationADefinition() == VarDecl::DefinitionKind::Definition){

		const std::string& name = pVarD->getName().str();
		const bool isExtern = pVarD->hasExternalStorage();
		m_externalLinkDefs.push_back(pVarD);
		m_isExternFlags.push_back(isExtern);
	}
	return true;
}