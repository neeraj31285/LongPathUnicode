#include "CheckerUtils.h"
#include "BitwiseOprandsMismatch_visitor.h"

using namespace std;
using namespace clang;


BitOprandsMismatchVisitor::BitOprandsMismatchVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}


const std::vector<clang::SourceLocation>& BitOprandsMismatchVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


bool BitOprandsMismatchVisitor::VisitBinaryOperator(BinaryOperator * pBinOpr)
{
	if (pBinOpr == nullptr || !CheckerUtils::isInUserCode(pBinOpr->getOperatorLoc(), m_astContext)) {
		return true;
	}

	const auto& opcode = pBinOpr->getOpcode();
	if (opcode == BinaryOperator::Opcode::BO_And || opcode == BinaryOperator::Opcode::BO_Or || opcode == BinaryOperator::Opcode::BO_Xor || 
		opcode == BinaryOperator::Opcode::BO_AndAssign || opcode == BinaryOperator::Opcode::BO_OrAssign || opcode == BinaryOperator::Opcode::BO_XorAssign)
	{
		int bitWidthL = 0;
		int bitWidthR = 0;

		DeclRefExpr *declRefL = nullptr;
		const CStyleCastExpr* castExprL = dyn_cast<CStyleCastExpr>(pBinOpr->getLHS()->IgnoreImplicit());
		if (castExprL != nullptr) {
			bitWidthL = m_astContext.getTypeSize(castExprL->getType());
		}
		else {
			declRefL = dyn_cast<DeclRefExpr>(const_cast<Expr *>(pBinOpr->getLHS()->IgnoreImplicit()));
		}

		DeclRefExpr *declRefR = nullptr;
		const CStyleCastExpr* castExprR = dyn_cast<CStyleCastExpr>(pBinOpr->getRHS()->IgnoreImplicit());
		if (castExprR != nullptr) {
			bitWidthR = m_astContext.getTypeSize(castExprR->getType());
		}
		else {
			declRefR = dyn_cast<DeclRefExpr>(const_cast<Expr *>(pBinOpr->getRHS()->IgnoreImplicit()));
		}

		if (declRefL != nullptr) {
			const string& name = declRefL->getDecl()->getNameAsString();
			const QualType& type = declRefL->getDecl()->getType();
			if (type.isPODType(m_astContext)) {
				bitWidthL = m_astContext.getTypeSize(type);
			}
		}

		if (declRefR != nullptr) {
			const string& name = declRefR->getDecl()->getNameAsString();
			const QualType& type = declRefR->getDecl()->getType();
			if (type.isPODType(m_astContext)) {
				bitWidthR = m_astContext.getTypeSize(type);
			}
		}

		if (bitWidthL == 64 && bitWidthR == 32) {
			m_voilationsSrcLoc.emplace_back(pBinOpr->getBeginLoc());
		}
	}

	return true;
}

