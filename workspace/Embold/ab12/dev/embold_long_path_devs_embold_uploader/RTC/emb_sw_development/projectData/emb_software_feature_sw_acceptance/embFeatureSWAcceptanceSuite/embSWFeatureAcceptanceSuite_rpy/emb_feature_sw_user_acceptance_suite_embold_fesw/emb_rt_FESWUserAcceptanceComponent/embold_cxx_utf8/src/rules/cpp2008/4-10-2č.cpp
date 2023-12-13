//===--- 4-10-2.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "4-10-2.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
using namespace clang::ast_matchers;

namespace emboldCxx {
    namespace cpp2008 {

        Rule_4_10_2::Rule_4_10_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_4_10_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(
                integerLiteral(equals(0),
                    hasParent(expr(anyOf(hasType(pointerType()),
                        hasType(memberPointerType())))))
                .bind("ZeroAsNullptr"),
                this);
        }

        void Rule_4_10_2::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            if (const auto* integerLiteralStmt =
                Result.Nodes.getNodeAs<Stmt>("ZeroAsNullptr")) {
                diag(integerLiteralStmt->getBeginLoc());
            }
        }

    } // namespace cpp2008
} // namespace emboldCxx


