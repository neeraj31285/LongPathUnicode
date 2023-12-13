//===--- 5.7.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "5.7.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <unordered_set>

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;
        using namespace std;
        Rule_5_7::Rule_5_7(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_5_7::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(namedDecl(anyOf(enumDecl(isDefinition()), recordDecl(isDefinition()))).bind("NamedDecl"), this);
        }

        void Rule_5_7::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const NamedDecl* D = Result.Nodes.getNodeAs<NamedDecl>("NamedDecl");

            if (D)
            {
                if (!D->isInvalidDecl() && !D->isImplicit() && !D->getName().empty())
                {
                    static std::unordered_set<std::string> s;
                    if (s.find(D->getName().str()) != s.end())
                    {
                        diag(D->getSourceRange().getBegin());
                    }
                    else
                    {
                        s.insert(D->getName().str());
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx




//*******************************************************************************  

// clang error can be mapped to get violations at conditions below 

// Examples: 

// struct stag a1 = { 0, 0 }; /* Compliant     */
// union  stag a2 = { 0, 0 }; /* Non-compliant - Constraint Violation in C99  */

// union abc{};
// enum abc{red,blue};	/* Non-compliant */

// struct elk
// {
//   uint16_t x;
// };

// struct elk      /* Non-compliant - Constraint violation in C99 */
// {
//  uint32_t x;
// };










