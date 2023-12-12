//===--- 10.5.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "10.5ぅ.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_10_5::Rule_10_5(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_10_5::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::cStyleCastExpr().bind("CStyleCastExpr"), this);
        }

        void Rule_10_5::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const CStyleCastExpr* D = Result.Nodes.getNodeAs<CStyleCastExpr>("CStyleCastExpr");

            if (D)
            {
                if (D->getBeginLoc().isValid())
                {
                    // For handling Exception i.e enum type can be casted to same enum type
                    clang::CastKind castkind = D->getCastKind();
                    if (castkind == clang::CastKind::CK_NoOp)
                    {
                        return;
                    }

                    const Expr* expr = D->getSubExprAsWritten()->IgnoreImplicit();
                    if (expr)
                    {
                        QualType qualtype1;
                        QualType qualtype2;

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
                                    qualtype2 = D->getType();
                                }
                            }
                        }

                        // if casting is done with Interger literal directly.
                        //Example:  enc = ( enum_t ) 3;       /* Non-compliant */

                        else {
                            qualtype1 = expr->getType();
                            qualtype2 = D->getType();
                        }

                        const Type* type1 = qualtype1.getTypePtrOrNull();
                        const Type* type2 = qualtype2.getTypePtrOrNull();

                        if (type1 && type2)
                        {
                            /*  An expression of 'essentially Boolean' type (%1s) is being cast to character type '%2s'.
                                An expression of 'essentially Boolean' type (%1s) is being cast to enum type '%2s'.
                                An expression of 'essentially Boolean' type (%1s) is being cast to signed type '%2s'.
                                An expression of 'essentially Boolean' type (%1s) is being cast to unsigned type '%2s'.
                                An expression of 'essentially Boolean' type (%1s) is being cast to floating type '%2s'.
                            */

                            if (type1->isBooleanType())
                            {
                                if (type2->isIntegerType())
                                {
                                    const IntegerLiteral* integerLiteral = dyn_cast<IntegerLiteral>(const_cast<Expr*>(D->getSubExprAsWritten()));
                                    if (integerLiteral)
                                    {
                                        if (integerLiteral->getValue() == 1 || integerLiteral->getValue() == 0)
                                        {
                                            return;      // Bool can be casted to 1 and 0
                                        }
                                    }
                                }
                                if (type2->isCharType() || type2->isEnumeralType() || type2->isFloatingType() || type2->isUnsignedIntegerType() || type2->isSignedIntegerType())
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }

                            /* An expression of 'essentially character' type (%1s) is being cast to Boolean type, '%2s'.
                                An expression of 'essentially character' type (%1s) is being cast to enum type, '%2s'.
                                An expression of 'essentially character' type (%1s) is being cast to floating type, '%2s'.
                            */

                            if (type1->isCharType())
                            {
                                if (type2->isBooleanType() || type2->isEnumeralType() || type2->isFloatingType())
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }

                            /*
                                An expression of 'essentially enum' type (%1s) is being cast to Boolean type, '%2s'.
                                An expression of 'essentially enum' type (%1s) is being cast to a different enum type, '%2s'.
                            */

                            if (type1->isEnumeralType())
                            {
                                if (type2->isBooleanType() || type2->isEnumeralType())
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }
                            /*
                               An expression of 'essentially signed' type (%1s) is being cast to Boolean type '%2s'.
                               An expression of 'essentially signed' type (%1s) is being cast to enum type, '%2s'.
                               An expression of 'essentially unsigned' type (%1s) is being cast to Boolean type '%2s'.
                               An expression of 'essentially unsigned' type (%1s) is being cast to enum type '%2s'.
                            */

                            if (type1->isSignedIntegerType() || type1->isUnsignedIntegerType())
                            {
                                if (type2->isBooleanType())
                                {
                                    const IntegerLiteral* integerLiteral = dyn_cast<IntegerLiteral>(const_cast<Expr*>(D->getSubExprAsWritten()));
                                    if (integerLiteral)
                                    {
                                        if (integerLiteral->getValue() == 1 || integerLiteral->getValue() == 0)
                                        {
                                            return;    // 1 and 0 can be casted to Bool
                                        }
                                    }
                                }
                                if (type2->isBooleanType() || type2->isEnumeralType())
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }

                            /*
                               An expression of 'essentially floating' type (%1s) is being cast to Boolean type '%2s'.
                               An expression of 'essentially floating' type (%1s) is being cast to character type '%2s'.
                               An expression of 'essentially floating' type (%1s) is being cast to enum type, '%2s'.
                            */

                            if (type1->isFloatingType())
                            {
                                if (type2->isBooleanType() || type2->isEnumeralType() || type2->isCharType())
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }

                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx
