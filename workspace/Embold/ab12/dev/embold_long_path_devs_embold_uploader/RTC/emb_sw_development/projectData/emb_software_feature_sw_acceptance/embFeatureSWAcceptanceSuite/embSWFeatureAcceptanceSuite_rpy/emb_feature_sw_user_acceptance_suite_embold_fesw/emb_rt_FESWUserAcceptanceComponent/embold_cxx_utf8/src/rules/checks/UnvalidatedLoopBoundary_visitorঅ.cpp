#include "CheckerUtilsг.h"
#include "UnvalidatedLoopBoundary_visitor바.h"
#include<unordered_set>

using namespace std;
using namespace clang;

static string var1="",var2="",var3="";
static const Stmt *ifStmtObj;
static unordered_set<string> inputFunctions={"__isoc99_scanf", "scanf"};

UnvalidatedLoopBoundaryVisitor::UnvalidatedLoopBoundaryVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& UnvalidatedLoopBoundaryVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool UnvalidatedLoopBoundaryVisitor::isInUserCode(const clang::SourceLocation &loc)
{
	if (loc.isValid()){
		clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
		const clang::SourceManager &SM = m_astCon.getSourceManager();
		return !(SM.isInSystemHeader(loc) || SM.isInSystemMacro(loc));
		}
	return false;
}

static string getIfCondVar(const Stmt * ST)
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
			           	return declRef->getDecl()->getNameAsString();
			        }    
		        }
			}
		}
	}
}

bool UnvalidatedLoopBoundaryVisitor::hasParentIfStmt(const Stmt * S)
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
					           	var2 = declRef->getDecl()->getNameAsString();
					        }    
				        }
					}
				}
			}
			return true;
		}
		return false;
	}
}

bool hasChildIfStmt(const Stmt *S)
{
	if((S != nullptr) && (S->getSourceRange().isValid()) && (S->child_begin()!=S->child_end()))
	{
		for (const Stmt *child : S->children())
		{
			if((child != nullptr) && (S->getSourceRange().isValid()))
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
						           	var2 = declRefex->getDecl()->getNameAsString();
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
					const bool returnval = hasChildIfStmt(child);
					if(returnval == 1)
					{
						return returnval;
					}
				}				
			}
		}
	}
}

bool UnvalidatedLoopBoundaryVisitor::VisitCallExpr(clang::CallExpr * pCallExpr)
{
	if((pCallExpr != nullptr) && (pCallExpr->getDirectCallee() != nullptr))
	{
		if(inputFunctions.find(pCallExpr->getDirectCallee()->getDeclName().getAsString()) != inputFunctions.end())
		{
			for (auto argument : pCallExpr->arguments())
      		{
      			if(clang::isa<clang::UnaryOperator>(argument))
      			{
      				const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(argument);
      				if(unaryOp && (unaryOp->getSubExpr()))
      				{	
	      				if(unaryOp->getSubExpr()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
	              		{
	                		const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(unaryOp->getSubExpr());
	                		if(declRef && (declRef->getDecl()))
	                		{
	                			var3 = declRef->getDecl()->getNameAsString();
	                		}                		
	                	}
	                }	
      			}
      		}
		}
	}
	return true;
}

bool UnvalidatedLoopBoundaryVisitor::VisitStmt(clang::Stmt *pStmt)
{	
	const ForStmt *FS;
	if(pStmt->getStmtClass() == Expr::StmtClass::ForStmtClass)
	{
		FS = dyn_cast<ForStmt>(pStmt);
		if((FS != nullptr) && (FS->getCond() != nullptr) && (isInUserCode(FS->getForLoc())))
		{
			Expr::StmtClass sclass = FS->getCond()->getStmtClass();
		    if (Expr::StmtClass::BinaryOperatorClass == sclass)
		    {
		    	const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(FS->getCond());
			    if ((binaryOperator != nullptr) && (binaryOperator->getRHS()))
			    {
			    	if( binaryOperator->getRHS()->IgnoreImplicit()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
			    	{
			    		const DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(binaryOperator->getRHS()->IgnoreImplicit());
			    		if((declRefExpr != nullptr) && (declRefExpr->getDecl())) 
			    		{
			    			var1 = declRefExpr->getDecl()->getNameAsString();

			    			clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
							const auto& parents = m_astCon.getParents(*FS);
						    const Stmt* ST = parents[0].get<Stmt>();
						    
						    if(ST != nullptr)
						    {
							    if(isa<CompoundStmt>(ST))
							    {
							    	if(!hasParentIfStmt(ST))
							    	{
							    		if(!hasChildIfStmt(ST))
							    		{
							    			m_voilationsSrcLoc.emplace_back(FS->getForLoc());
							    		}
							    	}  	
							    }
							    else if(!hasParentIfStmt(FS))
							    {
							    	if(!hasChildIfStmt(ST))
							    	{
							    		m_voilationsSrcLoc.emplace_back(FS->getForLoc());
							    	}
							    }

							    if(hasChildIfStmt(ST) && ((var1 != "") 
							    					  && (var2 != "") 
							    					  && (var3 != "") 
							    					  && (var1 == var2)
							    					  && (var1 == var3) 
							    	                  && (ifStmtObj !=nullptr)))
								{
									bool hasReturnStmt = false;
									for (const Stmt *child : ifStmtObj->children())
									{
										if((child != nullptr) && (ifStmtObj->getSourceRange().isValid()))
										{
											if(clang::isa<clang::ReturnStmt>(child))
											{
												hasReturnStmt = true;
											}
										}					
									}
									if(hasReturnStmt == false)
									{
										m_voilationsSrcLoc.emplace_back(FS->getForLoc());	
									}
								}

							    if(hasParentIfStmt(ST) && ((var1 != "") 
							    					   && (var2 != "") 
							    					   && (var3 != "") 
							    					   && (var1 != var2) 
							    					   && (var1 == var3)
							    					   ))
								{
									m_voilationsSrcLoc.emplace_back(FS->getForLoc());
								}
						    }
			    		}
			    	}
				}
			}
		}
	}
	return true;
}


