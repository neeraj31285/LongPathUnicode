//===--- 7.4.cpp - clang-tidy-misra ---------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "7.4.h"
#include "rules/common/BannedInclude.h"
#include "rules/common/BannedMacro.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include <cassert>

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        Rule_7_4::Rule_7_4(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_7_4::registerMatchers(ast_matchers::MatchFinder* Finder) {
            using namespace ast_matchers;
            Finder->addMatcher(stringLiteral().bind("stringLiteral"), this);
        }

        const clang::CastExpr* get_parent_cast_stmt(const clang::Stmt* stmt,
            ASTContext* Context) {
            auto it = Context->getParents(*stmt).begin();

            if (it == Context->getParents(*stmt).end())
                return nullptr;

            const clang::CastExpr* parentCastExpr = nullptr;
            const clang::Expr* parentExpr = it->get<clang::Expr>();
            if (parentExpr) {
                if ((parentExpr->getStmtClass() == clang::Stmt::StmtClass::ImplicitCastExprClass) ||
                    (parentExpr->getStmtClass() ==
                        clang::Stmt::StmtClass::ImplicitCastExprClass)) {
                    parentCastExpr = static_cast<const clang::CastExpr*>(parentExpr);
                }
                else {
                    parentCastExpr = get_parent_cast_stmt(parentExpr, Context);
                }
            }
            else {
                return nullptr;
            }

            return parentCastExpr;
        }

        void Rule_7_4::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const clang::StringLiteral* E =
                Result.Nodes.getNodeAs<StringLiteral>("stringLiteral");

            const clang::CastExpr* parentCastExpr = get_parent_cast_stmt(E, Result.Context);
            if (parentCastExpr
                && (parentCastExpr->getCastKind()
                    == clang::CastKind::CK_ArrayToPointerDecay)) {
                parentCastExpr = get_parent_cast_stmt(parentCastExpr, Result.Context);
                if (parentCastExpr) {
                    if (parentCastExpr->getCastKind() == clang::CastKind::CK_BitCast) {
                        const clang::Type* typePtr =
                            parentCastExpr->getType().getTypePtrOrNull();
                        if (typePtr && typePtr->isPointerType()) {
                            const PointerType* pointerType = typePtr->getAs<PointerType>();
                            clang::QualType pointeeType = pointerType->getPointeeType();
                            if (!pointeeType.isConstQualified()) {
                                diag(E->getExprLoc());
                            }
                        }
                    }
                }
                else {
                    diag(E->getExprLoc());
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx


