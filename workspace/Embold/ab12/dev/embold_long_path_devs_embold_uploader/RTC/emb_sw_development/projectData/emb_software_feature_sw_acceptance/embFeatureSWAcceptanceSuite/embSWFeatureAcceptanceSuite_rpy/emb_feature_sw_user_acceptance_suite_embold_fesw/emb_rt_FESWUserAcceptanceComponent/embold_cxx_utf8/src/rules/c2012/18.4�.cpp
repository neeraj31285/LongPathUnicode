//===--- 18.4.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "18.4î.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_18_4::Rule_18_4(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_18_4::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::stmt(anyOf(binaryOperator(hasOperatorName("+"), hasEitherOperand(ignoringImpCasts(declRefExpr(to(varDecl(hasType(pointerType()))))))), binaryOperator(hasOperatorName("-"), hasEitherOperand(ignoringImpCasts(declRefExpr(to(varDecl(hasType(pointerType()))))))), binaryOperator(hasOperatorName("+="), hasEitherOperand(hasType(pointerType()))), binaryOperator(hasOperatorName("-="), hasEitherOperand(hasType(pointerType()))), binaryOperator(hasOperatorName("+"), hasEitherOperand(ignoringImpCasts(declRefExpr(to(varDecl(hasType(decayedType(hasDecayedType(pointerType()))))))))))).bind("Stmt"), this);
        }

        void Rule_18_4::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const Stmt* S = Result.Nodes.getNodeAs<Stmt>("Stmt");
            if (S != nullptr) {
                diag(S->getSourceRange().getBegin());
            }
        }

    } // namespace c2012
} // namespace emboldCxx
