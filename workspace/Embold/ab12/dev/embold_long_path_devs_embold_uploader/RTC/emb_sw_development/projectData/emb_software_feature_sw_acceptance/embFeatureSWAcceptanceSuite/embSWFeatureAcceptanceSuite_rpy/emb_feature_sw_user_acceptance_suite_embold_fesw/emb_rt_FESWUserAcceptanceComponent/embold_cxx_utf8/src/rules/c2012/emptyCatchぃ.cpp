//===--- emptyCatch.cpp - clang-tidy-misra ---------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "emptyCatch.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_emptyCatch::Rule_emptyCatch(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_emptyCatch::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::cxxCatchStmt(has(compoundStmt(statementCountIs(0)))).bind("CXXCatchStmt"), this);
        }

        void Rule_emptyCatch::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CXXCatchStmt* D = Result.Nodes.getNodeAs<CXXCatchStmt>("CXXCatchStmt");
            if (D != nullptr)
            {
                if (D->getSourceRange().isValid() == 1)
                {
                    diag(D->getSourceRange().getBegin());
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx

