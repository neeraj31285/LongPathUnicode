#pragma once
#include "GammaRuleCheckerı.hpp"
#include "rule_5_9_astBeanå.h"
#include "rule_5_9_visitorΑ.h"

class Rule_5_9_Checker : public GammaRuleChecker<Rule_5_9_Visitor, Rule_5_9_astBean> {

public:

	Rule_5_9_Checker();

	static void onProcessEndPublish(std::unordered_map<std::string, std::vector<Rule_5_9_astBean>>& pTuAstBeans);

	static void onParserFinalPublish(std::unordered_map<std::string, std::vector<Rule_5_9_astBean>>& pTuAstBeans);

	void onEndOfTranslationUnit(clang::ASTContext& pContext, Rule_5_9_Visitor& pVisitor, std::vector<Rule_5_9_astBean>& pAstBeans) override;
};
