
#include "AssignOprConstArg_checkerങ.h"
#pragma warning(push,0)
#include "logger.h"
#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "assignment operator non-const arg";
static const char* const ISSUE_DESC = "Assignment operator must not have non-const argument.";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, AssignOprConstArgChecker)

void AssignOprConstArgChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, AssignOprConstArgVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& assignOprsDecl = pVisitor.getAssignOperatorsDecl();
	for (const FunctionDecl* decl : assignOprsDecl) {

		const string& fullPath = SM.getFilename(decl->getLocation()).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(decl->getLocation());
		codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
	}
}
