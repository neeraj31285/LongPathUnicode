//===--- 21.12.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "21.12.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "rules/common/BannedInclude.h"
#include "rules/common/BannedMacro.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_21_12::Rule_21_12(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_21_12::registerPPCallbacksImpl() {
            using BannedMacroPPCallback = common::BannedMacro<Rule_21_12>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedMacroPPCallback>(
                *this, BannedMacroPPCallback::StringSet{ "FE_DIVBYZERO", "FE_INEXACT", "FE_INVALID",
                                                        "FE_OVERFLOW", "FE_UNDERFLOW", "FE_ALL_EXCEPT" }));
        }

        void Rule_21_12::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::callExpr(hasDeclaration(
                anyOf(functionDecl(hasName("feclearexcept")),
                    functionDecl(hasName("fegetexceptflag")),
                    functionDecl(hasName("feraiseexcept")),
                    functionDecl(hasName("fesetexceptflag")),
                    functionDecl(hasName("fetestexcept"))))).bind("callExpr"),
                this);
        }

        void Rule_21_12::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            auto* E = Result.Nodes.getNodeAs<CallExpr>("callExpr");
            diag(E->getBeginLoc());
        }

    } // namespace c2012
} // namespace emboldCxx