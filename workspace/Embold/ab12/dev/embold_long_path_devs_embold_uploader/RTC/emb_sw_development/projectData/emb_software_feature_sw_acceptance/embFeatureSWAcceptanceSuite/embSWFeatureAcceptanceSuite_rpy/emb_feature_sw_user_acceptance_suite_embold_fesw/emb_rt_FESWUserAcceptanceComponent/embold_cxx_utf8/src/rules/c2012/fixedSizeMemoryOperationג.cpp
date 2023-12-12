//===--- fixedSizeMemoryOperation.cpp - clang-tidy-misra -----------------
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------

#include "fixedSizeMemoryOperationങ.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_fixedSizeMemoryOperation::Rule_fixedSizeMemoryOperation(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_fixedSizeMemoryOperation::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::callExpr(callee(functionDecl(anyOf(hasName("memcpy"),
                hasName("memcpy_s"),
                hasName("memset"),
                hasName("memset_s"),
                hasName("memcpy"),
                hasName("memcmp"),
                hasName("memmove"),
                hasName("memmove_s"),
                hasName("memchr"),
                hasName("malloc"),
                hasName("calloc"),
                hasName("realloc"),
                hasName("strncat"),
                hasName("strncat_s"),
                hasName("strncmp"))))).bind("CallExpr"), this);
        }

        static bool isMacro(clang::SourceLocation loc, clang::SourceManager& SM) {
            bool macro = false;
            if (SM.isMacroBodyExpansion(loc) == 1) {
                macro = true;
            }
            return macro;
        }

        void Rule_fixedSizeMemoryOperation::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const CallExpr* E = Result.Nodes.getNodeAs<CallExpr>("CallExpr");

            if (E != nullptr)
            {
                if (E->getSourceRange().isValid() == 1)
                {
                    const Expr* const* arguments = E->getArgs();
                    for (auto i = 0; i < E->getNumArgs(); i++)
                    {
                        SourceManager& SM = Result.Context->getSourceManager();
                        Expr::StmtClass sclass = arguments[i]->IgnoreParenImpCasts()->getStmtClass();
                        if (Expr::StmtClass::BinaryOperatorClass == sclass)
                        {
                            const BinaryOperator* binaryOperator = dyn_cast<BinaryOperator>(arguments[i]->IgnoreParenImpCasts());
                            const Expr::StmtClass sclass_RHS = binaryOperator->getRHS()->IgnoreParenImpCasts()->getStmtClass();
                            const Expr::StmtClass sclass_LHS = binaryOperator->getLHS()->IgnoreParenImpCasts()->getStmtClass();
                            const SourceLocation RHS_loc = binaryOperator->getRHS()->IgnoreParenImpCasts()->getExprLoc();
                            const SourceLocation LHS_loc = binaryOperator->getLHS()->IgnoreParenImpCasts()->getExprLoc();

                            if (((Expr::StmtClass::IntegerLiteralClass == sclass_RHS) && (isMacro(RHS_loc, SM) == 0)) ||
                                ((Expr::StmtClass::IntegerLiteralClass == sclass_LHS) && (isMacro(LHS_loc, SM) == 0)))
                            {
                                diag(E->getSourceRange().getBegin());
                            }
                        }
                        if ((Expr::StmtClass::IntegerLiteralClass == sclass) && (isMacro(arguments[i]->IgnoreParenImpCasts()->getExprLoc(), SM) == 0))
                        {
                            diag(E->getSourceRange().getBegin());
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx

