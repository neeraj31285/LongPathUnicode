#include "HiddenParameter_checkerכ.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "low";
static const char* const RULE_KEY = "hidden parameter";
static const char* const ISSUE_DESC = "A function declares a variable with the same name as a parameter, which is then hidden.";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, HiddenParameterChecker)

void HiddenParameterChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, HiddenParameterVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();
	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
	}
}
