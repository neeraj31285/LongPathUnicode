//===--- 2.7.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "2.7š.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/Lexer.h"
#include <unordered_set>

using namespace clang;
using namespace clang::tidy;

namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        void Rule_2_7::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(functionDecl(isDefinition(), hasBody(stmt()), hasAnyParameter(decl())).bind("function"), this);
        }

        Rule_2_7::Rule_2_7(llvm::StringRef Name, ClangTidyContext* Context) : EmboldTidyCheck(Name, Context) {}

        void Rule_2_7::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const auto* Function = Result.Nodes.getNodeAs<FunctionDecl>("function");
            if (Function)
            {
                if (!Function->isInvalidDecl() && !Function->isImplicit())
                {
                    if (!Function->hasWrittenPrototype() || Function->isTemplateInstantiation())
                    {
                        return;
                    }
                    if (const auto* Method = dyn_cast<CXXMethodDecl>(Function))
                    {
                        if (Method->isLambdaStaticInvoker())
                        {
                            return;
                        }
                    }
                    for (unsigned i = 0, e = Function->getNumParams(); i != e; ++i)
                    {
                        const auto* Param = Function->getParamDecl(i);
                        if (Param->isUsed() || Param->isReferenced() || !Param->getDeclName() || Param->hasAttr<UnusedAttr>())
                        {
                            continue;
                        }
                        diag(Function->getSourceRange().getBegin());
                    }
                }
            }
        }
    } // namespace c2012
} // namespace emboldCxx
