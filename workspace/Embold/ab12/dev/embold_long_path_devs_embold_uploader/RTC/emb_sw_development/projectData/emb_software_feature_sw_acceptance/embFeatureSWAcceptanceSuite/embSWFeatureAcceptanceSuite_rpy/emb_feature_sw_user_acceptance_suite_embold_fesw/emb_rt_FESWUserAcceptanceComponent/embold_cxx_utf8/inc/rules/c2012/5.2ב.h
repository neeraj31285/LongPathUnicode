//===--- 5.2.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Bit-fields shall only be declared with an appropriate type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_5_2_H
#define CLANG_TIDY_MISRA_C_2012_RULE_5_2_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
    namespace c2012 {

        // Checker class for rule 5.2
        class Rule_5_2 : public EmboldTidyCheck {
        public:
            Rule_5_2(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
            void registerPPCallbacksImpl() override;
            void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
            void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
        };

        // Preprocessor callback class for rule 5.2
        class Rule_5_2_PPCallbacks : public clang::PPCallbacks {
        public:
            Rule_5_2_PPCallbacks(Rule_5_2& RuleChecker)
                : PPCallbacks(), RuleChecker(RuleChecker) {}

            virtual void EndOfMainFile() override;

        private:
            Rule_5_2& RuleChecker;
        };

    } // c2012
} // namespace emboldCxx

#endif // CLANG_TIDY_MISRA_C_2012_RULE_5_2_H
