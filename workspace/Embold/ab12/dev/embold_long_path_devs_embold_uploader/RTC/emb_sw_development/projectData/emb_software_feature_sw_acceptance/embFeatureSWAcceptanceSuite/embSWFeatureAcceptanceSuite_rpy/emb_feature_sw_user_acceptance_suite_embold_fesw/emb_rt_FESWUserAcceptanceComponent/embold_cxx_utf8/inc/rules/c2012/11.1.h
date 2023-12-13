//===--- 11.1.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Conversions shall not be performed between a pointer to a function and any other type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_11_1_H
#define CLANG_TIDY_MISRA_C_2012_RULE_11_1_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_11_1 : public EmboldTidyCheck {
		public:
			Rule_11_1(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_11_1_H


