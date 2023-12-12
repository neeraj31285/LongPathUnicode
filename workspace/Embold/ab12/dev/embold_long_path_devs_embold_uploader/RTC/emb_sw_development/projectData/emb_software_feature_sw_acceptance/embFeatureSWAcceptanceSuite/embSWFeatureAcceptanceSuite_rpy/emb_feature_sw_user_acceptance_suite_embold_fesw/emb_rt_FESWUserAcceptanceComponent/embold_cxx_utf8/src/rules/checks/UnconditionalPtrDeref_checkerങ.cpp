
#include "UnconditionalPtrDeref_checkerß.h"
#pragma warning(push,0)
#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "high";
static const char* const RULE_KEY = "unconditional pointer return deref";
static const char* const ISSUE_DESC = "Pointer returned from a function call must be null checked before dreferencing.";

extern CodeIssueWriter codeIssueWriter;

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, UnconditionalPtrDerefChecker)


void UnconditionalPtrDerefChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, UnconditionalPtrDerefVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	const auto& voilationSrcLocs = pVisitor.getViolationSrcLocations();

	for (const SourceLocation& srcLoc : voilationSrcLocs) {

		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
	}

	const auto& funcCallersMap = pVisitor.getFuncCallsWithReturnedPtr();
	const auto& uncheckedPtrArgDrefs = pVisitor.getUnconditionalDerefFuncArgs();

	for (auto& funcCalle : funcCallersMap)
	{
		const auto& funcName = funcCalle.first;
		const auto& argName = funcCalle.second;
		const auto& itr = uncheckedPtrArgDrefs.find(funcName);

		if (itr != uncheckedPtrArgDrefs.end()) {
			const auto& locItr = itr->second.find(argName);
			if (locItr != itr->second.end()) {

				const string& fullPath = SM.getFilename(locItr->second).str();
				const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
				const size_t lineNum = SM.getPresumedLineNumber(locItr->second);
				codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
			}
		}
	}
}
