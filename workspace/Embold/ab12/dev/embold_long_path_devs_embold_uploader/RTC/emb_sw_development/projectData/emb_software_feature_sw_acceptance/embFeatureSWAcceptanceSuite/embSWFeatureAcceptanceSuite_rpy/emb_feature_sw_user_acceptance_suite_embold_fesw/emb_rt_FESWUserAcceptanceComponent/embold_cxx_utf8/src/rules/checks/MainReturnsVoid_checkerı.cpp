#include "MainReturnsVoid_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "low";
static const char* const RULE_KEY = "main returns void";
static const char* const ISSUE_DESC = "The return type of main() is void, which indicates that it will not return an integer value. But, the return type of main() should be an integer.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, MainReturnsVoidChecker)

void MainReturnsVoidChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, MainReturnsVoidVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
