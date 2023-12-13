#include "Misra_2121_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "MISRA-C2012-21.21";
static const char* const ISSUE_DESC = "The Standard Library function system of <stdlib.h> shall not be used";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, Misra_2121Checker)


void Misra_2121Checker::onEndOfTranslationUnit(clang::ASTContext& pContext, Misra_2121Visitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
