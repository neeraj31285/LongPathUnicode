#include "CheckerUtilsг.h"
#include "Misra_2115_visitorങ.h"
#include "llvm/Support/Path.h"
#include <unordered_set>
#include <boost/algorithm/string/trim.hpp>

using namespace std;
using namespace clang;

std::unordered_set<std::string> restrited_functions = { "memcmp", "memcpy", "memmove"};
std::unordered_set<std::string> qualifiers = { "const", "restrict", "_Atomic", "volatile"};

Misra_2115Visitor::Misra_2115Visitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& Misra_2115Visitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool Misra_2115Visitor::VisitCallExpr(clang::CallExpr *pCallExpr)
{
	if( (pCallExpr->getDirectCallee() != nullptr) && (CheckerUtils::isInUserCode(pCallExpr->getBeginLoc(), m_astContext)) )
	{
		std::string funct_name = pCallExpr->getDirectCallee()->getNameInfo().getAsString();
		if((restrited_functions.find(funct_name) != restrited_functions.end()) && (pCallExpr->getArg(0) != nullptr) && (pCallExpr->getArg(0)->getExprStmt() != nullptr))
		{
			std::string arg1_type = pCallExpr->getArg(0)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType().getDesugaredType(m_astContext).getAsString();
			std::string arg2_type = pCallExpr->getArg(1)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType().getDesugaredType(m_astContext).getAsString();
			
			//checkLogger.LogInfo("\n\narg1_type =  " + arg1_type);
			//checkLogger.LogInfo("\n\narg2_type =  " + arg2_type);

			for (auto itr : qualifiers)
			{			
				if((arg1_type.find(itr) != std::string::npos))
				{
					arg1_type.erase(arg1_type.find(itr),itr.length());
					boost::algorithm::trim(arg1_type);
	   			}
	   			if(arg2_type.find(itr) != std::string::npos)
	   			{
	   				arg2_type.erase(arg2_type.find(itr),itr.length());
	   				boost::algorithm::trim(arg2_type);
	   			}
	   		}

	   		if(arg1_type != arg2_type)
	   		{
	   			m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
	   		}
   		}
	}
	return true;
}

