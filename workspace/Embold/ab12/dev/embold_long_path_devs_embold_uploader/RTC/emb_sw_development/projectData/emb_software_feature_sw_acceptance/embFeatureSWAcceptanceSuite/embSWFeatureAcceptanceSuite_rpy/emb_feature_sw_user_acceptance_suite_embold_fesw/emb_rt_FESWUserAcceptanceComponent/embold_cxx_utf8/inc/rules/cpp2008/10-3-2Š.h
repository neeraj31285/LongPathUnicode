//===--- 10-3-2.h - clang-tidy-misra ----------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_TIDY_MISRA_CPP_2008_RULE_10_3_2_H
#define CLANG_TIDY_MISRA_CPP_2008_RULE_10_3_2_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace cpp2008 {

		class Rule_10_3_2 : public EmboldTidyCheck {
		public:
			Rule_10_3_2(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			virtual void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			virtual void
				checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // cpp2008
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_CPP_2008_RULE_10_3_2_H
