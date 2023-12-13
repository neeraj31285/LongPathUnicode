//===--- 21.10.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// The Standard Library time and date functions shall not be used
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//

#ifndef CLANG_TIDY_MISRA_C_2012_RULE_21_10_H
#define CLANG_TIDY_MISRA_C_2012_RULE_21_10_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_21_10 : public EmboldTidyCheck {
		public:
			Rule_21_10(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerPPCallbacksImpl() override;
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_21_10_H
