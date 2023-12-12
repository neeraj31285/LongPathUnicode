//===--- fixedSizeMemoryOperation.h -------------------------------*- C++ -*-===
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Memory Operations are called with a hard-coded length instead of using a macro or constant or sizeof().
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_fixedSizeMemoryOperation_H
#define CLANG_TIDY_MISRA_C_2012_RULE_fixedSizeMemoryOperation_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_fixedSizeMemoryOperation : public EmboldTidyCheck {
		public:
			Rule_fixedSizeMemoryOperation(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};
	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_fixedSizeMemoryOperation_H
