#include "CheckerUtils.h"
#include "Misra_2116_visitor.h"
#include "llvm/Support/Path.h"
#include<unordered_set>
//#include "clang/Lex/Lexer.h"
//

//using Acellere::Corona::CXX::Logger;
//Logger checkLogger(Logger::Category::Checker);

using namespace std;
using namespace clang;

Misra_2116Visitor::Misra_2116Visitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& Misra_2116Visitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool Misra_2116Visitor::VisitCallExpr(clang::CallExpr *pCallExpr)
{
	if( (pCallExpr->getDirectCallee() != nullptr) && (CheckerUtils::isInUserCode(pCallExpr->getBeginLoc(), m_astContext)) )
	{
		std::string funct_name = pCallExpr->getDirectCallee()->getNameInfo().getAsString();
		if(funct_name == "memcmp" && (pCallExpr->getArg(0) != nullptr) && (pCallExpr->getArg(0)->getExprStmt() != nullptr))
		{
			std::string arg1_type = pCallExpr->getArg(0)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType().getDesugaredType(m_astContext).getAsString();
			std::string arg2_type = pCallExpr->getArg(1)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType().getDesugaredType(m_astContext).getAsString();
			if(arg1_type.find("struct") != string::npos ||
				arg1_type.find("union") != string::npos ||
				arg1_type.find("float") != string::npos ||
				arg2_type.find("struct") != string::npos ||
				arg2_type.find("union") != string::npos ||
				arg2_type.find("float") != string::npos )
			{
   				m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
   			}

   			if(arg1_type.find("char") != string::npos || arg2_type.find("char") != string::npos )
   			{
   				if((pCallExpr->getArg(0)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType()->isArrayType()) || 
   				(pCallExpr->getArg(1)->getExprStmt()->IgnoreImplicit()->getType().getCanonicalType()->isArrayType()))
	   			{
	   				m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
	   			}
   			}
   		}
	}
	return true;
}

