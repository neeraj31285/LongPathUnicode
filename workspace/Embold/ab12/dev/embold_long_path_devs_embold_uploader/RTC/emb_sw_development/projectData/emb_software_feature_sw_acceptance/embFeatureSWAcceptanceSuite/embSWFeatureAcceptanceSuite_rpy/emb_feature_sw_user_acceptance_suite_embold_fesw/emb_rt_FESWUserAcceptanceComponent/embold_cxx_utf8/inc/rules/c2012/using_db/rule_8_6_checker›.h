#pragma once
#include "GammaRuleCheckerı.hpp"
#include "rule_8_6_astBeanč.h"
#include "rule_8_6_visitorš.h"

class Rule_8_6_Checker : public GammaRuleChecker<Rule_8_6_Visitor, Rule_8_6_astBean> {

public:

	Rule_8_6_Checker();
	
	static void onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_8_6_astBean>>& pTuAstBeans);

	static void onParserFinalPublish(std::unordered_map<std::string, std::vector<Rule_8_6_astBean>>& pTuAstBeans);

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Rule_8_6_Visitor& pVisitor, std::vector<Rule_8_6_astBean>& pAstBeans) override;
};
