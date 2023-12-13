
#include "InconsistentNullPtrCheck_checker.h"
#pragma warning(push,0)

#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "inconsistent null pointer check";
static const char* const ISSUE_DESC = "Pointer passed as an argument is being dereferenced before the null check in the function.";



ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, InconsistentNullPtrCheckChecker)


void InconsistentNullPtrCheckChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, InconsistentNullPtrCheckVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
