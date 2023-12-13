//===--- 20.12.h - clang-tidy-misra ------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Bit-fields shall only be declared with an appropriate type
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_20_12_H
#define CLANG_TIDY_MISRA_C_2012_RULE_20_12_H

#include "EmboldTidyCheck.h"
#include <unordered_map>

namespace emboldCxx {
	namespace c2012 {

		class Rule_20_12;

		// Preprocessor callback class for rule 20.12
		class Rule_20_12_PPCallbacks : public clang::PPCallbacks {

			Rule_20_12& m_ruleChecker;

		public:

			Rule_20_12_PPCallbacks(Rule_20_12& pRuleChecker);
			void MacroDefined(const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;
			void MacroExpands(const clang::Token& MacroNameTok, const clang::MacroDefinition& MD,
				clang::SourceRange	Range, const clang::MacroArgs* Args) override;
		};

		// Checker class for rule 20.12
		class Rule_20_12 : public EmboldTidyCheck {

			std::unordered_map<clang::IdentifierInfo*, std::pair<clang::MacroInfo*, clang::SourceRange>> macroExapand_map;

		public:friend class Rule_20_12_PPCallbacks;

			  void registerPPCallbacksImpl() override;
			  Rule_20_12(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_20_12_H
