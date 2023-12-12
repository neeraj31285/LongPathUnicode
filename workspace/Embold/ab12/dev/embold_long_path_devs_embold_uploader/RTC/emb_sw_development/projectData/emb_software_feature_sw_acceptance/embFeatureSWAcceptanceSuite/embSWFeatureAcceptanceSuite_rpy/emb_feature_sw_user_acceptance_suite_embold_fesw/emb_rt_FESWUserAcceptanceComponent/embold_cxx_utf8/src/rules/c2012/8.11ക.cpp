//===--- 8.11.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.11ô.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;

namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_11::Rule_8_11(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_11::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::varDecl(isExternC(), hasType(incompleteArrayType())).bind("VarDecl"), this);
        }

        void Rule_8_11::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const VarDecl* V = Result.Nodes.getNodeAs<VarDecl>("VarDecl");

            if (V != nullptr)
            {
                if (!V->isInvalidDecl() && !V->isImplicit())
                {
                    diag(V->getSourceRange().getBegin());
                }

            }
        }

    } // namespace c2012
} // namespace emboldCxx
