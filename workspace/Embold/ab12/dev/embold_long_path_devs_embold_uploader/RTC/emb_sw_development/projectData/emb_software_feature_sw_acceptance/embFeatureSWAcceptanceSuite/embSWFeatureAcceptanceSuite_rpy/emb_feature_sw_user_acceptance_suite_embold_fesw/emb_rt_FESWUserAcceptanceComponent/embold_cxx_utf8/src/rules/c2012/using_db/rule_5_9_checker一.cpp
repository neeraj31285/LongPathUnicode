#include "rule_5_9_checkerš.h"
#include "rule_5_9_visitorΑ.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "MISRA-C2012-5.9";
static const char* const ISSUE_DESC = "Identifiers that define objects or functions with internal linkage should be unique.";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY(RULE_KEY, Rule_5_9_Checker, true)

Rule_5_9_Checker::Rule_5_9_Checker()
{
}

void Rule_5_9_Checker::onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_5_9_astBean>>& pTuAstBeans)
{
}

void Rule_5_9_Checker::onEndOfTranslationUnit(ASTContext& pContext, Rule_5_9_Visitor& pVisitor, vector<Rule_5_9_astBean>& pAstBeans)
{
	SourceManager& SM = pContext.getSourceManager();
	const auto& internalLinkedDeclsMap = pVisitor.getInternalLinkedDeclsMap();
	const auto& nonInternalLinkedDeclsMap = pVisitor.getNonInternalLinkedDeclsMap();

	for (auto& itr : internalLinkedDeclsMap) {

		const string& declName = itr.first;
		const NamedDecl* namedDecl = itr.second;
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(SM.getFilename(namedDecl->getLocation()).str());

		const auto& otherDeclItr = nonInternalLinkedDeclsMap.find(declName);
		if (otherDeclItr != nonInternalLinkedDeclsMap.end()) {
			const NamedDecl* otherDecl = otherDeclItr->second;
			const unsigned int lineNum = SM.getPresumedLineNumber(otherDecl->getLocation());
			codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
		}
		else {

			Rule_5_9_astBean astObj;
			const unsigned int lineNum = SM.getPresumedLineNumber(namedDecl->getLocation());

			astObj.setIsStatic("Y");
			astObj.setSymbol(declName);
			astObj.setLine(to_string(lineNum));
			astObj.setFileName(relPath);
			pAstBeans.push_back(astObj);
		}
	}
		
	for (auto& itr : nonInternalLinkedDeclsMap)
	{
		const string& declName = itr.first;
		const NamedDecl* namedDecl = itr.second;
		const std::string& fullName = SM.getFilename(namedDecl->getLocation()).str();
		const unsigned int lineNum = SM.getPresumedLineNumber(namedDecl->getLocation());

		Rule_5_9_astBean astObj;
		astObj.setIsStatic("N");
		astObj.setSymbol(declName);
		astObj.setLine(to_string(lineNum));
		astObj.setFileName(emboldCxx::EmboldTidyModule::getRelativePath(fullName));
		pAstBeans.push_back(astObj);
	}
}


void Rule_5_9_Checker::onParserFinalPublish(unordered_map<string, vector<Rule_5_9_astBean>>& pTuAstBeans)
{

	unordered_set<string> allStaticGlobalDecls;

	for (auto& itrPair : pTuAstBeans) 
	{
		for (auto& astBean : itrPair.second) {
			if (astBean.getIsStatic() == "Y") {

				const string& declName = astBean.getSymbol();
				const bool& isSuccess = allStaticGlobalDecls.emplace(declName).second;
				if (!isSuccess) {
					codeIssueWriter.Write(astBean.getFileName(), ISSUE_DESC, RULE_KEY, SEVERITY, stoi(astBean.getLine()));
				}
			}
		}
	}

	for (auto& itrPair : pTuAstBeans)
	{
		for (auto& astBean : itrPair.second) {
			if (astBean.getIsStatic() == "N") {

				const string& declName = astBean.getSymbol();
				const bool& isUnique = allStaticGlobalDecls.find(declName) == allStaticGlobalDecls.end();
				if (!isUnique) {
					codeIssueWriter.Write(astBean.getFileName(), ISSUE_DESC, RULE_KEY, SEVERITY, stoi(astBean.getLine()));
				}
			}
		}
	}
}
