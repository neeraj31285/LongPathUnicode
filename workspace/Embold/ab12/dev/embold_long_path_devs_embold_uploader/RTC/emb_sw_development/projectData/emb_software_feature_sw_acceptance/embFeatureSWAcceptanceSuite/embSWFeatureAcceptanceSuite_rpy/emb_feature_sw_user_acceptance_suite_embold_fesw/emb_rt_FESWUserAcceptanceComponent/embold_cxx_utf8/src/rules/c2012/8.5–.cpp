//===--- 8.5.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.5ก.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_5::Rule_8_5(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_5::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::varDecl(isExternC()).bind("VarDecl"), this);
            Finder->addMatcher(ast_matchers::functionDecl(isExternC()).bind("FunctionDecl"), this);
        }

        void Rule_8_5::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const VarDecl* V = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
            const FunctionDecl* F = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");

            if (V != nullptr)
            {
                if (!V->isInvalidDecl() && !V->isImplicit())
                {
                    clang::VarDecl::DefinitionKind definitionKind = V->isThisDeclarationADefinition();
                    if (definitionKind == clang::VarDecl::DefinitionKind::DeclarationOnly)
                    {
                        const Decl* declaration = V->getPreviousDecl();
                        if (declaration)
                        {
                            diag(V->getSourceRange().getBegin());
                        }
                    }
                }
            }

            if (F != nullptr)
            {
                if (!F->isInvalidDecl() && !F->isImplicit())
                {
                    if (!F->isThisDeclarationADefinition())
                    {
                        const Decl* declaration = F->getPreviousDecl();
                        if (declaration)
                        {
                            if (!declaration->isImplicit())
                                diag(F->getSourceRange().getBegin());
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx



