#include "rule_5_9_visitorΑ.h"

using namespace std;
using namespace clang;

static bool isInUserCode(NamedDecl* D)
{
	SourceManager& SM = D->getASTContext().getSourceManager();
	return !(SM.isInSystemHeader(D->getLocation()) || SM.isInSystemMacro(D->getLocation()));
}


const unordered_map<string, NamedDecl*>& Rule_5_9_Visitor::getNonInternalLinkedDeclsMap()
{
	return m_nonInternalLinkedDecls;
}


const unordered_map<string, NamedDecl*>& Rule_5_9_Visitor::getInternalLinkedDeclsMap()
{
	return m_internalLinkedDecls;
}


Rule_5_9_Visitor::Rule_5_9_Visitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

bool Rule_5_9_Visitor::VisitVarDecl(VarDecl * pVarD)
{
	if (pVarD != nullptr && isInUserCode(pVarD))
	{
		const string& name = pVarD->getName().str();
		if (pVarD->getLinkageInternal() == Linkage::InternalLinkage) {
			m_internalLinkedDecls.emplace(make_pair(name, pVarD));
		}
		else {
			m_nonInternalLinkedDecls.emplace(make_pair(name, pVarD));
		}
	}
	return true;
}


bool Rule_5_9_Visitor::VisitFunctionDecl(FunctionDecl * pFuncD)
{
	if (pFuncD != nullptr && isInUserCode(pFuncD) && !pFuncD->isInlineSpecified())
	{
		const string& name = pFuncD->getName().str();
		if (pFuncD->getLinkageInternal() == Linkage::InternalLinkage) {
			m_internalLinkedDecls.emplace(make_pair(name, pFuncD));
		}
		else {
			m_nonInternalLinkedDecls.emplace(make_pair(name, pFuncD));
		}
	}
	return true;
}
