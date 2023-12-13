#include "BitwiseOprandsMismatch_checker.h"
#pragma warning(push,0)

#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "bitwise operands mismatch";
static const char* const ISSUE_DESC = "Both operands of a bitwise operation must normally be either 32-bit or 64-bit values. Although a 64-bit mask used on a 32-bit value is compliant.";



ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, BitOprandsMismatchChecker)

void BitOprandsMismatchChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, BitOprandsMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	const auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);

		if (!relPath.empty()) {
			
		}
	}
}
