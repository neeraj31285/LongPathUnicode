
#include "CheckerUtils.h"
#include "UnconditionalPtrDeref_visitor.h"


using namespace std;
using namespace clang;




UnconditionalPtrDerefVisitor::UnconditionalPtrDerefVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}


const std::vector<clang::SourceLocation>& UnconditionalPtrDerefVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


const std::vector<std::pair<std::string, std::string>>& UnconditionalPtrDerefVisitor::getFuncCallsWithReturnedPtr()
{
	return m_funcCallsWithReturnPtr;
}


const std::unordered_map<std::string, std::unordered_map<std::string, clang::SourceLocation>>& UnconditionalPtrDerefVisitor::getUnconditionalDerefFuncArgs()
{
	return m_unconditionalDrefFuncArgs;
}


bool UnconditionalPtrDerefVisitor::VisitFunctionDecl(FunctionDecl * D)
{
	if (!CheckerUtils::isInUserCode(D) || !(D->isThisDeclarationADefinition())) {
		return true;
	}
	m_currVisitingFuncName = D->getNameAsString();

	for (auto it = m_unconditionalDrefFuncArgs.cbegin(); it != m_unconditionalDrefFuncArgs.cend();)
	{
		(it->second.empty() ? m_unconditionalDrefFuncArgs.erase(it++) : (++it));
	}

	m_unconditionalDrefFuncArgs.emplace(make_pair(m_currVisitingFuncName, unordered_map<string, SourceLocation>()));
	m_ptrAssignedFrmCallee.clear();
	m_nullCheckedReturns.clear();
	return true;
}



bool UnconditionalPtrDerefVisitor::isPointerNullCheckedInIfStmt(const IfStmt* pIfStmt, ValueDecl* pPtrValDecl)
{
	Expr* declCondExpr = getNullPtrCheckdDeclRefExprFrom(pIfStmt);
	Expr* condition = const_cast<IfStmt*>(pIfStmt)->getCond();
	if (declCondExpr && isa<DeclRefExpr>(declCondExpr)) {
		ValueDecl* condValDecl = dyn_cast<DeclRefExpr>(declCondExpr)->getDecl();
		return (condValDecl == pPtrValDecl);
	}
	return false;
}


bool UnconditionalPtrDerefVisitor::VisitVarDecl(VarDecl * pVarDecl)
{
	if (!CheckerUtils::isInUserCode(pVarDecl) || !pVarDecl->getType()->isPointerType()) {
		return true;
	}

	auto iExpr = pVarDecl->getAnyInitializer();
	if(iExpr == nullptr){
		return true;
	}

	auto initExpr = iExpr->IgnoreImplicit();
	if (initExpr != nullptr) {

		if (initExpr->getStmtClass() == Expr::StmtClass::CallExprClass) {
			m_ptrAssignedFrmCallee.emplace(make_pair(pVarDecl->getNameAsString(), pVarDecl));
		}
	}
	return true;
}


bool UnconditionalPtrDerefVisitor::VisitReturnStmt(clang::ReturnStmt * pRetStmt)
{
	if (!CheckerUtils::isInUserCode(pRetStmt->getBeginLoc(), m_astContext)) {
		return true;
	}

	bool ifConditionFound = false;
	auto rootParent = getRootParentCheckIfStmt(const_cast<ASTContext&>(m_astContext).getParents(*pRetStmt), ifConditionFound);
	if (ifConditionFound) {
		const IfStmt* ifStatement = rootParent[0].get<IfStmt>();
		if (ifStatement) {
			Expr* declCondExpr = getNullPtrCheckdDeclRefExprFrom(ifStatement);
			if (declCondExpr && isa<DeclRefExpr>(declCondExpr)) {
				ValueDecl* condValDecl = dyn_cast<DeclRefExpr>(declCondExpr)->getDecl();
				m_nullCheckedReturns.insert(condValDecl->getNameAsString());
			}
		}
	}
	return true;
}


clang::DynTypedNodeList UnconditionalPtrDerefVisitor::getRootParentCheckIfStmt(clang::DynTypedNodeList pCurParent, bool& pIsIfStmtFound)
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


void UnconditionalPtrDerefVisitor::updateUnconditionalDrefFuncArgsMap(const DynTypedNodeList& pRootParent, ValueDecl* pValDecl,
																	  const clang::SourceLocation& pDrefOpLoc, bool pIsIfCondFound)
{
	if (isa<ParmVarDecl>(pValDecl)) {

		if (m_nullCheckedReturns.find(pValDecl->getNameAsString()) != m_nullCheckedReturns.end()) {
			return;
		}

		if (pIsIfCondFound) {
			const IfStmt* ifStatement = pRootParent[0].get<IfStmt>();
			if (ifStatement) {
				bool isPtrNullChecked = isPointerNullCheckedInIfStmt(ifStatement, pValDecl);
				if (!isPtrNullChecked) {
					const auto& itr = m_unconditionalDrefFuncArgs.find(m_currVisitingFuncName);
					if (itr != m_unconditionalDrefFuncArgs.end()) {
						auto& uncheckedNullPtrDrefsMap = itr->second;
						uncheckedNullPtrDrefsMap.emplace(pValDecl->getNameAsString(), pDrefOpLoc);
					}
				}
			}
		}
		else {
			const auto& itr = m_unconditionalDrefFuncArgs.find(m_currVisitingFuncName);
			if (itr != m_unconditionalDrefFuncArgs.end()) {
				auto& uncheckedNullPtrDrefsMap = itr->second;
				uncheckedNullPtrDrefsMap.emplace(pValDecl->getNameAsString(), pDrefOpLoc);
			}
		}
	}
}


clang::Expr* UnconditionalPtrDerefVisitor::getNullPtrCheckdDeclRefExprFrom(const clang::IfStmt * pIfStmt)
{
	Expr* condition = const_cast<IfStmt*>(pIfStmt)->getCond();
	if (condition->getStmtClass() == Expr::StmtClass::BinaryOperatorClass)
	{
		const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(condition);

		if (binaryOperator != nullptr && binaryOperator->isComparisonOp()) {

			Expr* lhsExpr = binaryOperator->getLHS()->IgnoreParenImpCasts();
			Expr* rhsExpr = binaryOperator->getRHS()->IgnoreParenImpCasts();
			Expr* declCondExpr = isa<CXXNullPtrLiteralExpr>(rhsExpr) ? (lhsExpr) : (isa<CXXNullPtrLiteralExpr>(lhsExpr) ? rhsExpr : nullptr);
			if (declCondExpr) {
				return declCondExpr;
			}
			return (isa<DeclRefExpr>(rhsExpr) ? (rhsExpr) : (isa<DeclRefExpr>(lhsExpr) ? lhsExpr : nullptr));
		}
	}
	else if (condition->getStmtClass() == Expr::StmtClass::UnaryOperatorClass)
	{
		const UnaryOperator *unaryOperator = dyn_cast<UnaryOperator>(condition);
		const UnaryOperator::Opcode opCode = unaryOperator->getOpcode();
		if (opCode == UnaryOperator::Opcode::UO_Not || opCode == UnaryOperator::Opcode::UO_LNot) {
			ImplicitCastExpr* castExpr = dyn_cast<ImplicitCastExpr>(unaryOperator->getSubExpr());
			if (castExpr && castExpr->getCastKind() == CastKind::CK_PointerToBoolean) {
				return unaryOperator->getSubExpr()->IgnoreParenImpCasts();
			}
		}
	}
	else if (isa<ImplicitCastExpr>(condition)) {
		ImplicitCastExpr* castExpr = dyn_cast<ImplicitCastExpr>(condition);
		if (castExpr && castExpr->getCastKind() == CastKind::CK_PointerToBoolean) {
			return condition->IgnoreParenImpCasts();
		}
	}
	return nullptr;
}


bool UnconditionalPtrDerefVisitor::VisitCallExpr(CallExpr * pCallExpr)
{
	if (!CheckerUtils::isInUserCode(pCallExpr->getBeginLoc(), m_astContext)) {
		return true;
	}

	const auto calleDecl = pCallExpr->getCalleeDecl();

	if (!calleDecl) {
		return true;
	}

	const auto funcDecl = calleDecl->getAsFunction();

	if (!funcDecl) {
		return true;
	}

	const auto& funcName = funcDecl->getNameAsString();

	int argIndex = 0;
	for (auto prmExpr : pCallExpr->arguments()) {
		Expr* argExpr = prmExpr->IgnoreParenImpCasts();
		if (isa<DeclRefExpr>(argExpr)) {
			ValueDecl* argValD = dyn_cast<DeclRefExpr>(argExpr)->getDecl();

			bool ifConditionFound = false;
			auto rootParent = getRootParentCheckIfStmt(const_cast<ASTContext&>(m_astContext).getParents(*pCallExpr), ifConditionFound);
			const bool isPtrAssignedFrmCallee = (m_ptrAssignedFrmCallee.find(argValD->getNameAsString()) != m_ptrAssignedFrmCallee.end());

			if (isPtrAssignedFrmCallee) {
				if (ifConditionFound) {
					const IfStmt* ifStatement = rootParent[0].get<IfStmt>();
					if (ifStatement) {
						bool isPtrNullChecked = isPointerNullCheckedInIfStmt(ifStatement, argValD);
						if (!isPtrNullChecked && argIndex < funcDecl->getNumParams()) {
							m_funcCallsWithReturnPtr.push_back(make_pair(funcName, funcDecl->getParamDecl(argIndex)->getNameAsString()));
						}
					}
				}
				else if (argIndex < funcDecl->getNumParams()) {
					m_funcCallsWithReturnPtr.push_back(make_pair(funcName, funcDecl->getParamDecl(argIndex)->getNameAsString()));
				}
			}
		}
		argIndex++;
	}
	return true;
}



bool UnconditionalPtrDerefVisitor::VisitUnaryOperator(UnaryOperator * pUnOpr)
{
	if (!CheckerUtils::isInUserCode(pUnOpr->getExprLoc(), m_astContext) || pUnOpr->getOpcode() != UnaryOperator::Opcode::UO_Deref) {
		return true;
	}

	Expr* expr = pUnOpr->getSubExpr()->IgnoreParenImpCasts();
	if (!isa<DeclRefExpr>(expr)) {
		return true;
	}

	bool ifConditionFound = false;
	ValueDecl* ptrVarDecl = dyn_cast<DeclRefExpr>(expr)->getDecl();

	auto rootParent = getRootParentCheckIfStmt(const_cast<ASTContext&>(m_astContext).getParents(*pUnOpr), ifConditionFound);

	updateUnconditionalDrefFuncArgsMap(rootParent, ptrVarDecl, pUnOpr->getBeginLoc(), ifConditionFound);

	const bool isPtrAssignedFrmCallee = (m_ptrAssignedFrmCallee.find(ptrVarDecl->getNameAsString()) != m_ptrAssignedFrmCallee.end());
	if (isPtrAssignedFrmCallee) {

		if (ifConditionFound) {
			const IfStmt* ifStatement = rootParent[0].get<IfStmt>();
			if (ifStatement) {
				bool isPtrNullChecked = isPointerNullCheckedInIfStmt(ifStatement, ptrVarDecl);
				if (!isPtrNullChecked) {
					m_voilationsSrcLoc.push_back(pUnOpr->getBeginLoc());
				}
			}
		}
		else {

			m_voilationsSrcLoc.push_back(pUnOpr->getBeginLoc());
		}
	}

	return true;
}



bool UnconditionalPtrDerefVisitor::VisitBinaryOperator(BinaryOperator * pBinOpr)
{
	if (pBinOpr == nullptr || !CheckerUtils::isInUserCode(pBinOpr->getOperatorLoc(), m_astContext)) {
		return true;
	}

	const auto& opcode = pBinOpr->getOpcode();
	if (opcode == BinaryOperator::Opcode::BO_Assign) {

		Expr* exprL = pBinOpr->getLHS()->IgnoreImplicit();

		if (isa<DeclRefExpr>(exprL)) {

			DeclRefExpr *declRefL = dyn_cast<DeclRefExpr>(exprL);
			ValueDecl* valDecl = declRefL->getDecl();

			if (valDecl->getType()->isPointerType()) {

				Expr* exprR = pBinOpr->getRHS()->IgnoreImplicit();
				if (isa<CallExpr>(exprR)) {

					const Type *exprType = exprR->getType().getTypePtrOrNull();
					if (exprType != nullptr)
					{
						CallExpr *callExpr = dyn_cast<CallExpr>(exprR);
						FunctionDecl *func = callExpr->getDirectCallee();

						if (func != nullptr) {
							if (func->getReturnType()->isPointerType()) {
								m_ptrAssignedFrmCallee.insert(make_pair(valDecl->getNameAsString(), valDecl));
							}
						}
					}					
				}
			}
		}
	}
	return true;
}
