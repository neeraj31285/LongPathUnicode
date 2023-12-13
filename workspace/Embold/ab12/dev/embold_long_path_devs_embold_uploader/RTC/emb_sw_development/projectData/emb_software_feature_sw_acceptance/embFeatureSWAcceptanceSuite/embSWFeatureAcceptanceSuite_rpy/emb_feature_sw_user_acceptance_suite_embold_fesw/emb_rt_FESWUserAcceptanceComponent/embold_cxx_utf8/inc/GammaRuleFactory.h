#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#pragma warning(push,0)
#include "clang/AST/ASTConsumer.h"
#pragma warning(pop)

using RULE_PUBLISHER = void(*)();
using RULE_CONSTRUCTOR = clang::ASTConsumer*(*)();

class GammaRuleFactory {

	mutable std::unordered_map<std::string, RULE_PUBLISHER> m_parserFinalPublisherMap;
	mutable std::unordered_map<std::string, RULE_PUBLISHER> m_processEndPublisherMap;
	mutable std::unordered_map<std::string, std::pair<RULE_CONSTRUCTOR, bool>> m_factoryCtorMap;

	GammaRuleFactory();
	~GammaRuleFactory();
	GammaRuleFactory(GammaRuleFactory&&) = delete;
	GammaRuleFactory(const GammaRuleFactory&) = delete;
	GammaRuleFactory& operator=(const GammaRuleFactory&) = delete;

public:

	static const GammaRuleFactory& Instance();

	void performProcessEndPublish() const;
	void performParserFinalPublish() const;
	
	const std::unordered_map<std::string, std::pair<RULE_CONSTRUCTOR, bool>>& getRuleConstructors() const;

	const bool addProcessEndPublisher(const std::string& pRuleName, RULE_PUBLISHER pPublisher) const;
	const bool addParserFinalPublisher(const std::string& pRuleName, RULE_PUBLISHER pPublisher) const;
	const bool addFactoryConstructor(const std::string& pRuleName, RULE_CONSTRUCTOR pFactoryCtor, const bool pIsCRule) const;
	
};


#define ADDTO_GAMMA_RULE_FACTORY_CTOR(_ruleKey, _ruleType)														\
static clang::ASTConsumer* factoryConstructor() {																\
	auto ruleObject = new _ruleType();																			\
	ruleObject->setCheckerName(_ruleKey);																		\
	return ruleObject;																							\
}																												\
																												\
const bool _ruleType::initialize(){																				\
	GammaRuleFactory::Instance().addFactoryConstructor(_ruleKey, &factoryConstructor, false);					\
	return true;																								\
}

#define ADDTO_GAMMA_RULE_FACTORY(_ruleKey, _ruleType, _isCSpecificRule)											\
static clang::ASTConsumer* factoryConstructor() {																\
	auto ruleObject = new _ruleType();																			\
	ruleObject->setCheckerName(_ruleKey);																		\
	return ruleObject;																							\
}																												\
static void parserFinalPublish() {																				\
	_ruleType::loadAstBeans(_ruleKey);																			\
	_ruleType::onParserFinalPublish(_ruleType::getTUAstBeanMap());												\
}																												\
static void processEndPublish() {																				\
	_ruleType::onProcessEndPublish(_ruleType::getTUAstBeanMap());												\
	_ruleType::dumpAstBeans(_ruleKey);																			\
}																												\
																												\
const bool _ruleType::initialize(){																				\
	GammaRuleFactory::Instance().addFactoryConstructor(_ruleKey, &factoryConstructor, _isCSpecificRule);		\
	GammaRuleFactory::Instance().addParserFinalPublisher(_ruleKey, &parserFinalPublish);						\
	GammaRuleFactory::Instance().addProcessEndPublisher(_ruleKey, &processEndPublish);							\
	return true;																								\
}																												

