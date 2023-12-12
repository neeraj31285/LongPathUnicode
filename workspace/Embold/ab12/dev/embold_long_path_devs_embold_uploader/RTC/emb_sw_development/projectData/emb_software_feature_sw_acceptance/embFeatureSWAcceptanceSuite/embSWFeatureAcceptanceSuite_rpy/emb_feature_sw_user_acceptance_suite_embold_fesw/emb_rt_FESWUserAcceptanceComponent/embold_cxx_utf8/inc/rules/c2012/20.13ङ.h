//===--- 20.13.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Bit-fields shall only be declared with an appropriate type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_20_13_H
#define CLANG_TIDY_MISRA_C_2012_RULE_20_13_H

#include "EmboldTidyCheckæ.h"
#include <unordered_map>

namespace emboldCxx {
	namespace c2012 {

		class Rule_20_13;

		// Preprocessor callback class for rule 20.13
		class Rule_20_13_PPCallbacks : public clang::PPCallbacks {

			Rule_20_13& m_ruleChecker;

		public:

			Rule_20_13_PPCallbacks(Rule_20_13& pRuleChecker);
			void Endif(clang::SourceLocation Loc, clang::SourceLocation IfLoc) override;
		};

		// Checker class for rule 20.13
		class Rule_20_13 : public EmboldTidyCheck {

			std::vector<clang::SourceRange> m_sourceRanges;
			//std::unordered_map<SourceRange,SourceLocation &> m_map;

		public:friend class Rule_20_13_PPCallbacks;

			  void registerPPCallbacksImpl() override;
			  Rule_20_13(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_20_13_H
