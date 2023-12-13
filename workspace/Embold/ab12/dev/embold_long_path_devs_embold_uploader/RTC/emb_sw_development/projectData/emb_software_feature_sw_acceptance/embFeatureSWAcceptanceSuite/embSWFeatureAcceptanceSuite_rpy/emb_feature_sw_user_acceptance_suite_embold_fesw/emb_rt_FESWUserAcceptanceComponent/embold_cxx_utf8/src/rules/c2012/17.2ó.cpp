//===--- 17.2.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "17.2.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_17_2::Rule_17_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_17_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(functionDecl(forEachDescendant(callExpr().bind("CallExpr"))).bind("FunctionDecl"), this);
        }

        void Rule_17_2::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CallExpr* C = Result.Nodes.getNodeAs<CallExpr>("CallExpr");
            const FunctionDecl* F = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");
            const Decl* decl = C->getCalleeDecl();
            if (decl == F)
            {
                diag(C->getSourceRange().getBegin());
            }

        }

    } // namespace c2012
} // namespace emboldCxx