//===--- 18-0-4.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "18-0-4ď.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "rules/common/BannedIncludeघ.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace cpp2008 {

        Rule_18_0_4::Rule_18_0_4(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_18_0_4::registerMatchers(ast_matchers::MatchFinder* Finder) {
            using namespace clang::ast_matchers;
            Finder->addMatcher(callExpr(callee(functionDecl(matchesName(
                "^(std)?::(clock|difftime|mktime|time)$"))))
                .bind("CallExpr"),
                this);
        }

        void Rule_18_0_4::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            if (const auto* callExpr = Result.Nodes.getNodeAs<CallExpr>("CallExpr")) {
                diag(callExpr->getBeginLoc());
            }
        }

    } // namespace cpp2008
} // namespace emboldCxx
