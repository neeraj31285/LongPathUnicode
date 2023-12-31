#include "DestructorNotVirtual_checker.h"
#pragma warning(push,0)



#include "GammaRuleFactory.h"
#pragma warning(pop)

using namespace std;
using namespace clang;

static const char* const SEVERITY = "Low";
static const char* const RULE_KEY = "destructor not virtual";
static const char* const ISSUE_DESC = "If any class, in the inheritance hierarchy with virtual functions, implements the destructor, then the destructor should be declared as virtual in the top-most base class.";




ADDTO_GAMMA_RULE_FACTORY_CTOR(RULE_KEY, DestructorNotVirtualChecker)

void DestructorNotVirtualChecker::onEndOfTranslationUnit(clang::ASTContext& pContext, DestructorNotVirtualVisitor & pVisitor, std::vector<GammaASTBean>& pAstBeans)
{
	auto& SM = pContext.getSourceManager();
	auto& assignOprsDecl = pVisitor.getNonCompliantDestructors();
	for (const CXXDestructorDecl* decl : assignOprsDecl) {

		const string& fullPath = SM.getFilename(decl->getLocation()).str();
		const string& relPath = emboldCxx::EmboldTidyModule::getRelativePath(fullPath);
		const size_t lineNum = SM.getPresumedLineNumber(decl->getLocation());
		
	}
}
