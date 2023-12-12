#include "ConstOverrideMismatch_checkerå.h"
#pragma warning(push,0)
#include "logger.h"

#include "CodeIssueWriter.hpp"
#include "GammaRuleFactoryก.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "medium";
static const char* const RULE_KEY = "const override mismatch";
static const char* const ISSUE_DESC = "Function override fails due to mismatch of const qualifiers in function declaration.";

extern CodeIssueWriter codeIssueWriter;
static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, ConstOverrideMismatchChecker)

bool ConstOverrideMismatchChecker::isOverrideDeclMismatch(const clang::CXXRecordDecl* pRecordDecl, const string& pMethodName, 
														  const bool pIsDrvdConst, ConstOverrideMismatchVisitor& pVisitor)
{
	for (auto baseItr = pRecordDecl->bases_begin(); baseItr != pRecordDecl->bases_end(); ++baseItr) {

		const CXXRecordDecl *cxxBaseRec = baseItr->getType()->getAsCXXRecordDecl();
		auto& recordMethodMap = pVisitor.getCxxRecordToMethodMap();
		auto itr = recordMethodMap.find(cxxBaseRec);

		if (itr != recordMethodMap.end()) {

			auto baseMethodsMap = itr->second;
			auto mthdItr = baseMethodsMap.find(pMethodName);

			if (mthdItr != baseMethodsMap.end()) {

				const CXXMethodDecl* baseMethod = mthdItr->second;
				if (baseMethod->isVirtual()) {
					const bool isBaseConst = baseMethod->isConst();
					return !((pIsDrvdConst && isBaseConst) || (!pIsDrvdConst && !isBaseConst));
				}
			}
		}
	}
	return false;
}

void ConstOverrideMismatchChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, ConstOverrideMismatchVisitor& pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();

	pVisitor.sortCxxRecordDecls();

	auto& recordMethodMap = pVisitor.getCxxRecordToMethodMap();
	for (auto cxxPair : recordMethodMap) {

		const CXXRecordDecl* cxxRecord = cxxPair.first;

		if (cxxRecord->getNumBases() == 0) continue;

		for (auto methodPair : cxxPair.second) {

			const string& methodName = methodPair.first;
			const CXXMethodDecl* methodDecl = methodPair.second;
			if (isOverrideDeclMismatch(cxxRecord, methodName, methodDecl->isConst(), pVisitor)) {
				const string& fullPath = SM.getFilename(methodDecl->getLocation()).str();
				const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
				const size_t lineNum = SM.getPresumedLineNumber(methodDecl->getLocation());
				codeIssueWriter.Write(relPath, ISSUE_DESC, RULE_KEY, SEVERITY, lineNum);
			}
		}
	}
}
