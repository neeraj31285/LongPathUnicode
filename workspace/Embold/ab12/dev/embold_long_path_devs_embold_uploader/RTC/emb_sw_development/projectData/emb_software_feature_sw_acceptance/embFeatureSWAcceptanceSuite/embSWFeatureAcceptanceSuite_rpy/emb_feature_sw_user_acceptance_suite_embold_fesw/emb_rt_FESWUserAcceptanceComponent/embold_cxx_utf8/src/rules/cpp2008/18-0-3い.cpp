//===--- 18-0-3.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "18-0-3.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "rules/common/BannedInclude.h"
#include <string>

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace cpp2008 {

        Rule_18_0_3::Rule_18_0_3(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_18_0_3::registerMatchers(ast_matchers::MatchFinder* Finder) {
            using namespace clang::ast_matchers;
            Finder->addMatcher(callExpr(callee(functionDecl(matchesName(
                "^(std)?::(exit|abort|getenv|system)$"))))
                .bind("CallExpr"),
                this);
        }

        void Rule_18_0_3::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            if (const auto* callExpr = Result.Nodes.getNodeAs<CallExpr>("CallExpr")) {
                diag(callExpr->getBeginLoc());
            }
        }

    } // namespace cpp2008
} // namespace emboldCxx