//===--- 8.4.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.4.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_4::Rule_8_4(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_4::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::varDecl(isDefinition()).bind("VarDecl"), this);
            Finder->addMatcher(ast_matchers::functionDecl(isDefinition()).bind("FunctionDecl"), this);
        }

        void Rule_8_4::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const VarDecl* D = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
            const FunctionDecl* F = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");

            if (D != nullptr)
            {
                if (!D->isInvalidDecl() && !D->isImplicit())
                {
                    const clang::Linkage linkage = D->getLinkageInternal();
                    if (linkage == clang::Linkage::ExternalLinkage)
                    {
                        if (!D->isWeak())
                        {
                            clang::VarDecl::DefinitionKind definitionKind = D->isThisDeclarationADefinition();
                            if (definitionKind == clang::VarDecl::DefinitionKind::TentativeDefinition)
                            {
                                const VarDecl* varDecl = D->getDefinition();
                                const Decl* decl = D->getPreviousDecl();
                                if (!varDecl && !decl)
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }

                            if (definitionKind == clang::VarDecl::DefinitionKind::Definition)
                            {
                                const Decl* declaration = D->getPreviousDecl();
                                if (!declaration)
                                {
                                    diag(D->getSourceRange().getBegin());
                                }
                            }
                        }
                    }
                }
            }


            if (F != nullptr)
            {
                if (!F->isInvalidDecl() && !F->isImplicit())
                {
                    const clang::Linkage linkage = F->getLinkageInternal();
                    if (linkage == clang::Linkage::ExternalLinkage)
                    {
                        if (!F->isWeak())
                        {
                            const Decl* declaration = F->getPreviousDecl();
                            if (!declaration)
                            {
                                diag(F->getSourceRange().getBegin());
                            }
                        }
                    }
                }
            }
        }


    } // namespace c2012
} // namespace emboldCxx



