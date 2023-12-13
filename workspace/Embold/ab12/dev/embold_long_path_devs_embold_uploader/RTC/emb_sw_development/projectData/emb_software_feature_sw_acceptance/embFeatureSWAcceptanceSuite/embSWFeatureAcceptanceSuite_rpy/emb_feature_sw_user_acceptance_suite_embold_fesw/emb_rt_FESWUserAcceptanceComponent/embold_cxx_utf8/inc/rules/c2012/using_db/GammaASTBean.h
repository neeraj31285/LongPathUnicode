#pragma once

#include <vector>
#include <fstream>
#include <functional>
#include <unordered_map>
						

class GammaASTBean {

	using GETTER = std::function<const std::string&(GammaASTBean*)>;
	using SETTER = std::function<void(GammaASTBean*, const std::string&)>;

protected:

	struct Bind
	{
		SETTER m_setter;
		GETTER m_getter;
		const std::string m_propKey;

		Bind() = delete;
		Bind(const std::string& pKey);

		template<class _beanTy>
		Bind& getter(const std::string&(_beanTy::*pGetter)());

		template<class _beanTy>
		Bind& setter(void(_beanTy::*pSetter)(const std::string&));
	};

private:

	const std::vector<Bind> m_accessors;

public:

	GammaASTBean();
	GammaASTBean(const std::vector<Bind>& pAccessorMap);

	virtual void serialize(std::ofstream& pOutStream);
	virtual void deserialize(const std::string& pBeanStr);
};


template<class _beanTy>
inline GammaASTBean::Bind& GammaASTBean::Bind::setter(void(_beanTy::* pSetFptr)(const std::string &))
{
	m_setter = [=](GammaASTBean* pTarget, const std::string& pValueStr) {
		(static_cast<_beanTy*>(pTarget)->*pSetFptr)(pValueStr);
	};
	return *this;
}

template<class _beanTy>
inline GammaASTBean::Bind& GammaASTBean::Bind::getter(const std::string &(_beanTy::* pGetFptr)())
{
	m_getter = [=](GammaASTBean* pTarget)->const std::string& {
		return (static_cast<_beanTy*>(pTarget)->*pGetFptr)();
	};
	return *this;
}

#define PROPERTY(_propVar)								\
private : std::string _propVar;							\
public:													\
inline const std::string& get##_propVar() {				\
	return _propVar;									\
}														\
inline void set##_propVar(const std::string& pValStr){	\
	_propVar = pValStr;									\
}