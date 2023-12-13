#include "MisraRule_3_2_checker.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "low";
static const char* const RULE_KEY = "MISRA-C2012-3.2";
static const char* const ISSUE_DESC = "Line-splicing shall not be used in // comments.";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY(RULE_KEY, MisraRule_3_2_Checker, true)

MisraRule_3_2_Checker::MisraRule_3_2_Checker()
{
}

void MisraRule_3_2_Checker::onProcessEndPublish(unordered_map<string, vector<MisraRule_3_2_astBean>>& pTuAstBeans)
{
}

void MisraRule_3_2_Checker::onParserFinalPublish(unordered_map<string, vector<MisraRule_3_2_astBean>>& pTuAstBeans)
{
}

void MisraRule_3_2_Checker::onEndOfTranslationUnit(clang::ASTContext& pContext, MisraRule_3_2_Visitor& pVisitor, vector<MisraRule_3_2_astBean>& pAstBeans)
{
	vector<SourceLocation> voilationSrcLocs;
	auto& SM = pContext.getSourceManager();
	const auto& commentsList = pContext.getRawCommentList().getComments();

	for (auto& itr : commentsList)
	{
		const auto& srcRange = itr->getSourceRange();
		const string& commentStr = Lexer::getSourceText(CharSourceRange::getTokenRange(srcRange), SM, LangOptions()).str();

		if (commentStr.size() > 1 && commentStr.at(0) == '/' && commentStr.at(1) == '/') {

			const auto& pos = commentStr.find_last_of('\\');
			if (pos != string::npos && pos < commentStr.size()) {
				const char& nextCh = commentStr.at(pos + 1);
				if (nextCh == '\r' || nextCh == '\n') {
					voilationSrcLocs.emplace_back(srcRange.getBegin());
				}
			}
		}
	}

	for (const SourceLocation& srcLoc : voilationSrcLocs) {
		const string& fullPath = SM.getFilename(srcLoc).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(srcLoc);
		codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
	}
}