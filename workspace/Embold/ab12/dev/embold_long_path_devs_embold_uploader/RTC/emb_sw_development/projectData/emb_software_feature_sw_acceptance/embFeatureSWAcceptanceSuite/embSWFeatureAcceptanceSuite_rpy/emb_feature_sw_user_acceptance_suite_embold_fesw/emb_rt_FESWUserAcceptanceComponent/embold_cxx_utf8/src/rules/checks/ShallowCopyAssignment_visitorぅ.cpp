#include "CheckerUtils.h"
#include "ShallowCopyAssignment_visitor.h"

using namespace std;
using namespace clang;

ShallowCopyAssignmentVisitor::ShallowCopyAssignmentVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& ShallowCopyAssignmentVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


bool ShallowCopyAssignmentVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!CheckerUtils::isValidCxxRecord(pCxxRecord)) {
		return true;
	}

	CXXDestructorDecl* dtor = pCxxRecord->getDestructor();
	if (dtor && pCxxRecord->hasUserDeclaredDestructor() && dtor->hasBody())
	{
		Stmt* dtorBody = pCxxRecord->getDestructor()->getBody();
		for (auto child_iter = dtorBody->child_begin(); child_iter != dtorBody->child_end(); child_iter++)
		{
			if (child_iter->getStmtClass() == Expr::StmtClass::CXXDeleteExprClass)
			{
				const CXXDeleteExpr *deleteOperator = dyn_cast<CXXDeleteExpr>(const_cast<Stmt *>(*child_iter));
				
				bool assignOprDefined = false;
				const bool isAssignOprDeclared = pCxxRecord->hasUserDeclaredCopyAssignment();

				if (isAssignOprDeclared)
				{
					for (const CXXMethodDecl *cxxMethod : pCxxRecord->methods())
					{
						if (cxxMethod->isCopyAssignmentOperator()) {

							Stmt* assignOprBody = cxxMethod->getBody();
							if (assignOprBody == nullptr) {
								break;
							}
							assignOprDefined = true;
							for (auto child_itr = assignOprBody->child_begin(); child_itr != assignOprBody->child_end(); child_itr++)
							{
								if (Expr::StmtClass::BinaryOperatorClass == child_itr->getStmtClass())
								{
									const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(const_cast<Stmt *>(*child_itr));
									if (binaryOperator != nullptr && binaryOperator->getOpcode() == BinaryOperator::Opcode::BO_Assign &&
										Expr::StmtClass::MemberExprClass == binaryOperator->getLHS()->getStmtClass()) {
										
										const MemberExpr *memExpr_LHS = dyn_cast<MemberExpr>(const_cast<Expr *>(binaryOperator->getLHS()));

										if (memExpr_LHS != nullptr && !(binaryOperator->getRHS()->getStmtClass() == Expr::StmtClass::CXXNewExprClass))
										{
											const bool isPointerType = memExpr_LHS->getMemberDecl()->getType()->isPointerType();
											if (isPointerType) {
												m_voilationsSrcLoc.emplace_back(binaryOperator->getOperatorLoc());
											}
										}
									}
								}
							}
							break;
						}
					}
				}

				if (!(isAssignOprDeclared && assignOprDefined)) {
					m_voilationsSrcLoc.emplace_back(deleteOperator->getExprLoc());
				}
			}
		}
	}

	return true;
}
