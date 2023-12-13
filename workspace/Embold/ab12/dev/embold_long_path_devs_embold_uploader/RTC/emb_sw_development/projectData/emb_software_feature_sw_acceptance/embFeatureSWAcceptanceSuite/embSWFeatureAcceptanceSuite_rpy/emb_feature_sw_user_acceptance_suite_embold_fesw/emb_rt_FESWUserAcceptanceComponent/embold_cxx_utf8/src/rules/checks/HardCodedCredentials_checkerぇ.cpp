#include "HardCodedCredentials_checker.h"
#pragma warning(push,0)


#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "hard coded credentials";
static const char* const ISSUE_DESC = "Use of hard-coded credentials can lead to a malicious user bypassing authentication.";



ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, HardCodedCredentialsChecker)

void HardCodedCredentialsChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, HardCodedCredentialsVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}
