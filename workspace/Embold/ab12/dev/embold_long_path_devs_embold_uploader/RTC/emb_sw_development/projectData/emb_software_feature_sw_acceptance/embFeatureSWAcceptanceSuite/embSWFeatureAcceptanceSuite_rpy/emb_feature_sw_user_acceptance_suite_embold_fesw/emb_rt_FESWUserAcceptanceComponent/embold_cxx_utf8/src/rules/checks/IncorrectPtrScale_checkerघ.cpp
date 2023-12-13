#include "IncorrectPtrScale_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "incorrect pointer scaling";
static const char* const ISSUE_DESC = "Implicit scaling of pointers in an arithmetic operation must result in correctly aligned memory address.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, IncorrectPtrScaleChecker)

void IncorrectPtrScaleChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, IncorrectPtrScaleVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);

		if (!relPath.empty()) {
			
		}
	}
}
