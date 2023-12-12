//===--- 21.6.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// The Standard Library input/output functions shall not be used
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_21_6_H
#define CLANG_TIDY_MISRA_C_2012_RULE_21_6_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_21_6 : public EmboldTidyCheck {
		public:
			Rule_21_6(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerPPCallbacksImpl() override;
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_21_6_H
