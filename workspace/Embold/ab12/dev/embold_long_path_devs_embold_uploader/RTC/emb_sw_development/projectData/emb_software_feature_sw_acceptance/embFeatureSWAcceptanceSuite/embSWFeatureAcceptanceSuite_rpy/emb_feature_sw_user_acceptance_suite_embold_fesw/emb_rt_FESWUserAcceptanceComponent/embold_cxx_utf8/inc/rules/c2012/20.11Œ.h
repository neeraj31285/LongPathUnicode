//===--- 20.11.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Bit-fields shall only be declared with an appropriate type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_20_11_H
#define CLANG_TIDY_MISRA_C_2012_RULE_20_11_H

#include "EmboldTidyCheckæ.h"
#include <unordered_map>

namespace emboldCxx {
	namespace c2012 {

		class Rule_20_11;

		// Preprocessor callback class for rule 20.11
		class Rule_20_11_PPCallbacks : public clang::PPCallbacks {

			Rule_20_11& m_ruleChecker;

		public:

			Rule_20_11_PPCallbacks(Rule_20_11& pRuleChecker);
			void MacroDefined(const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;
		};

		// Checker class for rule 20.11
		class Rule_20_11 : public EmboldTidyCheck {

			std::vector<clang::SourceRange> m_sourceRanges;
			std::vector<clang::SourceLocation> m_map;

		public:friend class Rule_20_11_PPCallbacks;

			  void registerPPCallbacksImpl() override;
			  Rule_20_11(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_20_11_H
