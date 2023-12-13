//===--- 22.5.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// A pointer to a FILE object shall not be dereferenced
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_22_5_H
#define CLANG_TIDY_MISRA_C_2012_RULE_22_5_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_22_5 : public EmboldTidyCheck {
		public:
			Rule_22_5(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_22_5_H
