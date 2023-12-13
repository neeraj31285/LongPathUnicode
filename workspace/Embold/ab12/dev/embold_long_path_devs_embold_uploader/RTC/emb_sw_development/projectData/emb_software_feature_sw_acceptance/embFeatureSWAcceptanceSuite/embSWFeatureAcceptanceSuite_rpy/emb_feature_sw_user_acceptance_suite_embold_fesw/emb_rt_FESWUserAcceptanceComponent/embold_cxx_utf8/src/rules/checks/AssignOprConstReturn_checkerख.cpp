#include "AssignOprConstReturn_checker.h"
#pragma warning(push,0)


#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "assignment operator non-const return";
static const char* const ISSUE_DESC = "Assignment operator must return const qualified valid object.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, AssignOprConstReturnChecker)

void AssignOprConstReturnChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstReturnVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& assignOprsDecl = pVisitor.getAssignOperatorsDecl();
	for (const FunctionDecl* decl : assignOprsDecl) {

		const string& fullPath = SM.getFilename(decl->getLocation()).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(decl->getLocation());
		
	}
}
