//===--- 11.2cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "11.2ż.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_11_2::Rule_11_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_11_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(castExpr(ignoringImplicit(castExpr())).bind("CastExpr"), this);
        }

        void Rule_11_2::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
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
                            Expr::StmtClass sclass = expr->getStmtClass();
                            if (Expr::StmtClass::DeclRefExprClass == sclass)
                            {
                                const DeclRefExpr* declRef = dyn_cast<DeclRefExpr>(const_cast<Expr*>(D->getSubExprAsWritten()));
                                if (declRef)
                                {
                                    const ValueDecl* valueDecl = declRef->getDecl();
                                    if (valueDecl)
                                    {
                                        const QualType qualtype1 = valueDecl->getType();
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
                                                        if (type2->isIncompleteType() && !type2->isVoidType())
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
                            else
                            {
                                const QualType qualtype2 = D->getType();
                                if (!qualtype2.isNull())
                                {
                                    const Type* type3 = qualtype2.getTypePtrOrNull();
                                    if (type3)
                                    {
                                        const QualType pointeeType2 = type3->getPointeeType();
                                        if (!pointeeType2.isNull())
                                        {
                                            const Type* type4 = pointeeType2.getTypePtrOrNull();
                                            if (type4)
                                            {
                                                if (type4->isIncompleteType() && !type4->isVoidType())
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
        }

    } // namespace c2012
} // namespace emboldCxx
