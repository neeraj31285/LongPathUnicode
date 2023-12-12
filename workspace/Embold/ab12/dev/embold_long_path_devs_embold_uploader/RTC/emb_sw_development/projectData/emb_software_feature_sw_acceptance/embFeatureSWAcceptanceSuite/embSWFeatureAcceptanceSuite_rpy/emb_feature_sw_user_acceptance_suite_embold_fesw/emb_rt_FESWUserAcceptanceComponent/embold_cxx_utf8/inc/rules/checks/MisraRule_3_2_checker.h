#pragma once

#include "GammaRuleCheckerı.hpp"
#include "clang/AST/RecursiveASTVisitor.h"
#include "GammaASTBeanæ.h"

class MisraRule_3_2_astBean : public GammaASTBean { };

class MisraRule_3_2_Visitor : public clang::RecursiveASTVisitor<MisraRule_3_2_Visitor> {
public: MisraRule_3_2_Visitor(const clang::ASTContext& pAstContext) { }
};

class MisraRule_3_2_Checker : public GammaRuleChecker<MisraRule_3_2_Visitor, MisraRule_3_2_astBean> {
public:

	MisraRule_3_2_Checker();

	static void onProcessEndPublish(std::unordered_map<std::string, std::vector<MisraRule_3_2_astBean>>& pTuAstBeans);

	static void onParserFinalPublish(std::unordered_map<std::string, std::vector<MisraRule_3_2_astBean>>& pTuAstBeans);

	void onEndOfTranslationUnit(clang::ASTContext& pContext, MisraRule_3_2_Visitor& pVisitor, std::vector<MisraRule_3_2_astBean>& pAstBeans) override;
};
