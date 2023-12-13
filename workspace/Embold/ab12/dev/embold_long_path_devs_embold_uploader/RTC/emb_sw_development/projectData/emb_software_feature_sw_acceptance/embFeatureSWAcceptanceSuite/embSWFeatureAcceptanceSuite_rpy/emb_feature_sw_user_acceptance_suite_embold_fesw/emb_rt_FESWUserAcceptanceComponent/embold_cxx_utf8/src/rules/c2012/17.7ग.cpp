//===--- 17.7.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "17.7.h"
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

        Rule_17_7::Rule_17_7(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_17_7::registerMatchers(ast_matchers::MatchFinder* Finder) {
            auto MatchedCallExpr = expr(ignoringImplicit(ignoringParenImpCasts(callExpr(callee(functionDecl(unless(returns(voidType())), unless(isImplicit())))))))
                .bind("match");

            auto UnusedInCompoundStmt = compoundStmt(forEach(MatchedCallExpr), unless(hasParent(stmtExpr())));
            auto UnusedInIfStmt = ifStmt(eachOf(hasThen(MatchedCallExpr), hasElse(MatchedCallExpr)));
            auto UnusedInWhileStmt = whileStmt(hasBody(MatchedCallExpr));
            auto UnusedInDoStmt = doStmt(hasBody(MatchedCallExpr));
            auto UnusedInForStmt = forStmt(eachOf(hasLoopInit(MatchedCallExpr), hasIncrement(MatchedCallExpr), hasBody(MatchedCallExpr)));
            auto UnusedInRangeForStmt = cxxForRangeStmt(hasBody(MatchedCallExpr));
            auto UnusedInCaseStmt = switchCase(forEach(MatchedCallExpr));

            Finder->addMatcher(stmt(anyOf(UnusedInCompoundStmt, UnusedInIfStmt, UnusedInWhileStmt, UnusedInDoStmt, UnusedInForStmt,
                UnusedInRangeForStmt, UnusedInCaseStmt)), this);

        }

        void Rule_17_7::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            if (const auto* Matched = Result.Nodes.getNodeAs<CallExpr>("match"))
            {
                diag(Matched->getSourceRange().getBegin());
            }

        }

    } // namespace c2012
} // namespace emboldCxx