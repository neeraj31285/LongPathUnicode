#include "UnvalidatedLoopBoundary_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "unvalidated loop boundary";
static const char* const ISSUE_DESC = "The checker reports when an unvalidated argument is used as a loop boundary.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, UnvalidatedLoopBoundaryChecker)


void UnvalidatedLoopBoundaryChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedLoopBoundaryVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
