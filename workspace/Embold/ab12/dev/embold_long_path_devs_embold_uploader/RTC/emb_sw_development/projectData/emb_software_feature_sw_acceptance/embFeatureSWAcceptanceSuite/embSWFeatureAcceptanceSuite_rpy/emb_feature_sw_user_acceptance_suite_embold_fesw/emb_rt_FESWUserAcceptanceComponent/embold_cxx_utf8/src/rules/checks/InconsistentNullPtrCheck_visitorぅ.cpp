
#include "CheckerUtils.h"
#include "InconsistentNullPtrCheck_visitor.h"

using namespace std;
using namespace clang;


InconsistentNullPtrCheckVisitor::InconsistentNullPtrCheckVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}


const std::vector<clang::SourceLocation>& InconsistentNullPtrCheckVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


bool InconsistentNullPtrCheckVisitor::VisitFunctionDecl(FunctionDecl * D)
{
	if (!CheckerUtils::isInUserCode(D) || !(D->isThisDeclarationADefinition())) {
		return true;
	}
	m_drefsWithoutNullCheck.clear();
	return true;
}


clang::DynTypedNodeList InconsistentNullPtrCheckVisitor::getRootParentCheckIfStmt(clang::DynTypedNodeList pCurParent, bool& pIsIfStmtFound)
{
	pIsIfStmtFound = false;
	while (!pCurParent.empty()) {
		const Stmt* statement = pCurParent[0].get<Stmt>();
		if (statement) {
			if (isa<IfStmt>(statement)) {
				pIsIfStmtFound = true;
				break;
			}
		}
		pCurParent = const_cast<ASTContext&>(m_astContext).getParents(pCurParent[0]);
	}
	return pCurParent;
}


bool InconsistentNullPtrCheckVisitor::isPointerNullChecked(const IfStmt* pStmt, ValueDecl* pValDecl)
{
	Expr* declExpr = nullptr;
	Expr* condition = const_cast<IfStmt*>(pStmt)->getCond();

	if (condition->getStmtClass() == Expr::StmtClass::BinaryOperatorClass)
	{
		const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(condition);

		if (binaryOperator != nullptr && binaryOperator->isComparisonOp()) {

			Expr* lhsExpr = binaryOperator->getLHS()->IgnoreParenImpCasts();
			Expr* rhsExpr = binaryOperator->getRHS()->IgnoreParenImpCasts();
			declExpr = isa<CXXNullPtrLiteralExpr>(rhsExpr) ? (lhsExpr) : (isa<CXXNullPtrLiteralExpr>(lhsExpr) ? rhsExpr : nullptr);
			if (!declExpr) {
				declExpr = isa<DeclRefExpr>(rhsExpr) ? (rhsExpr) : (isa<DeclRefExpr>(lhsExpr) ? lhsExpr : nullptr);
			}
		}
	}
	else if (isa<ImplicitCastExpr>(condition)) {
		ImplicitCastExpr* castExpr = dyn_cast<ImplicitCastExpr>(condition);
		if (castExpr->getCastKind() == CastKind::CK_PointerToBoolean) {
			declExpr = condition->IgnoreParenImpCasts();
		}
	}

	if (declExpr && isa<DeclRefExpr>(declExpr)) {
		ValueDecl* valDecl = dyn_cast<DeclRefExpr>(declExpr)->getDecl();
		if (valDecl == pValDecl) {
			auto itr = std::find(m_drefsWithoutNullCheck.begin(), m_drefsWithoutNullCheck.end(), pValDecl);
			if (itr != m_drefsWithoutNullCheck.end()) {
				m_voilationsSrcLoc.push_back(pStmt->getBeginLoc());
			}
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}


bool InconsistentNullPtrCheckVisitor::VisitUnaryOperator(UnaryOperator * pUnOpr)
{
	if (!CheckerUtils::isInUserCode(pUnOpr->getExprLoc(), m_astContext) || pUnOpr->getOpcode() != UnaryOperator::Opcode::UO_Deref) {
		return true;
	}

	ValueDecl* parmVarDecl = nullptr;
	Expr* expr = pUnOpr->getSubExpr()->IgnoreParenImpCasts();
	if (isa<DeclRefExpr>(expr)) {
		parmVarDecl = dyn_cast<DeclRefExpr>(expr)->getDecl();
		if (!isa<ParmVarDecl>(parmVarDecl)) {
			return true;
		}
	}
	else {
		return true;
	}

	bool ifConditionFound = false;
	bool isPtrNullChecked = false;
	auto rootParent = getRootParentCheckIfStmt(const_cast<ASTContext&>(m_astContext).getParents(*pUnOpr), ifConditionFound);

	if (ifConditionFound) {
		const IfStmt* ifStatement = rootParent[0].get<IfStmt>();
		if (ifStatement) {
			isPtrNullChecked = isPointerNullChecked(ifStatement, parmVarDecl);
		}
	}
	if(!isPtrNullChecked) {
		m_drefsWithoutNullCheck.push_back(parmVarDecl);
	}

	return true;
}
