#include "CheckerUtilsг.h"
#include "UnvalidatedMemAllocation_visitorå.h"
#include<unordered_set>

using namespace std;
using namespace clang;

string ifCondVar="",memAllocVar = "", inputVar = "";
static const Stmt *ifStmtObj;
static const CallExpr *memAllocCall;
bool isViolation = false;
static unordered_set<string> inputFunctions={"__isoc99_scanf", "scanf"};
static unordered_set<string> memAllocFunctions = {"malloc", "calloc", "realloc"};

UnvalidatedMemAllocVisitor::UnvalidatedMemAllocVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& UnvalidatedMemAllocVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool UnvalidatedMemAllocVisitor::isInUserCode(const clang::SourceLocation &loc)
{
	if (loc.isValid()){
		clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
		const clang::SourceManager &SM = m_astCon.getSourceManager();
		return !(SM.isInSystemHeader(loc) || SM.isInSystemMacro(loc));
		}
	return false;
}

bool hasChildIfStmt1(const Stmt *S)
{
	if((S != nullptr) && (S->getSourceRange().isValid()) && (S->child_begin()!=S->child_end()))
	{
		for (const Stmt *child : S->children())
		{
			if((child != nullptr) && (S->getSourceRange().isValid()))
			{
				if(clang::isa<clang::IfStmt>(child))
				{
					const IfStmt *ifstmt = dyn_cast<IfStmt>(child);
					if((ifstmt != nullptr) && (ifstmt->getCond() != nullptr))
					{
						if(ifstmt->getCond()->getStmtClass() == Expr::StmtClass::BinaryOperatorClass)
						{
							const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(ifstmt->getCond());
							if((binaryOperator != nullptr) && (binaryOperator->getLHS()))
							{
								if(binaryOperator->getLHS()->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
								{
									const DeclRefExpr *declRefex = dyn_cast<DeclRefExpr>(binaryOperator->getLHS()->IgnoreImpCasts());
									if((declRefex != nullptr) && (declRefex->getDecl() !=nullptr))
									{
										ifCondVar = declRefex->getDecl()->getNameAsString();
										ifStmtObj = ifstmt;
									}    
								}
							}
						}
					}
					return 1;
				}
				else
				{
					if((child != nullptr) && (child->getSourceRange().isValid()))
					{
						const bool returnval = hasChildIfStmt1(child);
						if(returnval == 1)
						{
							return returnval;
						}
					}				
				}
			}
		}
	}
	return false;
}

bool UnvalidatedMemAllocVisitor::hasParentIfStmt(const Stmt * S)
{
	clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
	const auto& parents = m_astCon.getParents(*S);
	const Stmt* ST = parents[0].get<Stmt>();
	if(ST != nullptr)
	{
	  	if(isa<IfStmt>(ST))
	    {    	
	    	const IfStmt *ifstmt = dyn_cast<IfStmt>(ST);
		   	if((ifstmt != nullptr) && (ifstmt->getCond() != nullptr))
		  	{
		  		if(ifstmt->getCond()->getStmtClass() == Expr::StmtClass::BinaryOperatorClass)
		       	{
		          	const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(ifstmt->getCond());
		            if((binaryOperator != nullptr) && (binaryOperator->getLHS()))
		            {
		                if(binaryOperator->getLHS()->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
		                {
					        const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(binaryOperator->getLHS()->IgnoreImpCasts());
					        if((declRef != nullptr) && (declRef->getDecl() !=nullptr))
					        {
					           	ifCondVar = declRef->getDecl()->getNameAsString();
					        }    
				        }
					}
				}
			}
			return true;
		}

		if(isa<BinaryOperator>(ST) || isa<CallExpr>(ST))
		{
			const bool returnval = hasParentIfStmt(ST);
			if(returnval == 1)
			{
				return returnval;
			}
		}

		if(isa<CompoundStmt>(ST))
		{
			const bool returnval = hasParentIfStmt(ST);
			if(returnval == 1)
			{
				return returnval;
			}
			if(hasChildIfStmt1(ST))
			{
				bool hasReturnStmt = false;
				for (const Stmt *child : ifStmtObj->children())
				{
					if((child != nullptr) && (ifStmtObj->getSourceRange().isValid()))
					{
						if(clang::isa<clang::CompoundStmt>(child))
						{
							for(const Stmt *child1 : child->children())
							{
								if(clang::isa<clang::ReturnStmt>(child1))
								{
									hasReturnStmt = true;
									return true;
								}
							}
						}
						if(clang::isa<clang::ReturnStmt>(child))
						{
							hasReturnStmt = true;
							return true;
						}
					}					
				}
				if(hasReturnStmt == false)
				{
					isViolation = true;
				}
			}
		}
		return false;
	}
	return false;
}



bool UnvalidatedMemAllocVisitor::VisitCallExpr(clang::CallExpr * pCallExpr)
{
	if((pCallExpr != nullptr) && (pCallExpr->getDirectCallee() != nullptr))
	{
		if(memAllocFunctions.find(pCallExpr->getDirectCallee()->getDeclName().getAsString()) != memAllocFunctions.end())
		{
			memAllocCall = pCallExpr;
			for(int argument = 0; argument < pCallExpr->getNumArgs(); argument++)
			{
				if(clang::isa<clang::DeclRefExpr>(pCallExpr->getArg(argument)->IgnoreImpCasts()))
				{
					const DeclRefExpr *declref = dyn_cast<DeclRefExpr>(pCallExpr->getArg(argument)->IgnoreImpCasts());
					if(declref != nullptr)
					{
						memAllocVar = declref->getDecl()->getNameAsString();
					}
				}
			}
		}
		if(inputFunctions.find(pCallExpr->getDirectCallee()->getDeclName().getAsString()) != inputFunctions.end())
		{	
			for (auto argument : pCallExpr->arguments())
      		{
      			if(clang::isa<clang::UnaryOperator>(argument))
      			{
      				const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(argument);
      				if((unaryOp != nullptr) && (unaryOp->getSubExpr()))
      				{	
	      				if(unaryOp->getSubExpr()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
	              		{
	                		const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(unaryOp->getSubExpr());
	                		if(declRef && (declRef->getDecl()))
	                		{
	                			inputVar = declRef->getDecl()->getNameAsString();
	                		}                		
	                	}
	                }	
      			}
      		}
		}

		if(memAllocVar == inputVar)
		{
			clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
			if(memAllocCall != nullptr)
			{
				const auto& parents = m_astCon.getParents(*memAllocCall);
				const Expr* ST = parents[0].get<Expr>();	

				if(ST != nullptr)
				{
					if((!hasParentIfStmt(ST)) && (!hasChildIfStmt1(ST)) && (!isViolation)) 
					{
						m_voilationsSrcLoc.emplace_back(memAllocCall->getBeginLoc());
					}

					if((hasParentIfStmt(ST)) && (ifCondVar != "") && (memAllocVar != ifCondVar))
					{
						m_voilationsSrcLoc.emplace_back(memAllocCall->getBeginLoc());
					}
				}

				if(isViolation == true)
				{
					m_voilationsSrcLoc.emplace_back(memAllocCall->getBeginLoc());
				}
			}
		}
	}
	isViolation = false;
	ifCondVar = "";
	return true;
}
