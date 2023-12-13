
#include "rule_8_7_checker.h"
#include "rule_8_7_visitor.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "MISRA-C2012-8.7";
static const char* const ISSUE_DESC = "Functions and objects should not be defined with external linkage if they are referenced in only one translation unit";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);


ADDTO_GAMMA_RULE_FACTORY(RULE_KEY, Rule_8_7_Checker, true)


Rule_8_7_Checker::Rule_8_7_Checker()
{
}

void Rule_8_7_Checker::onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_8_7_astBean>>& pTuAstBeans)
{
}

void Rule_8_7_Checker::onEndOfTranslationUnit(ASTContext& pContext, Rule_8_7_Visitor& pVisitor, vector<Rule_8_7_astBean>& pAstBeans)
{
	SourceManager& SM = pContext.getSourceManager();
	const auto& externDeclsMap = pVisitor.getExternDeclsMap();
	const auto& formalLinkRefsMap = pVisitor.getFormalLinkageRefMap();

	for (auto& itr : externDeclsMap)
	{
		const string& declName = itr.first;
		const NamedDecl* namedDecl = itr.second;
		const std::string& fullName = SM.getFilename(namedDecl->getLocation()).str();
		const unsigned int lineNum = SM.getPresumedLineNumber(namedDecl->getLocation());

		const bool isNotReferredLocally = (formalLinkRefsMap.find(declName) == formalLinkRefsMap.end());
		if (isNotReferredLocally) {
			codeIssueWriter.Write(emboldCxx::EmboldTidyModule::getRelativePath(fullName), ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
		}
		else {
			Rule_8_7_astBean astObj;
			astObj.setIsDecl("Y");
			astObj.setSymbol(declName);
			astObj.setLine(to_string(lineNum));
			astObj.setFileName(emboldCxx::EmboldTidyModule::getRelativePath(fullName));
			pAstBeans.push_back(astObj);
		}
	}

	for (auto& itr : formalLinkRefsMap) {
		const bool& isExtern = itr.second;
		const string& declName = itr.first;
		if (!isExtern) {
			Rule_8_7_astBean astObj;
			astObj.setIsDecl("N");
			astObj.setSymbol(declName);
			pAstBeans.push_back(astObj);
		}
	}
}


void Rule_8_7_Checker::onParserFinalPublish(std::unordered_map<std::string, std::vector<Rule_8_7_astBean>>& pTuAstBeans)
{
	vector<string> externDecls;
	vector<pair<string, string>> declLoc;
	vector<unordered_set<string>> externLinkages;

	for (auto& itrPair : pTuAstBeans) 
	{
		externLinkages.emplace_back(unordered_set<string>());
		for (Rule_8_7_astBean& astBean : itrPair.second)
		{
			if (astBean.getIsDecl() == "Y") {
				externDecls.push_back(astBean.getSymbol());
				declLoc.push_back(make_pair(astBean.getFileName(), astBean.getLine()));
			}
			else {
				externLinkages.back().insert(astBean.getSymbol());
			}
		}
	}

	for (int index = 0; index < externDecls.size(); ++index)
	{
		bool externLinkageFound = false;
		const string& externDecl = externDecls.at(index);
		for (const auto& linkageSet : externLinkages) {
			if (linkageSet.find(externDecl) != linkageSet.end()) {
				externLinkageFound = true;
				break;
			}
		}
		if (!externLinkageFound) {

			const auto& lineNum = declLoc.at(index).second;
			const auto& relFilePath = declLoc.at(index).first;
			codeIssueWriter.Write(relFilePath, ISSUE_DESC, RULE_KEY, SEVERITY, stoi(lineNum));
		}
	}
}
