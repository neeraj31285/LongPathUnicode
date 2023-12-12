#include "CheckerUtilsг.h"
#include "NonPODMemOperation_visitorघ.h"
#include<unordered_set>
#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

unordered_set<string> memOperation={"memset", "bzero", "memcpy", "memcpy_s", "memset_s", "memcmp", "memmove", "memmove_s", "memchr"};

NonPODMemOperationVisitor::NonPODMemOperationVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& NonPODMemOperationVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool NonPODMemOperationVisitor::isInUserCode(const clang::SourceLocation &loc) {

		if (loc.isValid()){
			clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
			const clang::SourceManager &SM = m_astCon.getSourceManager();
			return !(SM.isInSystemHeader(loc) || SM.isInSystemMacro(loc));
		}
		return false;
	}

bool NonPODMemOperationVisitor::VisitCallExpr(clang::CallExpr * pCallExpr){
	if ((pCallExpr != nullptr) && (pCallExpr->getSourceRange().isValid())
			&& (pCallExpr->getType().getTypePtrOrNull() != nullptr) && (pCallExpr->getDirectCallee() != nullptr) 
			&& (isInUserCode(pCallExpr->getBeginLoc()))
			)
	{
		if(memOperation.find( pCallExpr->getDirectCallee()->getNameInfo().getAsString()) != memOperation.end())
		{
			for (auto argument : pCallExpr->arguments())
      {
      	if((argument->IgnoreImplicit()->getType().getTypePtrOrNull()) && (argument->IgnoreImplicit()->getType()->getPointeeCXXRecordDecl()))
				{
					if(argument->IgnoreImplicit()->getType()->getPointeeCXXRecordDecl()->isPOD() != 1)
					{
						m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
					}
				}    	
      }			
		}
	}
	return true;
}
