
#include "GammaASTBean.h"

#include <string>
#include <iostream>

using namespace std;

GammaASTBean::Bind::Bind(const string& pKey) 
	: m_propKey(pKey)
{
}

GammaASTBean::GammaASTBean()
{
}

GammaASTBean::GammaASTBean(const vector<Bind>& pAccessors)
	: m_accessors(pAccessors)
{
}

void GammaASTBean::serialize(ofstream& pOutStream)
{
	bool isFirst = true;
	const size_t& size = m_accessors.size();
	pOutStream << "{";
	for(size_t index = 0; index < size; ++index)
	{
		const string& key = m_accessors[index].m_propKey;
		const string& value = m_accessors[index].m_getter(this);
		if (!value.empty()) {
			pOutStream << (isFirst ? "" : ",") << key << ":" << value;
			isFirst = false;
		}
	}
	pOutStream << "}";
}

void GammaASTBean::deserialize(const std::string& pBeanStr)
{
	bool pairFound = false;
	string propKey, propValue;
	int kPos = -1, vPos = -1, propIndex = 0;
	for (size_t index = 0; index < pBeanStr.size(); ++index) {
		switch (pBeanStr.at(index)) {
		case '{': {
			kPos = index + 1;
			break;
		}
		case '}': {
			propValue = pBeanStr.substr(vPos, index - vPos);
			pairFound = true;
			break;
		}
		case ',': {
			kPos = index + 1;
			propValue = pBeanStr.substr(vPos, index - vPos);
			pairFound = true;
			break;
		}
		case ':': {
			vPos = index + 1;
			propKey = pBeanStr.substr(kPos, index - kPos);
			break;
		}
		default : break;
		}

		if (pairFound) {
			while (propIndex < m_accessors.size()) {
				if (propKey == m_accessors.at(propIndex++).m_propKey) {
					m_accessors[propIndex - 1].m_setter(this, propValue);
					break;
				}
			}
			pairFound = false;
		}
	}
}
