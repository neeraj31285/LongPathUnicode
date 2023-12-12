//===--- deactivatedCode.h - clang-tidy-misra ------------------------------
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
//===------------------------------------------------------------------------
#ifndef CLANG_TIDY_MISRA_C_2012_DEACTIVATED_CODE_H
#define CLANG_TIDY_MISRA_C_2012_DEACTIVATED_CODE_H

#include <vector>
#include <unordered_map>
#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_deactivatedCode;

		// Preprocessor callback class for rule 20.8
		class Rule_deactivatedCode_PPCallbacks : public clang::PPCallbacks {

			Rule_deactivatedCode& m_ruleChecker;

		public:

			Rule_deactivatedCode_PPCallbacks(Rule_deactivatedCode& pRuleChecker);
			void If(clang::SourceLocation Loc, clang::SourceRange, ConditionValueKind) override;
			void Elif(clang::SourceLocation Loc, clang::SourceRange ConditionRange, ConditionValueKind ConditionValue, clang::SourceLocation IfLoc) override;
			void MacroDefined(const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;
		};


		// Checker class for rule 20.8
		class Rule_deactivatedCode : public EmboldTidyCheck {

			std::vector<clang::SourceRange> m_sourceRanges;
			std::vector<clang::SourceLocation> m_sourceLocs;
			std::unordered_map<std::string, const clang::MacroDirective*> m_macroMap;

			const clang::MacroDirective* getMacroReplacementToken(const std::string& pMacroStr);

		public:	friend class Rule_deactivatedCode_PPCallbacks;

			  Rule_deactivatedCode(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			  void registerPPCallbacksImpl() override;
			  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
			  virtual void onEndOfTranslationUnit() {}
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_DEACTIVATED_CODE_H
