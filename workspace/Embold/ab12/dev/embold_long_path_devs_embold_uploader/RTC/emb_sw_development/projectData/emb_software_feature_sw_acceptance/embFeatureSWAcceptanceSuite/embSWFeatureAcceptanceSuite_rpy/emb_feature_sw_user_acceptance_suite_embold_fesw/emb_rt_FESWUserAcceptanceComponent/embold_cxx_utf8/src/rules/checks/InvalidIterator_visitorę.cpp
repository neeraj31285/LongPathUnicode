#include "CheckerUtils.h"
#include "IteratorChecksUtility.h"
#include "InvalidIterator_visitor.h"

using namespace clang;

InvalidIteratorVisitor::InvalidIteratorVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& InvalidIteratorVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool InvalidIteratorVisitor::VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* pCXXMemberCallExpr)
{
	if (CheckerUtils::isInUserCode(pCXXMemberCallExpr->getBeginLoc(), m_astContext)
		&& pCXXMemberCallExpr->getNumArgs() > 0)
	{
		const std::string& objectType = IteratorChecksUtility::GetType(pCXXMemberCallExpr->getObjectType());
		Expr* objectExpr = pCXXMemberCallExpr->getImplicitObjectArgument();

		if (IteratorChecksUtility::isContainerType(objectType))
		{
			ValueDecl* objectDecl = IteratorChecksUtility::GetObjectDecl(objectExpr);

			for (Expr* arg : pCXXMemberCallExpr->arguments())
			{
				Expr* argumentExpr = IteratorChecksUtility::GetCXXMemberCallExprArgument(arg);
				if (isa<DeclRefExpr>(argumentExpr))
				{
					DeclRefExpr* argument = dyn_cast<DeclRefExpr>(argumentExpr);
					const std::string& argumentType = IteratorChecksUtility::GetType(argument->getType());

					if (IteratorChecksUtility::isIteratorType(argumentType))
					{
						for (auto iteratorAssignment : m_iteratorAssignmentMap)
						{
							if (argument->getDecl() == iteratorAssignment.first)
							{
								if (objectDecl != iteratorAssignment.second)
									m_voilationsSrcLoc.emplace_back(pCXXMemberCallExpr->getBeginLoc());

								break;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool InvalidIteratorVisitor::VisitVarDecl(clang::VarDecl* pVarDecl)
{
	if (CheckerUtils::isInUserCode(pVarDecl) && pVarDecl->isLocalVarDecl())
	{
		const std::string& varDeclType = IteratorChecksUtility::GetType(pVarDecl->getType());

		if (IteratorChecksUtility::isIteratorType(varDeclType)
			&& pVarDecl->getInit()
			&& (pVarDecl->getInitStyle() == clang::VarDecl::InitializationStyle::CInit))
		{
			Expr* expr = IteratorChecksUtility::GetCXXMemberCallExprArgument(pVarDecl->getInit()->IgnoreParenImpCasts());
			if (isa<CXXMemberCallExpr>(expr))
			{
				CXXMemberCallExpr* cxxMemberCallExpr = dyn_cast<CXXMemberCallExpr>(expr);
				Expr* objectExpr = cxxMemberCallExpr->getImplicitObjectArgument();
				ValueDecl* objectDecl = IteratorChecksUtility::GetObjectDecl(objectExpr);
				const std::string& objectType = IteratorChecksUtility::GetType(cxxMemberCallExpr->getObjectType());

				if (IteratorChecksUtility::isContainerType(objectType) && objectDecl != nullptr)
				{
					m_iteratorAssignmentMap[pVarDecl] = objectDecl;
				}
			}
		}
	}
	return true;
}

bool InvalidIteratorVisitor::VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr)
{
	if (CheckerUtils::isInUserCode(pCXXOperatorCallExpr->getBeginLoc(), m_astContext)
		&& pCXXOperatorCallExpr->isAssignmentOp())
	{
		const std::string& lhsType = IteratorChecksUtility::GetType(pCXXOperatorCallExpr->getArg(0)->getType());
		Expr* lhsExpr = pCXXOperatorCallExpr->getArg(0);
		Expr* rhsExpr = pCXXOperatorCallExpr->getArg(1)->IgnoreParenImpCasts();

		if (IteratorChecksUtility::isIteratorType(lhsType) && isa<CXXMemberCallExpr>(rhsExpr))
		{
			CXXMemberCallExpr* memberCallExpr = dyn_cast<CXXMemberCallExpr>(rhsExpr);
			const std::string& rhsType = IteratorChecksUtility::GetType(memberCallExpr->getObjectType());

			if (IteratorChecksUtility::isContainerType(rhsType))
			{
				Expr* objectExpr = memberCallExpr->getImplicitObjectArgument();
				ValueDecl* objectDecl = IteratorChecksUtility::GetObjectDecl(objectExpr);

				if (isa<DeclRefExpr>(lhsExpr) && objectDecl != nullptr)
				{
					DeclRefExpr* iterator = dyn_cast<DeclRefExpr>(lhsExpr);
					m_iteratorAssignmentMap[iterator->getDecl()] = objectDecl;
				}
			}
		}
	}
	return true;
}

bool InvalidIteratorVisitor::VisitFunctionDecl(clang::FunctionDecl* pFuncDecl)
{
	if (CheckerUtils::isInUserCode(pFuncDecl) && pFuncDecl->isThisDeclarationADefinition())
	{
		m_iteratorAssignmentMap.clear();
	}
	return true;
}