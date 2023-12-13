#include "ShallowCopyConstruct_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "shallow copy construct";
static const char* const ISSUE_DESC = "Shallow copy and default copy constructor must be avoided when dynamically allocated resources are involved.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, ShallowCopyConstructChecker)


void ShallowCopyConstructChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, ShallowCopyConstructVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
