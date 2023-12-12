//===--- 17.7.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// The value returned by a function having non-void return type shall be used
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_17_7_H
#define CLANG_TIDY_MISRA_C_2012_RULE_17_7_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_17_7 : public EmboldTidyCheck {
		public:
			Rule_17_7(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_17_7_H


