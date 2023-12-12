//===--- 9.3.h - clang-tidy-misra -------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// --------------------------------------------------------------------------
// Rule 9.3
// Arrays shall not be partially initializ ed
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//

#ifndef CLANG_TIDY_MISRA_C_2012_RULE_9_3_H
#define CLANG_TIDY_MISRA_C_2012_RULE_9_3_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_9_3 : public EmboldTidyCheck {
		public:
			Rule_9_3(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_9_3_H
