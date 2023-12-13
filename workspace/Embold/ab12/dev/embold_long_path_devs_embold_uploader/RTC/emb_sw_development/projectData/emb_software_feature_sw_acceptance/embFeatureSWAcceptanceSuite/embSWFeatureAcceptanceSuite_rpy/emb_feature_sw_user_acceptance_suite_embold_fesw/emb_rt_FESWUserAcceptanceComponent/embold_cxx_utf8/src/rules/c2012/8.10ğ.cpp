//===--- 8.10.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.10.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_10::Rule_8_10(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_10::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::functionDecl(isInline(), unless(isStaticStorageClass())).bind("FunctionDecl"), this);
        }

        void Rule_8_10::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const FunctionDecl* D = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");
            diag(D->getSourceRange().getBegin());
        }

    } // namespace c2012
} // namespace emboldCxx