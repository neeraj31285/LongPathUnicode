//===--- 8.8.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------------------------------------------------------- 
// The static storage class specifier shall be used in all declarations of objects and functions that have internal linkage
// ---------------------------------------------------------------------------------------------------------------------------
//===----------------------------------------------------------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_8_8_H
#define CLANG_TIDY_MISRA_C_2012_RULE_8_8_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_8_8 : public EmboldTidyCheck {
		public:
			Rule_8_8(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_8_8_H


