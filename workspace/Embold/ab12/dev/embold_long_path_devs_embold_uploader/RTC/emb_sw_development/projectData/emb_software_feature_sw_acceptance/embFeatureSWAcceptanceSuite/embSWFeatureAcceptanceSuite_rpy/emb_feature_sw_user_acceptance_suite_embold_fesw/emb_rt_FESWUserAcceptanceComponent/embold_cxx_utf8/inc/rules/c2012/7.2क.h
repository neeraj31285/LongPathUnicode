//===--- 7.2.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// A typedef name shall be a unique identifier
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_7_2_H
#define CLANG_TIDY_MISRA_C_2012_RULE_7_2_H

#include "EmboldTidyCheckæ.h"



namespace emboldCxx {
	namespace c2012 {

		class Rule_7_2 : public EmboldTidyCheck {
		public:
			Rule_7_2(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
			static llvm::StringRef GetSignedIntType(const clang::IntegerLiteral* D, const clang::SourceManager& SM);
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_7_2_H
