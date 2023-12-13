#include "CheckerUtils.h"
#include "IncorrectPtrScale_visitor.h"

using namespace std;
using namespace clang;


IncorrectPtrScaleVisitor::IncorrectPtrScaleVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}


const std::vector<clang::SourceLocation>& IncorrectPtrScaleVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


void IncorrectPtrScaleVisitor::updateAssignmentTypeMap(const string& pVarStr, const QualType& pType)
{
	const auto& itr = m_assignedValTypes.find(pVarStr);
	if (itr == m_assignedValTypes.end()) {
		m_assignedValTypes.emplace(make_pair(pVarStr, pType.getUnqualifiedType()));
	}
	else {
		m_assignedValTypes.emplace(make_pair(pVarStr, itr->second));
	}
}


void IncorrectPtrScaleVisitor::updateAssignmentTypeMap(const string& pVarStr, DeclRefExpr* pDeclRef)
{
	const auto& varStrR = pDeclRef->getDecl()->getNameAsString();
	const auto& itr = m_assignedValTypes.find(varStrR);
	if (itr == m_assignedValTypes.end()) {
		m_assignedValTypes.emplace(make_pair(pVarStr, pDeclRef->getDecl()->getType().getUnqualifiedType()));
	}
	else {
		m_assignedValTypes.emplace(make_pair(pVarStr, itr->second));
	}
}


bool IncorrectPtrScaleVisitor::isIncorrectTypeBasedScaling(const ValueDecl* pValueDecl, const QualType& pOtherType)
{
	const auto& varType = pValueDecl->getType();
	if (varType->isPointerType() || varType->isArrayType()) {

		const auto& itr = m_assignedValTypes.find(pValueDecl->getNameAsString());
		if (itr != m_assignedValTypes.end())
		{
			const auto& otherType = (!pOtherType.isNull() && pOtherType->isPointerType()) ? pOtherType : varType;
			if (itr->second->isArrayType()) {

				const auto& pointedTypeStr = itr->second.getAsString();
				const auto& otherPointeeType = otherType->getPointeeType();
				if (!otherPointeeType.isNull()) {
					const auto& accessTypeStr = otherPointeeType.getUnqualifiedType().getAsString();
					return (pointedTypeStr.find(accessTypeStr) == string::npos);
				}
			}
		}
		else if(varType->isArrayType() && !pOtherType.isNull() && pOtherType->isPointerType()){

			auto varTypeStr = varType->getArrayElementTypeNoTypeQual()->getLocallyUnqualifiedSingleStepDesugaredType().getAsString();
			auto assignedTypeStr = pOtherType->getPointeeType().getUnqualifiedType().getAsString();
			return (assignedTypeStr.find(varTypeStr) == string::npos);
		}
	}
	return false;
}


bool IncorrectPtrScaleVisitor::VisitVarDecl(clang::VarDecl * pVarDecl)
{
	if (!CheckerUtils::isInUserCode(pVarDecl)) {
		return true;
	}

	auto iExpr = pVarDecl->getAnyInitializer();
	if(iExpr == nullptr){
		return true;
	}

	auto initExpr = iExpr->IgnoreImplicit();
	if (initExpr != nullptr ) {
		if (initExpr->getStmtClass() == Stmt::StmtClass::CStyleCastExprClass) {
			auto castExpr = dyn_cast<CStyleCastExpr>(initExpr);
			if (castExpr != nullptr){
				const Expr *expr = castExpr->getSubExprAsWritten()->IgnoreImplicit();
				if (expr->getStmtClass() == Expr::StmtClass::DeclRefExprClass) {
					auto declRef = dyn_cast<DeclRefExpr>(const_cast<Expr*>(expr));
					if (declRef != nullptr) {
						updateAssignmentTypeMap(pVarDecl->getNameAsString(), declRef);
					}
				}
				else if (expr->getStmtClass() == Expr::StmtClass::UnaryOperatorClass) {
					auto unOpr = dyn_cast<UnaryOperator>(const_cast<Expr*>(expr));
					if (unOpr != nullptr && unOpr->getOpcode() == UnaryOperator::Opcode::UO_AddrOf) {
						updateAssignmentTypeMap(pVarDecl->getNameAsString(), unOpr->getSubExpr()->getType());
					}
				}
			}
		}
		else if (initExpr->getStmtClass() == Expr::StmtClass::UnaryOperatorClass) {
			auto unOpr = dyn_cast<UnaryOperator>(const_cast<Expr*>(initExpr));
			if (unOpr != nullptr && unOpr->getOpcode() == UnaryOperator::Opcode::UO_AddrOf) {
				updateAssignmentTypeMap(pVarDecl->getNameAsString(), unOpr->getSubExpr()->getType());
			}
		}
		else if (initExpr->getStmtClass() == Stmt::StmtClass::DeclRefExprClass) {
			auto declRef = dyn_cast<DeclRefExpr>(const_cast<Expr*>(initExpr));
			updateAssignmentTypeMap(pVarDecl->getNameAsString(), declRef);
		}
	}
	return true;
}


void IncorrectPtrScaleVisitor::recordAssignmentOperation(clang::BinaryOperator * pBinOpr)
{
	DeclRefExpr *declRefL = dyn_cast<DeclRefExpr>(pBinOpr->getLHS()->IgnoreImplicit());

	if (declRefL != nullptr) {
		const auto& leftVar = declRefL->getDecl()->getNameAsString();
		DeclRefExpr *declRefR = dyn_cast<DeclRefExpr>(pBinOpr->getRHS()->IgnoreImplicit());
		if (declRefR != nullptr){
			updateAssignmentTypeMap(leftVar, declRefR);
		}
		else {
			CStyleCastExpr* castExprR = dyn_cast<CStyleCastExpr>(pBinOpr->getRHS()->IgnoreImplicit());
			if (castExprR != nullptr)
			{
				const Expr *expr = castExprR->getSubExprAsWritten()->IgnoreImplicit();
				if (expr->getStmtClass() == Expr::StmtClass::DeclRefExprClass) {
					DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(castExprR->getSubExprAsWritten());
					if (declRef != nullptr)
					{
						updateAssignmentTypeMap(leftVar, declRef);
					}
				}
			}
		}
	}
}


bool IncorrectPtrScaleVisitor::VisitBinaryOperator(BinaryOperator * pBinOpr)
{
	if (pBinOpr == nullptr || !CheckerUtils::isInUserCode(pBinOpr->getOperatorLoc(), m_astContext)) {
		return true;
	}

	const auto& opcode = pBinOpr->getOpcode();

	if (opcode == BinaryOperator::Opcode::BO_Assign) {
		recordAssignmentOperation(pBinOpr);
		return true;
	}

	if (opcode == BinaryOperator::Opcode::BO_Add || opcode == BinaryOperator::Opcode::BO_Sub)
	{
		CStyleCastExpr* castExprL = dyn_cast<CStyleCastExpr>(pBinOpr->getLHS()->IgnoreImplicit());
		CStyleCastExpr* castExprR = dyn_cast<CStyleCastExpr>(pBinOpr->getRHS()->IgnoreImplicit());

		if (castExprL != nullptr && castExprL->getType()->isPointerType() && castExprL->getCastKind() != clang::CastKind::CK_NoOp) {
			
			const Expr *expr = castExprL->getSubExprAsWritten()->IgnoreImplicit();
			if (expr != nullptr && expr->getStmtClass() == Expr::StmtClass::DeclRefExprClass){
				
				const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(const_cast<Expr *>(castExprL->getSubExprAsWritten()));
				if (declRef != nullptr && isIncorrectTypeBasedScaling(declRef->getDecl(), castExprL->getType())){
					m_voilationsSrcLoc.push_back(castExprL->getBeginLoc());
				}
			}
		}
		else if (castExprR != nullptr && castExprR->getType()->isPointerType() && castExprR->getCastKind() != clang::CastKind::CK_NoOp) {

			const Expr *expr = castExprR->getSubExprAsWritten()->IgnoreImplicit();
			if (expr != nullptr && expr->getStmtClass() == Expr::StmtClass::DeclRefExprClass) {

				const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(const_cast<Expr *>(castExprR->getSubExprAsWritten()));
				if (declRef != nullptr && isIncorrectTypeBasedScaling(declRef->getDecl(), castExprR->getType())) {
					m_voilationsSrcLoc.push_back(castExprR->getBeginLoc());
				}
			}
		}
		else{
			const DeclRefExpr *declRefL = dyn_cast<DeclRefExpr>(const_cast<Expr *>(pBinOpr->getLHS()->IgnoreImplicit()));
			const DeclRefExpr *declRefR = dyn_cast<DeclRefExpr>(const_cast<Expr *>(pBinOpr->getRHS()->IgnoreImplicit()));
			if (declRefL != nullptr && isIncorrectTypeBasedScaling(declRefL->getDecl(), QualType())){
				m_voilationsSrcLoc.push_back(declRefL->getBeginLoc());
			}
			else if (declRefR != nullptr && isIncorrectTypeBasedScaling(declRefR->getDecl(), QualType())) {
				m_voilationsSrcLoc.push_back(declRefR->getBeginLoc());
			}
		}
	}
	return true;
}

