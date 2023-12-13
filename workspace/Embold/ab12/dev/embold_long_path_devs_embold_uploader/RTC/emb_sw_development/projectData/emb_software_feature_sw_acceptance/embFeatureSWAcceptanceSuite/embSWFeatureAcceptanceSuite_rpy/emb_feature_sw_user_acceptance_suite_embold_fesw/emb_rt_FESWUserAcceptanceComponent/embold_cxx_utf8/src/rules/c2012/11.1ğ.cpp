//===--- 11.1cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "11.1.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_11_1::Rule_11_1(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_11_1::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(castExpr(ignoringImplicit(castExpr())).bind("CastExpr"), this);
        }

        void Rule_11_1::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CastExpr* D = Result.Nodes.getNodeAs<CastExpr>("CastExpr");
            if (D)
            {
                if (D->getBeginLoc().isValid())
                {
                    clang::CastKind castkind = D->getCastKind();
                    if (castkind == clang::CastKind::CK_BitCast || castkind == clang::CastKind::CK_IntegralToPointer || castkind == clang::CastKind::CK_PointerToIntegral)
                    {
                        const Expr* expr = D->getSubExprAsWritten();
                        if (expr)
                        {
                            QualType qualtype1;

                            /* For the castExpr which has declRefExpr as child.
                            Example-
                            typedef void ( *fp32 )( int32_t n );
                            fp32 fp2 = ( fp32) fp1;   (Non-compliant - function pointer to different function pointer)
                            */

                            Expr::StmtClass sclass = expr->getStmtClass();
                            if (Expr::StmtClass::DeclRefExprClass == sclass)
                            {
                                const DeclRefExpr* declRef = dyn_cast<DeclRefExpr>(const_cast<Expr*>(D->getSubExprAsWritten()));
                                if (declRef)
                                {
                                    const ValueDecl* valueDecl = declRef->getDecl();
                                    if (valueDecl)
                                    {
                                        qualtype1 = valueDecl->getType();
                                    }
                                }
                            }

                            /* for castExpr which does not have declRefExpr as child.
                            Example-
                            typedef void ( *fp16 )( int16_t n );
                            fp16 fp3 = ( fp16 ) 0x8000;     (Non-compliant  - integer to function pointer)
                            */

                            else
                            {
                                qualtype1 = D->getType();
                            }

                            /* Checking if type is pointer to function */

                            if (!qualtype1.isNull())
                            {
                                const Type* type1 = qualtype1.getTypePtrOrNull();
                                if (type1)
                                {
                                    const QualType pointeeType1 = type1->getPointeeType();
                                    if (!pointeeType1.isNull())
                                    {
                                        const Type* type2 = pointeeType1.getTypePtrOrNull();
                                        if (type2)
                                        {
                                            if (type2->isFunctionType() && !type2->isVoidType())     // checking void type for exception 2
                                            {
                                                diag(D->getSourceRange().getBegin());
                                            }
                                        }
                                    }
                                }
                            }

                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx