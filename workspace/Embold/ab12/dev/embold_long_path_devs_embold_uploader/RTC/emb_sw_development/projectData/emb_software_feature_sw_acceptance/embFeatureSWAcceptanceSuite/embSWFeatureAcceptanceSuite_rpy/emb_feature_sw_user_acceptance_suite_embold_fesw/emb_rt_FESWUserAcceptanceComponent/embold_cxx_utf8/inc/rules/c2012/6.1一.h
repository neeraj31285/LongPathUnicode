//===--- 6.1.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Bit-fields shall only be declared with an appropriate type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_6_1_H
#define CLANG_TIDY_MISRA_C_2012_RULE_6_1_H

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_6_1 : public EmboldTidyCheck {
		public:
			Rule_6_1(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
			static llvm::StringRef GetSignedIntType(const clang::NamedDecl* D, const clang::SourceManager& SM);
		};

	} // c2012
} // namespace emboldCxx

#endif // CLANG_TIDY_MISRA_C_2012_RULE_6_1_H
