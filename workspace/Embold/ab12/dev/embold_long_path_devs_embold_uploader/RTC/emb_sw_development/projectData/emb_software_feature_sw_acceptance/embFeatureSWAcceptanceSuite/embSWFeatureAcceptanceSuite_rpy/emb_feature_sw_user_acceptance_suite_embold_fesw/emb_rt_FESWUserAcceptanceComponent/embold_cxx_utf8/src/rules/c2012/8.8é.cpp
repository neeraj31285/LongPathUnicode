//===--- 8.8.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.8.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_8::Rule_8_8(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_8::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::functionDecl().bind("FunctionDecl"), this);
            Finder->addMatcher(ast_matchers::varDecl().bind("VarDecl"), this);
        }

        void Rule_8_8::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const FunctionDecl* D = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");
            const VarDecl* V = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
            if (D != nullptr)
            {
                if (!D->isInvalidDecl())
                {
                    const clang::Linkage linkage = D->getLinkageInternal();
                    if (linkage == clang::Linkage::InternalLinkage)
                    {
                        const clang::StorageClass storageClass = D->getStorageClass();
                        if (storageClass != clang::StorageClass::SC_Static)
                        {
                            diag(D->getSourceRange().getBegin());
                        }
                    }

                }
            }
            if (V != nullptr)
            {
                if (!V->isInvalidDecl())
                {
                    const clang::Linkage linkage = V->getLinkageInternal();
                    if (linkage == clang::Linkage::InternalLinkage)
                    {
                        const clang::StorageClass storageClass = V->getStorageClass();
                        if (storageClass != clang::StorageClass::SC_Static)
                        {
                            diag(V->getSourceRange().getBegin());
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx




