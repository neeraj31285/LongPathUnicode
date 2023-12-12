//===--- 7.2.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "7.2क.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;

namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_7_2::Rule_7_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_7_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::integerLiteral().bind("IntegerLiteral"), this);
        }


        llvm::Optional<SourceLocation> GetMacroAwareLocation(SourceLocation Loc, const SourceManager& SM)
        {
            if (Loc.isInvalid())
            {
                return llvm::None;
            }

            // Look where the location was *actually* written.
            SourceLocation SpellingLoc = SM.getSpellingLoc(Loc);
            if (SpellingLoc.isInvalid())
            {
                return llvm::None;
            }
            return SpellingLoc;
        }

        llvm::Optional<SourceRange> GetMacroAwareSourceRange(SourceRange Loc, const SourceManager& SM)
        {
            llvm::Optional<SourceLocation> Begin = GetMacroAwareLocation(Loc.getBegin(), SM);
            llvm::Optional<SourceLocation> End = GetMacroAwareLocation(Loc.getEnd(), SM);

            if (!Begin || !End)
            {
                return llvm::None;
            }
            return SourceRange(*Begin, *End);
        }

        StringRef Rule_7_2::GetSignedIntType(const clang::IntegerLiteral* D, const clang::SourceManager& SM)
        {
            llvm::Optional<SourceRange> Range = GetMacroAwareSourceRange(D->getSourceRange(), SM);		// To handle case where integerLiteral is in Macro
            const StringRef LiteralSourceText = Lexer::getSourceText(CharSourceRange::getTokenRange(*Range), SM, LangOptions());
            return LiteralSourceText;
        }

        void Rule_7_2::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const IntegerLiteral* D = Result.Nodes.getNodeAs<IntegerLiteral>("IntegerLiteral");

            if (D != nullptr) {
                //if(D->getType()->isSignedIntegerType())
                if (D->getType()->hasUnsignedIntegerRepresentation())
                {
                    StringRef Literal = Rule_7_2::GetSignedIntType(D, Result.Context->getSourceManager()).str();

                    if ((Literal.endswith_insensitive("U") == 0) && (Literal.endswith_insensitive("UL") == 0) && (Literal.endswith_insensitive("ULL") == 0))
                    {
                        diag(D->getSourceRange().getBegin());
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx


