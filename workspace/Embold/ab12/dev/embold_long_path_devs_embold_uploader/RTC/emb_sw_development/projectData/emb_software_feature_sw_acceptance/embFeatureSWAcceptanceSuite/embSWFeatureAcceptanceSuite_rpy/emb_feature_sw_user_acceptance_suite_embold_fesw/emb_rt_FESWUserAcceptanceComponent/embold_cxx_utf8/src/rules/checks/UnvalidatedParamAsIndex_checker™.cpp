#include "UnvalidatedParamAsIndex_checkerа.h"
#pragma warning(push,0)

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "unvalidated param as index";
static const char* const ISSUE_DESC = "An unvalidated argument is passed to a function that uses it to access an array.";

extern CodeIssueWriter codeIssueWriter;

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, UnvalidatedParamAsIndexChecker)

void UnvalidatedParamAsIndexChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, UnvalidatedParamAsIndexVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
	}
}
