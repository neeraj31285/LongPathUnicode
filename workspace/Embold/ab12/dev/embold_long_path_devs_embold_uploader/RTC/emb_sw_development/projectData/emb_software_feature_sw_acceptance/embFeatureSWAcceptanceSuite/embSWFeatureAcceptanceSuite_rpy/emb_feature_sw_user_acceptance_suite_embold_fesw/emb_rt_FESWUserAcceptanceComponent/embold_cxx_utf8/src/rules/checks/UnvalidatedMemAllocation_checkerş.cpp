#include "UnvalidatedMemAllocation_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "unvalidated memory allocation";
static const char* const ISSUE_DESC = "The checker flags code that uses unvalidated data in determining the size of a memory allocation.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, UnvalidatedMemAllocChecker)


void UnvalidatedMemAllocChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedMemAllocVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
