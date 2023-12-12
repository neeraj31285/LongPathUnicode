//===--- 11.5cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "11.5い.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_11_5::Rule_11_5(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_11_5::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(castExpr().bind("CastExpr"), this);
        }

        void Rule_11_5::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CastExpr* D = Result.Nodes.getNodeAs<CastExpr>("CastExpr");
            if (D)
            {
                if (D->getBeginLoc().isValid())
                {
                    clang::CastKind castkind = D->getCastKind();
                    if (castkind == clang::CastKind::CK_BitCast)
                    {
                        const Expr* expr = D->getSubExprAsWritten();
                        if (expr)
                        {
                            QualType qualtype1;
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

                            if (!qualtype1.isNull())
                            {
                                const Type* type1 = qualtype1.getTypePtrOrNull();
                                if (type1)
                                {

                                    if (type1->isVoidPointerType())
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

    } // namespace c2012
} // namespace emboldCxx
