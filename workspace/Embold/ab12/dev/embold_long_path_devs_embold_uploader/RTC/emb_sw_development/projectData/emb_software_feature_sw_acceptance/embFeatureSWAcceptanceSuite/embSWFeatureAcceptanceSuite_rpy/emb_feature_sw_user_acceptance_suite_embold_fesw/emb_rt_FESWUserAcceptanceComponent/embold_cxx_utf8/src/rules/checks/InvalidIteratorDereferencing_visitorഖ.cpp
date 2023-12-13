#include "InvalidIteratorDereferencing_visitor.h"
#include "CheckerUtils.h"
#include "IteratorChecksUtility.h"

using namespace clang;

InvalidIteratorDereferencingVisitor::InvalidIteratorDereferencingVisitor(clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& InvalidIteratorDereferencingVisitor::getViolationSrcLocations()
{
	return m_violationsSrcLoc;
}

bool InvalidIteratorDereferencingVisitor::VisitVarDecl(clang::VarDecl* pVarDecl)
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
					m_iteratorAssignmentMap[pVarDecl] = objectDecl;
			}
		}
	}
	return true;
}

bool InvalidIteratorDereferencingVisitor::VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr)
{
	if (CheckerUtils::isInUserCode(pCXXOperatorCallExpr->getBeginLoc(), m_astContext))
	{
		Expr* lhsExpr = pCXXOperatorCallExpr->getArg(0)->IgnoreParenImpCasts();
		const std::string& lhsType = IteratorChecksUtility::GetType(pCXXOperatorCallExpr->getArg(0)->getType());

		if (IteratorChecksUtility::isIteratorType(lhsType) && isa<DeclRefExpr>(lhsExpr))
		{
			DeclRefExpr* iterator = dyn_cast<DeclRefExpr>(lhsExpr);

			if (pCXXOperatorCallExpr->isAssignmentOp())
			{
				Expr* rhsExpr = pCXXOperatorCallExpr->getArg(1)->IgnoreParenImpCasts();

				if (isa<CXXMemberCallExpr>(rhsExpr))
				{
					CXXMemberCallExpr* memberCallExpr = dyn_cast<CXXMemberCallExpr>(rhsExpr);
					const std::string& rhsType = IteratorChecksUtility::GetType(memberCallExpr->getObjectType());

					if (IteratorChecksUtility::isContainerType(rhsType))
					{
						Expr* objectExpr = memberCallExpr->getImplicitObjectArgument();
						ValueDecl* objectDecl = IteratorChecksUtility::GetObjectDecl(objectExpr);

						if (objectDecl)
							m_iteratorAssignmentMap[iterator->getDecl()] = objectDecl;
					}
				}
			}
			else if (pCXXOperatorCallExpr->getOperator() == clang::OverloadedOperatorKind::OO_ExclaimEqual)
			{
				Expr* rhsExpr = pCXXOperatorCallExpr->getArg(1)->IgnoreParenImpCasts();
				if (isa<CXXMemberCallExpr>(rhsExpr))
				{
					CXXMemberCallExpr* memberCallExpr = dyn_cast<CXXMemberCallExpr>(rhsExpr);
					const std::string& rhsType = IteratorChecksUtility::GetType(memberCallExpr->getObjectType());

					if (IteratorChecksUtility::isContainerType(rhsType))
					{
						Expr* objectExpr = memberCallExpr->getImplicitObjectArgument();
						ValueDecl* objectDecl = IteratorChecksUtility::GetObjectDecl(objectExpr);

						const std::string& memberFunction = memberCallExpr->getDirectCallee()->getNameAsString();
						if (memberFunction.find("end") != std::string::npos
							&& (m_iteratorAssignmentMap[iterator->getDecl()] == objectDecl))
						{
							m_boundsCheckedIterators[iterator->getDecl()] = m_sourceRange;
						}
					}
				}
			}
			else if (pCXXOperatorCallExpr->getOperator() == clang::OverloadedOperatorKind::OO_Star)
			{
				if (m_boundsCheckedIterators.find(iterator->getDecl()) != m_boundsCheckedIterators.end())
				{
					SourceRange sourceRange = m_boundsCheckedIterators[iterator->getDecl()];
					if (sourceRange.isValid() && sourceRange.getEnd() < pCXXOperatorCallExpr->getBeginLoc())
					{
						m_boundsCheckedIterators.erase(iterator->getDecl());
						m_violationsSrcLoc.emplace_back(pCXXOperatorCallExpr->getBeginLoc());
					}
				}
				else
					m_violationsSrcLoc.emplace_back(pCXXOperatorCallExpr->getBeginLoc());
			}
		}
	}
	return true;
}

bool InvalidIteratorDereferencingVisitor::VisitIfStmt(IfStmt* ifStmt)
{
	if (CheckerUtils::isInUserCode(ifStmt->getBeginLoc(), m_astContext))
		m_sourceRange = ifStmt->getSourceRange();
	return true;
}

bool InvalidIteratorDereferencingVisitor::VisitWhileStmt(WhileStmt* whileStmt)
{
	if (CheckerUtils::isInUserCode(whileStmt->getBeginLoc(), m_astContext))
		m_sourceRange = whileStmt->getSourceRange();
	return true;
}

bool InvalidIteratorDereferencingVisitor::VisitForStmt(ForStmt* forStmt)
{
	if (CheckerUtils::isInUserCode(forStmt->getBeginLoc(), m_astContext))
		m_sourceRange = forStmt->getSourceRange();
	return true;
}

bool InvalidIteratorDereferencingVisitor::VisitFunctionDecl(FunctionDecl* pFuncDecl)
{
	if (CheckerUtils::isInUserCode(pFuncDecl) && pFuncDecl->isThisDeclarationADefinition())
	{
		m_boundsCheckedIterators.clear();
		m_iteratorAssignmentMap.clear();
	}
	return true;
}
