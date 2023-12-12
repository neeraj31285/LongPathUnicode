//===--- 18.7.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "18.7А.h"
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

        Rule_18_7::Rule_18_7(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_18_7::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(fieldDecl(hasType(incompleteArrayType())).bind("FieldDecl"), this);
        }

        void Rule_18_7::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const FieldDecl* D = Result.Nodes.getNodeAs<FieldDecl>("FieldDecl");
            if (D != nullptr)
            {
                diag(D->getSourceRange().getBegin());
            }

        }

    } // namespace c2012
} // namespace emboldCxx
