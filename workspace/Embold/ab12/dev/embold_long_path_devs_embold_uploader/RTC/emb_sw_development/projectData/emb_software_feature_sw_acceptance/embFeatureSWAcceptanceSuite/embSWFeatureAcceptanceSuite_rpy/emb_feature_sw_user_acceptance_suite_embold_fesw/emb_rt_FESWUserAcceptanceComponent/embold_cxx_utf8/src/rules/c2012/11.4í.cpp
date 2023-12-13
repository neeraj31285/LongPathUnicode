//===--- 11.4cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "11.4.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_11_4::Rule_11_4(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_11_4::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(castExpr().bind("CastExpr"), this);
        }

        void Rule_11_4::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CastExpr* D = Result.Nodes.getNodeAs<CastExpr>("CastExpr");
            if (D)
            {
                if (D->getBeginLoc().isValid())
                {
                    clang::CastKind castkind = D->getCastKind();
                    if (castkind == clang::CastKind::CK_PointerToBoolean || castkind == clang::CastKind::CK_IntegralToPointer || castkind == clang::CastKind::CK_PointerToIntegral)
                    {
                        diag(D->getSourceRange().getBegin());
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx