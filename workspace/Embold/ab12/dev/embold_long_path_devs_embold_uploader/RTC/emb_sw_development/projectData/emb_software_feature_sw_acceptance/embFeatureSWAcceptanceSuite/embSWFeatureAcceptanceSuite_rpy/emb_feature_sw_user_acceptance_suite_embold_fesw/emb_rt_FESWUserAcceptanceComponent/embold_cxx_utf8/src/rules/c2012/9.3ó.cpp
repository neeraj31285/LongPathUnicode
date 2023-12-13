//===--- 9.3.cpp - clang-tidy-misra ---------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "9.3.h"
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

        Rule_9_3::Rule_9_3(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_9_3::registerMatchers(ast_matchers::MatchFinder* Finder) {
            using namespace ast_matchers;
            Finder->addMatcher(
                varDecl(hasType(constantArrayType()),
                    hasInitializer(initListExpr().bind("initListExpr"))),
                this);
        }

        clang::SmallVector<const Expr*, 1> designatedInitExprs;

        bool IsDesignatedInit(const Expr* initExpr) {
            if (std::find(designatedInitExprs.begin(), designatedInitExprs.end(),
                initExpr) != designatedInitExprs.end()) {
                return true;
            }
            return false;
        }

        bool IsInitListPartial(const InitListExpr* initListExpr,
            const ast_matchers::MatchFinder::MatchResult& Result) {
            size_t numInits = initListExpr->getNumInits();
            if (numInits == 1) {
                const Expr* initExpr = initListExpr->getInit(0);
                if (initExpr != nullptr) {
                    initExpr = initExpr->IgnoreImplicit();
                    if (initExpr->getStmtClass() == Stmt::StmtClass::IntegerLiteralClass) {
                        const IntegerLiteral* integerLiteral = static_cast<const IntegerLiteral*>(initExpr);
                        if (integerLiteral != nullptr) {
                            QualType qualtype = integerLiteral->getType();
                            if (!qualtype.isNull()) {
                                const Type* type = qualtype.getTypePtrOrNull();
                                if (type != nullptr) {
                                    if (integerLiteral->getValue() == 0 && !type->hasUnsignedIntegerRepresentation()) {
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            bool isPartial = false;
            for (const Expr* initExpr : initListExpr->inits()) {
                const InitListExpr* innerInitListExpr =
                    dyn_cast<InitListExpr>(initExpr);
                if (innerInitListExpr) {
                    isPartial = IsInitListPartial(innerInitListExpr, Result);
                    if (isPartial) {
                        break;
                    }
                }
                else {
                    const Expr* arrayFiller = initListExpr->getArrayFiller();
                    if (arrayFiller) {
                        if (!IsDesignatedInit(initExpr)
                            && (initExpr->getStmtClass() !=
                                Stmt::StmtClass::ImplicitValueInitExprClass)) {
                            isPartial = true;
                            break;
                        }
                    }
                    else {
                        break;
                    }
                }
            }
            return isPartial;
        }

        void Rule_9_3::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const clang::InitListExpr* E =
                Result.Nodes.getNodeAs<InitListExpr>("initListExpr");
            std::string symanticAst;
            std::string syntacticAst;
            designatedInitExprs.clear();

            if (!(E->isSyntacticForm())) {
                E = E->getSyntacticForm();
            }

            std::string astStr;
            llvm::raw_string_ostream ss(astStr);
            E->dump(ss, *Result.Context);
            std::string logMsg = ss.str();

            // Collect all the designated exprs
            for (const Expr* initExpr : E->inits()) {
                const DesignatedInitExpr* designatedInitExpr =
                    dyn_cast<DesignatedInitExpr>(initExpr);
                if (designatedInitExpr) {
                    designatedInitExprs.push_back(designatedInitExpr->getInit());
                }
            }

            E = E->getSemanticForm();
            if (IsInitListPartial(E, Result)) {
                diag(E->getExprLoc());
            }
        }

    } // namespace c2012
} // namespace emboldCxx


