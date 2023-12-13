#pragma once
#include <fstream>
#include <vector>
#include <unordered_map>
#include <functional>

#pragma warning(push,0)
#include "GammaASTBean.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/SourceManager.h"
#pragma warning(pop)

template<class TyVisitor, class TyBean = GammaASTBean>
class GammaRuleChecker : public clang::ASTConsumer
{
	std::string m_checkerName;

	static std::unordered_map<std::string, std::vector<TyBean>> m_tuAstBeansMap;

	static const std::string extractFileName(const std::string& pFilePath);

public:

	void setCheckerName(const std::string& pCheckerName);

	void HandleTranslationUnit(clang::ASTContext& Context) override;

	virtual void onEndOfTranslationUnit(clang::ASTContext& pContext, TyVisitor& pVisitor,
										std::vector<TyBean>& pAstBeans) = 0;

	static void dumpAstBeans(const std::string& pRuleName);
	static void loadAstBeans(const std::string& pRuleName);
	static std::unordered_map<std::string, std::vector<TyBean>>& getTUAstBeanMap();
};
