#include "CharSignedDeclaration_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "low";
static const char* const RULE_KEY = "char signedness";
static const char* const ISSUE_DESC = "Avoid declarations of char without explicit specification of \"signed\" or \"unsigned\".";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, CharSignedDeclarationChecker)

void CharSignedDeclarationChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, CharSignedDeclarationVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		
	}
}