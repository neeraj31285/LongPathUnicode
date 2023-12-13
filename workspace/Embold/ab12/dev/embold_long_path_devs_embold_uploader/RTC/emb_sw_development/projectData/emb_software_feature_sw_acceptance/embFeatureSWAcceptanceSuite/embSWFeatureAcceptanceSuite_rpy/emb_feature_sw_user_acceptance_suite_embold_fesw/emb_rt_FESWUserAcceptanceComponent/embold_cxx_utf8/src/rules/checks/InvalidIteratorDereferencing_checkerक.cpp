#include "InvalidIteratorDereferencing_checker.h"
#pragma warning(push,0)

#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "invalid iterator dereferencing";
static const char* const ISSUE_DESC = "Dereferencing an iterator which may be equal to end() can result in illegal memory access.";

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, InvalidIteratorDereferencingChecker)


void InvalidIteratorDereferencingChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, InvalidIteratorDereferencingVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
	}
}