#include "CheckerUtils.h"
#include "UnvalidatedParamAsIndex_visitor.h"

using namespace clang;

UnvalidatedParamAsIndexVisitor::UnvalidatedParamAsIndexVisitor(clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& UnvalidatedParamAsIndexVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool UnvalidatedParamAsIndexVisitor::VisitCallExpr(clang::CallExpr* callExpr)
{
	if (CheckerUtils::isInUserCode(callExpr->getExprLoc(), m_astContext)
		&& callExpr->getDirectCallee() != nullptr
		&& CheckerUtils::isInUserCode(callExpr->getDirectCallee())
		&& callExpr->getNumArgs() > 0
		)
	{
		if (m_unvalidatedParameters.find(callExpr->getDirectCallee()) != m_unvalidatedParameters.end())
		{
			unsigned int arg_index = m_unvalidatedParameters[callExpr->getDirectCallee()];
			Expr* argExpr = callExpr->getArg(arg_index);
			if (isa<DeclRefExpr>(argExpr->IgnoreImpCasts()))
			{
				DeclRefExpr* argDeclRef = dyn_cast<DeclRefExpr>(argExpr->IgnoreImpCasts());
				VarDecl* arg = dyn_cast<VarDecl>(argDeclRef->getDecl());

				if (arg && arg->isLocalVarDecl())
				{
					if (m_boundsCheckedVariables.find(arg) == m_boundsCheckedVariables.end())
						m_voilationsSrcLoc.emplace_back(callExpr->getBeginLoc());
				}
			}
		}
	}
	return true;
}

bool UnvalidatedParamAsIndexVisitor::VisitArraySubscriptExpr(clang::ArraySubscriptExpr* arraySubscriptExpr)
{
	if (CheckerUtils::isInUserCode(arraySubscriptExpr->getExprLoc(), m_astContext))
	{
		Expr* index = arraySubscriptExpr->getIdx()->IgnoreImpCasts();

		if (isa<DeclRefExpr>(index))
		{
			DeclRefExpr* indexVariable = dyn_cast<DeclRefExpr>(index);

			if (isa<ParmVarDecl>(indexVariable->getDecl()))
			{
				ParmVarDecl* param = dyn_cast<ParmVarDecl>(indexVariable->getDecl());
				//FunctionDecl* functionDecl = dyn_cast<FunctionDecl>(param->getParentFunctionOrMethod());

				if (m_parentFunction && (m_boundsCheckedParameters.find(indexVariable->getDecl()) == m_boundsCheckedParameters.end()))
					m_unvalidatedParameters[m_parentFunction] = param->getFunctionScopeIndex();
			}
		}
	}
	return true;
}

bool UnvalidatedParamAsIndexVisitor::VisitBinaryOperator(clang::BinaryOperator* binaryOperator)
{
	if (CheckerUtils::isInUserCode(binaryOperator->getExprLoc(), m_astContext) && binaryOperator->isComparisonOp())
	{
		Expr* lhsExpr = binaryOperator->getLHS()->IgnoreParenImpCasts();

		if (isa<DeclRefExpr>(lhsExpr))
		{
			DeclRefExpr* operandLHS = dyn_cast<DeclRefExpr>(lhsExpr);

			if (isa<ParmVarDecl>(operandLHS->getDecl()))
				m_boundsCheckedParameters.emplace(operandLHS->getDecl());
			else if (isa<VarDecl>(operandLHS->getDecl()))
				m_boundsCheckedVariables.emplace(operandLHS->getDecl());
		}
	}
	return true;
}

bool UnvalidatedParamAsIndexVisitor::VisitFunctionDecl(clang::FunctionDecl* functionDecl)
{
	if (CheckerUtils::isInUserCode(functionDecl) && functionDecl->hasBody())
	{
		m_parentFunction = functionDecl;
		m_boundsCheckedParameters.clear();
		m_boundsCheckedVariables.clear();
	}
	return true;
}