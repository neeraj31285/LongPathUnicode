#include "PrintStrFormatMismatch_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "print string format mismatch";
static const char* const ISSUE_DESC = "The format string specified in the print function depicts an equal number of parameters according to the format specifications.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, PrintStrFormatMismatchChecker)

void PrintStrFormatMismatchChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, PrintStrFormatMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
