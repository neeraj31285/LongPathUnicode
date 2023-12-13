//===--- 11.4.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// An identifier with external linkage shall have exactly one external definition.
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_11_4_H
#define CLANG_TIDY_MISRA_C_2012_RULE_11_4_H

#include "EmboldTidyCheck.h"


namespace emboldCxx {
	namespace c2012 {

		class Rule_11_4 : public EmboldTidyCheck {
		public:
			Rule_11_4(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_11_4_H