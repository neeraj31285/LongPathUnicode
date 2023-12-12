//===--- 21.8.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "21.8ഖ.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"
#include "rules/common/BannedIncludeघ.h"
#include "rules/common/BannedMacroş.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_21_8::Rule_21_8(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        /*void Rule_21_8::registerPPCallbacksImpl() {
          using BannedIncludePPCallback = common::BannedInclude<Rule_21_8>;
          getPreprocessor().addPPCallbacks(::llvm::make_unique<BannedIncludePPCallback>(
              *this, BannedIncludePPCallback::StringSet{"stdlib.h"}));
        }*/

        void Rule_21_8::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::callExpr(hasDeclaration(
                anyOf(functionDecl(hasName("abort")).bind("functionDecl"),
                    functionDecl(hasName("exit")).bind("functionDecl"),
                    functionDecl(hasName("system")).bind("functionDecl")
                ))).bind("callExpr"), this);
        }

        void Rule_21_8::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            auto* E = Result.Nodes.getNodeAs<CallExpr>("callExpr");
            const FunctionDecl* D = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl");
            StringRef filePath = Result.Context->getSourceManager().getFilename(D->getLocation());
            StringRef fileName = llvm::sys::path::filename(filePath);
            if (fileName.equals("stdlib.h")) {
                diag(E->getBeginLoc());
            }
        }

    } // namespace c2012
} // namespace emboldCxx
