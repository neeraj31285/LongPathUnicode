#include "CheckerUtilsг.h"
#include "ShallowCopyConstruct_visitor나.h"

using namespace std;
using namespace clang;


ShallowCopyConstructVisitor::ShallowCopyConstructVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}


const std::vector<clang::SourceLocation>& ShallowCopyConstructVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool ShallowCopyConstructVisitor::VisitCXXRecordDecl(CXXRecordDecl* pCxxRecord)
{
	if (!CheckerUtils::isValidCxxRecord(pCxxRecord))
	{
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

				CXXConstructorDecl *copyCtor = nullptr;
				for (auto ctorItr = pCxxRecord->ctor_begin(); ctorItr != pCxxRecord->ctor_end(); ++ctorItr)
				{
					if (ctorItr->isCopyConstructor())
					{
						copyCtor = *ctorItr;
						Stmt *ctorBody = copyCtor->getBody();

						if (ctorBody == nullptr)
						{
							copyCtor = nullptr;
							break;
						}

						for (auto child_itr = ctorBody->child_begin(); child_itr != ctorBody->child_end(); child_itr++)
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
					}
				}

				if (copyCtor == nullptr)
				{ 
					m_voilationsSrcLoc.emplace_back(deleteOperator->getExprLoc());
				}
			}
		}
	}

	return true;
}
