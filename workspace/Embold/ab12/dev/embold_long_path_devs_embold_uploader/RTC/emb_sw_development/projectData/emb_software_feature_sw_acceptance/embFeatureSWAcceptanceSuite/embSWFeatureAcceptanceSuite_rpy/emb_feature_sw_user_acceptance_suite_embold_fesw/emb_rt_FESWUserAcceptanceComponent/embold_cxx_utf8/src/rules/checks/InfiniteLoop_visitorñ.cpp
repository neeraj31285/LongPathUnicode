#include "CheckerUtils.h"
#include "InfiniteLoop_visitor.h"
#include<unordered_set>
#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

static string var1="var",var2="var";

InfiniteLoopVisitor::InfiniteLoopVisitor(const clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& InfiniteLoopVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool InfiniteLoopVisitor::isInUserCode(const clang::SourceLocation &loc) {

		if (loc.isValid()) {
			clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
			const clang::SourceManager &SM = m_astCon.getSourceManager();
			return !(SM.isInSystemHeader(loc) || SM.isInSystemMacro(loc));
		}
		return false;
	}

bool IsUnaryOperator(const Stmt *S)
{
	if((S != nullptr) && (S->getSourceRange().isValid()) && (S->child_begin()!=S->child_end()))
	{
		for (const Stmt *child : S->children())
		{
			if((child != nullptr) && (S->getSourceRange().isValid()))
			if(clang::isa<clang::UnaryOperator>(child))
			{
				const UnaryOperator *unaryOp = dyn_cast<UnaryOperator>(child);
				if((unaryOp!= nullptr) && (unaryOp->isIncrementDecrementOp()))
				{
					const DeclRefExpr *declRef = dyn_cast<DeclRefExpr>(unaryOp->getSubExpr());
	        if((declRef != nullptr) && (declRef->getDecl() != nullptr))
	        {
	        	var1 = declRef->getDecl()->getNameAsString();
		        return 1;
	        }    
				}
			}
			else
			{
				if((child != nullptr) && (child->getSourceRange().isValid()))
				{
					const bool returnval = IsUnaryOperator(child);
					if(returnval == 1)
					{
						return returnval;
					}
				}				
			}
		}
	}
}


bool InfiniteLoopVisitor::getChildren(const Stmt *S)
{
	if((S != nullptr) && (isInUserCode(S->getBeginLoc())) && (S->getSourceRange().isValid()) && (S->child_begin()!=S->child_end()))
	{	
	for (const Stmt *child : S->children())
	{
		if((child != nullptr) && (S->getSourceRange().isValid()))
		if(clang::isa<clang::BreakStmt>(child))
		{
			clang::ASTContext& m_astCon = const_cast<ASTContext &>(m_astContext);
			const auto& parents = m_astCon.getParents(*S);
      const Stmt* ST = parents[0].get<Stmt>();
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
      }
			return 1;
		}
		else
		{
			if((child != nullptr) && (child->getSourceRange().isValid()))
			{
				const bool returnval = getChildren(child);
				if(returnval == 1)
				{
					return returnval;
				}
			}
		}
	}
}
}

bool InfiniteLoopVisitor::VisitStmt(Stmt *S)
{
	if(isInUserCode(S->getBeginLoc()))
	if(S->getStmtClassName() == "WhileStmt")
	{
		const WhileStmt *WS = dyn_cast<WhileStmt>(S);
		if((WS->getBody() != nullptr) && (WS->getCond() != nullptr))
		{
			//Handling cases like: while(true){..(no break statement) ..}
			if(WS->getCond()->getStmtClass() == Expr::StmtClass::CXXBoolLiteralExprClass)
			{
				const CXXBoolLiteralExpr *boolLiteral = dyn_cast<CXXBoolLiteralExpr>(WS->getCond());
		    if (boolLiteral != nullptr)
		    {
		     	if(boolLiteral->getValue() == 1)
		     	{
		     		if(getChildren(WS) != 1)
		     		{
							m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
						}
						else
						{
							IsUnaryOperator(WS);
							if((var1 != var2) && (var2 != "var"))
							{
								m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
							}
						}
				 	}
		    }
			}

			// Handling cases like: while(1){..(no break statement) ..}
			else if(WS->getCond()->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::IntegerLiteralClass)
			{
				const IntegerLiteral *condition_value = dyn_cast<IntegerLiteral>(WS->getCond()->IgnoreImpCasts());
				if(condition_value != nullptr)
				{
					if(condition_value->getValue() == 1)
					{
						if(getChildren(WS) != 1)
						{
							m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
						}
						else
						{
							IsUnaryOperator(WS);
							if((var1 != var2) && (var2 != "var"))
							{
								m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
							}
						}
					}
				}
			}

			// Handling cases where while loop is empty or suspecious semicolon at while loop. Example: while(a==b);
			if ((WS->getBody() != nullptr) && (WS->getBody()->child_begin() == WS->getBody()->child_end()))
			{
				m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
			}

			// Handling cases where assignment operator is used instead of comparision in while condition. Example:while(a=b)
			if (Expr::StmtClass::BinaryOperatorClass == WS->getCond()->getStmtClass())
			{
				const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(WS->getCond());
		    if (binaryOperator != nullptr)
		    {
        	if(binaryOperator->getOpcodeStr().str() == "=")
        	{
        		m_voilationsSrcLoc.emplace_back(WS->getBeginLoc());
        	}
        }
			}
		}
	}
	return true;
}



bool InfiniteLoopVisitor::TraverseForStmt(ForStmt *pForStmt)
{
	if((pForStmt != nullptr) && (isInUserCode(pForStmt->getForLoc())))
	{
		auto *for_cond = pForStmt->getCond();
		auto *for_inc = pForStmt->getInc();
		auto *for_init = pForStmt->getInit();
		unordered_set<string> opsetSet={"<=", ">="};

		if(for_cond == nullptr && for_inc == nullptr)
		{
			m_voilationsSrcLoc.emplace_back(pForStmt->getForLoc());
		}
		
		else if(for_cond != nullptr)
		{
			Expr::StmtClass sclass = for_cond->getStmtClass();
	    if (Expr::StmtClass::BinaryOperatorClass == sclass)
	    {
	      const BinaryOperator *binaryOperator = dyn_cast<BinaryOperator>(for_cond);
	      if (binaryOperator != nullptr)
	      {
	        llvm::APSInt Value1;
	        llvm::APSInt Value2;
	        if(opsetSet.find(binaryOperator->getOpcodeStr().str()) != opsetSet.end())
	        {
	          if((binaryOperator->getRHS() != nullptr) && (binaryOperator->getLHS() != nullptr))
	          {
	            if(binaryOperator->getRHS()->getStmtClass() == Expr::StmtClass::IntegerLiteralClass )
	            {
	              const IntegerLiteral *integerLiteral = dyn_cast<IntegerLiteral>(binaryOperator->getRHS());
	              if (integerLiteral != nullptr)
	              {
	                Value1 = integerLiteral->getValue();
	              }
	            }                                       
	            else if(binaryOperator->getLHS()->getStmtClass() == Expr::StmtClass::IntegerLiteralClass )
	            {
	              const IntegerLiteral *integerLiteral = dyn_cast<IntegerLiteral>(binaryOperator->getLHS());
	              if (integerLiteral != nullptr)
	              {
	                Value1 = integerLiteral->getValue();
	              }
	            }
              if(binaryOperator->getLHS()->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
              {
                const DeclRefExpr *declRefExpr = dyn_cast<DeclRefExpr>(binaryOperator->getLHS()->IgnoreImpCasts());
                if((declRefExpr != nullptr) && (declRefExpr->getDecl() != nullptr))
	              {
	                if(declRefExpr->getDecl()->getKind() == clang::Decl::Kind::Var)
	                {
	                  const clang::VarDecl *varDecl = dyn_cast<clang::VarDecl>(declRefExpr->getDecl());
	                  if(varDecl != nullptr)
	                  {
	                    const clang::ASTContext & ctx = varDecl->getASTContext();
	                    const clang::Expr * initializerExpr = varDecl->getInit();
	                    clang::Expr::EvalResult result;
	                    if(initializerExpr != nullptr)
	                    {
	                    	if(initializerExpr->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
		                    {
		                     	const DeclRefExpr *declRefEx = dyn_cast<DeclRefExpr>(initializerExpr->IgnoreImpCasts());
		                    	if((declRefEx != nullptr) && (declRefEx->getDecl() != nullptr))
		                     	{
		                     		if(declRefEx->getDecl()->getKind() == clang::Decl::Kind::ParmVar)
		                     		{
		                     			return true;
		                     		}
		                     	}
		                    }
	                      if (initializerExpr->EvaluateAsInt(result, ctx)) 
		                    {
		                      Value2 = result.Val.getInt();
		                    } 
			                }
	                  }
	                }                                
	              }
	            }
	          }

	          /* Hanling cases like for(int i=1; i<=1; i++) */
	          if((for_init != nullptr) && (for_init->getStmtClass() == Expr::StmtClass::DeclStmtClass))
	          {
	            const clang::DeclStmt *declStmt = dyn_cast<clang::DeclStmt>(for_init);
	            if((declStmt != nullptr) && (declStmt->getSingleDecl() != nullptr))
	            {
	              if(declStmt->getSingleDecl()->getKind() == clang::Decl::Kind::Var)
	              {
	                const clang::VarDecl *varDecl = dyn_cast<clang::VarDecl>(declStmt->getSingleDecl());
	                if(varDecl != nullptr)
	                {
	          	      const clang::ASTContext & ctx = varDecl->getASTContext();
	                  const clang::Expr * initializerExpr = varDecl->getInit();
	                  clang::Expr::EvalResult result;
	                  if(initializerExpr != nullptr)
				            {
		                  if(initializerExpr->IgnoreImpCasts()->getStmtClass() == Expr::StmtClass::DeclRefExprClass)
		          	      {
		                   	const DeclRefExpr *declRefExp = dyn_cast<DeclRefExpr>(initializerExpr->IgnoreImpCasts());
		                   	if((declRefExp != nullptr) && (declRefExp->getDecl() != nullptr))
		                  	{
		                   		if(declRefExp->getDecl()->getKind() == clang::Decl::Kind::ParmVar)
		                   		{
		                   			return true;
		                   		}
		                   	}
		                  }			                            
				              if (initializerExpr->EvaluateAsInt(result, ctx)) 
				              {
				                Value2 = result.Val.getInt();
				              } 
				        	  }
	                }                                                                  
	              }
	            }
	          }
		  SmallString<16> S1, S2;
		  Value1.toString(S1,10, false,false);
		  Value2.toString(S2,10, false,false);
	          if(S1 == S2)
	          {
	            m_voilationsSrcLoc.emplace_back(pForStmt->getForLoc());
	          }
	        }
	      }
	    }
		}
	}
	return true;
}

