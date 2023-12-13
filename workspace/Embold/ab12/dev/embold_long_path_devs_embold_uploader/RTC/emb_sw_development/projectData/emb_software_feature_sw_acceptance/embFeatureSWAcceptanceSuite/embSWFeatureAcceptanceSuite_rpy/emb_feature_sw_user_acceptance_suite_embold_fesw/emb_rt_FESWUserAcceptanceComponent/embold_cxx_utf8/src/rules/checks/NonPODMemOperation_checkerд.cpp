#include "NonPODMemOperation_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "non POD memory operation";
static const char* const ISSUE_DESC = "Low level memory manipulation routines should be never used for initializing objects of non-POD (plain old data) types.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, NonPODMemOperationChecker)


void NonPODMemOperationChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, NonPODMemOperationVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
