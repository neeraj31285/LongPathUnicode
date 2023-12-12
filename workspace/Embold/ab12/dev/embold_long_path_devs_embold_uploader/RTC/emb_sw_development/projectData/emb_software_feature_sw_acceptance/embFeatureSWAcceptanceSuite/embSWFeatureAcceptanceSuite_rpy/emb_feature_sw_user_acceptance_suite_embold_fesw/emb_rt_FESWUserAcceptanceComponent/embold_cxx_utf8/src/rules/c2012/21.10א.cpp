//===--- 21.10.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "21.10घ.h"
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

        Rule_21_10::Rule_21_10(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_21_10::registerPPCallbacksImpl() {
            using BannedIncludePPCallback = common::BannedInclude<Rule_21_10>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedIncludePPCallback>(
                *this, BannedIncludePPCallback::StringSet{ "time.h" }));

            using BannedMacroPPCallback = common::BannedMacro<Rule_21_10>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedMacroPPCallback>(
                *this, BannedMacroPPCallback::StringSet{ "CLOCKS_PER_SEC" }));
        }

        void Rule_21_10::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::callExpr(hasDeclaration(
                anyOf(functionDecl(hasName("clock")).bind("functionDecl"),
                    functionDecl(hasName("difftime")).bind("functionDecl"),
                    functionDecl(hasName("mktime")).bind("functionDecl"),
                    functionDecl(hasName("time")).bind("functionDecl"),
                    functionDecl(hasName("asctime")).bind("functionDecl"),
                    functionDecl(hasName("ctime")).bind("functionDecl"),
                    functionDecl(hasName("gmtime")).bind("functionDecl"),
                    functionDecl(hasName("localtime")).bind("functionDecl"),
                    functionDecl(hasName("strftime")).bind("functionDecl"))))
                .bind("callExpr"), this);
        }

        void Rule_21_10::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            auto* E = Result.Nodes.getNodeAs<CallExpr>("callExpr");
            const FunctionDecl* D = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl");
            if (!D->isInvalidDecl())
            {
                StringRef filePath = Result.Context->getSourceManager().getFilename(D->getLocation());
                StringRef fileName = llvm::sys::path::filename(filePath);
                if (fileName.equals("time.h"))
                {
                    if (E->getBeginLoc().isValid())
                    {
                        diag(E->getBeginLoc());
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx
