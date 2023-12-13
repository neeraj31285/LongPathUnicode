
#include "rule_8_6_checker.h"
#include "rule_8_6_visitor.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "MISRA-C2012-8.6";
static const char* const ISSUE_DESC = "An identifier with external linkage shall have exactly one external definition";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY(RULE_KEY, Rule_8_6_Checker, true)

Rule_8_6_Checker::Rule_8_6_Checker()
{
}

void Rule_8_6_Checker::onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_8_6_astBean>>& pTuAstBeans)
{
}

void Rule_8_6_Checker::onEndOfTranslationUnit(ASTContext& pContext, Rule_8_6_Visitor& pVisitor, vector<Rule_8_6_astBean>& pAstBeans)
{
	SourceManager& SM = pContext.getSourceManager();
	const auto& externDefsMap = pVisitor.getExternalDeclDefs();
	const auto& isExternFlags = pVisitor.getIsExternFlags();
	const size_t size = externDefsMap.size();
	
	for (int index = 0; index < size; ++index)
	{
		NamedDecl *decl = externDefsMap.at(index);
		const string& declName = decl->getName().str();
		const string& isExtern = isExternFlags.at(index) ? "Y" : "N";
		const string& fullPath = SM.getFilename(decl->getLocation()).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(decl->getLocation());
		
		pAstBeans.emplace_back(Rule_8_6_astBean());
		pAstBeans.back().setSymbol(declName);
		pAstBeans.back().setIsExtern(isExtern);
		pAstBeans.back().setFileName(relPath);
		pAstBeans.back().setLine(to_string(lineNum));
	}
}


void Rule_8_6_Checker::onParserFinalPublish(std::unordered_map<std::string, std::vector<Rule_8_6_astBean>>& pTuAstBeans)
{
	unordered_map<string, Rule_8_6_astBean> declDefinitionSet;
	for (auto& itrPair : pTuAstBeans) 
	{
		for (Rule_8_6_astBean& astBean : itrPair.second)
		{
			const string& declName = astBean.getSymbol();
			const auto& retItr = declDefinitionSet.emplace(make_pair(declName, astBean));

			if ( !retItr.second) {
				auto& otherAstBean = retItr.first->second;
				if (astBean.getIsExtern() == "Y") {
					const auto& lineNum = astBean.getLine();
					const auto& relFilePath = astBean.getFileName();
					codeIssueWriter.Write(relFilePath, ISSUE_DESC, RULE_KEY, SEVERITY, stoi(lineNum));
				}
				if(otherAstBean.getIsExtern() == "Y") {
					const auto& lineNum = otherAstBean.getLine();
					const auto& relFilePath = otherAstBean.getFileName();
					codeIssueWriter.Write(relFilePath, ISSUE_DESC, RULE_KEY, SEVERITY, stoi(lineNum));
				}
			}
		}
	}
}
