#pragma once
#include "logger.h"
#include "GammaRuleCheckeró.h"
#include "EmboldTidyModuleв.h"

#if defined(_WIN32) || defined(_WIN64)
//#include <filesystem> 
//namespace fs = std::filesystem;
//switching back to boost::filesystem for now since std::filesytem has bugs, surfaced at bosch.
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#else
//Replace this when filesystem is accepted in C++1y
//	with std C++ header and namespace name. (GCC 5.2?, clang++??)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

static const char* const EXT = ".bean";

template<class TyVisitor, class TyBean>
std::unordered_map<std::string, std::vector<TyBean>> GammaRuleChecker<TyVisitor, TyBean>::m_tuAstBeansMap;


template<class TyVisitor, class TyBean>
inline std::unordered_map<std::string, std::vector<TyBean>>& GammaRuleChecker<TyVisitor, TyBean>::getTUAstBeanMap() {
	return m_tuAstBeansMap;
}


template<class TyVisitor, class TyBean>
inline const std::string GammaRuleChecker<TyVisitor, TyBean>::extractFileName(const std::string& pFilePath)
{
	short startPos = -1;
	const short endPos = pFilePath.size() - std::string(EXT).size() - 1;
	for (short index = endPos; index > 0; index--) {
		const char ch = pFilePath.at(index);
		if ( ch == '/' || ch == '\\') {
			startPos = index + 1;
			break;
		}
	}
	const std::string rawName = pFilePath.substr(startPos, endPos - startPos + 1);
	const std::size_t pos = rawName.find_first_of("_");
	return rawName.substr(pos + 1, rawName.size() - pos);
}

template <class TyVisitor, class TyBean>
inline void GammaRuleChecker<TyVisitor, TyBean>::setCheckerName(const std::string &pCheckerName)
{
	static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);
	m_checkerName = pCheckerName;
}

template <class TyVisitor, class TyBean>
void GammaRuleChecker<TyVisitor, TyBean>::HandleTranslationUnit(clang::ASTContext &Context)
{
	static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);
	logger.LogInfo("[emboldcxx::check] " + m_checkerName);

	TyVisitor astVisitor(Context);
	astVisitor.TraverseTranslationUnitDecl(Context.getTranslationUnitDecl());

	const auto& SM = Context.getSourceManager();
	const std::string& filePath = SM.getFileEntryForID(SM.getMainFileID())->getName().str();
	const std::string& fileName = llvm::sys::path::filename(filePath).str();

	std::vector<TyBean> beansVector;
	onEndOfTranslationUnit(Context, astVisitor, beansVector);
	m_tuAstBeansMap.emplace(std::make_pair(fileName, beansVector));
}


template<class TyVisitor, class TyBean>
inline void GammaRuleChecker<TyVisitor, TyBean>::dumpAstBeans(const std::string& pRuleName)
{
	// const fs::path& tempDir = fs::temp_directory_path() / pRuleName;

	// for (auto pairItr : m_tuAstBeansMap) {
	// 	const std::string& threadId = ("/" + std::to_string(CommandLineOptions::_thread_id) + "_");
	// 	const std::string& astFileName = tempDir.string() + (threadId + pairItr.first + EXT);
	// 	auto& astBeans = pairItr.second;
	// 	const std::size_t size = astBeans.size();
	// 	std::ofstream outStream(astFileName, std::ios::trunc);
	// 	outStream << "[\n";
	// 	for (int index = 0; index < size; ++index)
	// 	{
	// 		astBeans[index].serialize(outStream);
	// 		if (index < size - 1) {
	// 			outStream << ",\n";
	// 		}
	// 	}
	// 	outStream << "\n]";
	// 	outStream.close();
	// }
	// m_tuAstBeansMap.clear();
}


template<class TyVisitor, class TyBean>
inline void GammaRuleChecker<TyVisitor, TyBean>::loadAstBeans(const std::string& pRuleName)
{
	fs::directory_iterator end_itr;
	fs::path tmp_path = fs::temp_directory_path() / pRuleName;

	for (fs::directory_iterator itr(tmp_path); itr != end_itr; ++itr)
	{
		const std::string& astFile = itr->path().string();
		
		const std::string& fileName = extractFileName(astFile);
		auto& astBeans = m_tuAstBeansMap.emplace(make_pair(fileName, std::vector<TyBean>())).first->second;

		std::ifstream readStream(astFile, std::ios::in);
		if (readStream.fail()) {
			break;
		}
		while (!readStream.eof())
		{
			std::string streamline;
			std::getline(readStream, streamline);
			if (streamline.empty() || streamline.at(0) == '[' || streamline.at(0) == ']') {
				continue;
			}
			astBeans.push_back(TyBean());
			astBeans.back().deserialize(streamline);
		}
	}
}
