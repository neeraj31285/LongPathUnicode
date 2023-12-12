//===--- 12-8-2.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "12-8-2ú.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace cpp2008 {

        using namespace clang::ast_matchers;

        Rule_12_8_2::Rule_12_8_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_12_8_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            auto matcher =
                cxxMethodDecl(ofClass(has(cxxMethodDecl(isVirtual(), isPure()))),
                    hasOverloadedOperatorName("="), isPublic())
                .bind("CXXMethodDecl");
            Finder->addMatcher(matcher, this);
        }

        void Rule_12_8_2::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            if (const auto* methodDecl =
                Result.Nodes.getNodeAs<CXXMethodDecl>("CXXMethodDecl")) {
                diag(methodDecl->getLocation());
            }
        }

    } // namespace cpp2008
} // namespace emboldCxx
