//===--- 18.7.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Flexible array members shall not be declared
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_18_7_H
#define CLANG_TIDY_MISRA_C_2012_RULE_18_7_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_18_7 : public EmboldTidyCheck {
		public:
			Rule_18_7(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_18_7_H

