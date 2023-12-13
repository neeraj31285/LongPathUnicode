#pragma once
#include "GammaRuleChecker.hpp"
#include "rule_8_7_astBean.h"
#include "rule_8_7_visitor.h"

class Rule_8_7_Checker : public GammaRuleChecker<Rule_8_7_Visitor, Rule_8_7_astBean> {

public:

	Rule_8_7_Checker();
	
	static void onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_8_7_astBean>>& pTuAstBeans);

	static void onParserFinalPublish(std::unordered_map<std::string, std::vector<Rule_8_7_astBean>>& pTuAstBeans);

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Rule_8_7_Visitor& pVisitor, std::vector<Rule_8_7_astBean>& pAstBeans) override;
};
