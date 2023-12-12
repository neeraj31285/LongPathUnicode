
#include <string>
#include "GammaRuleFactoryก.h"
//#include "ReadSettings.hpp"
//#include <boost/filesystem.hpp>

using namespace std;

GammaRuleFactory::GammaRuleFactory()
{
}

GammaRuleFactory::~GammaRuleFactory()
{
}

const GammaRuleFactory& GammaRuleFactory::Instance()
{
	static GammaRuleFactory instance;
	return instance;
}

const std::unordered_map<std::string, std::pair<RULE_CONSTRUCTOR, bool>>& GammaRuleFactory::getRuleConstructors() const
{
	return m_factoryCtorMap;
}

const bool GammaRuleFactory::addFactoryConstructor(const string& pRuleName, RULE_CONSTRUCTOR pFactoryCtor, const bool pIsCRule) const
{
	return m_factoryCtorMap.emplace(make_pair(pRuleName, make_pair(pFactoryCtor, pIsCRule))).second;
}

const bool GammaRuleFactory::addParserFinalPublisher(const string& pRuleName, RULE_PUBLISHER pPublisher) const
{
	return m_parserFinalPublisherMap.emplace(make_pair(pRuleName, pPublisher)).second;
}

const bool GammaRuleFactory::addProcessEndPublisher(const string& pRuleName, RULE_PUBLISHER pPublisher) const
{
	return m_processEndPublisherMap.emplace(make_pair(pRuleName, pPublisher)).second;
}

void GammaRuleFactory::performProcessEndPublish() const
{
	//for (auto itr : m_processEndPublisherMap)
	//{
	//	if (ReadSettings::IsRuleEnabled(itr.first)) {
	//		(itr.second)();
	//	}
	//}
}

void GammaRuleFactory::performParserFinalPublish() const
{
	//for (auto itr : m_parserFinalPublisherMap)
	//{
	//	const string& ruleKey = itr.first;
	//	if (ReadSettings::IsRuleEnabled(ruleKey)) {
	//		(itr.second)();
	//	}
	//	const boost::filesystem::path& tmp_path = (boost::filesystem::temp_directory_path() / ruleKey);
	//	if (boost::filesystem::exists(tmp_path)) {
	//		boost::filesystem::remove_all(tmp_path);
	//	}
	//}
}
