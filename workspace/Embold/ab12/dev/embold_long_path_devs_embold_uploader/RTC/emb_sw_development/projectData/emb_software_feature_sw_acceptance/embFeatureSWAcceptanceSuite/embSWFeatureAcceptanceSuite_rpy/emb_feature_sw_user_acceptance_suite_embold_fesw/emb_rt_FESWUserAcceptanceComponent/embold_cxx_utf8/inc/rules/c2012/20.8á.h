
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_20_8_H
#define CLANG_TIDY_MISRA_C_2012_RULE_20_8_H

#include <vector>
#include <unordered_map>
#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_20_8;

		// Preprocessor callback class for rule 20.8
		class Rule_20_8_PPCallbacks : public clang::PPCallbacks {

			Rule_20_8& m_ruleChecker;

		public:

			Rule_20_8_PPCallbacks(Rule_20_8& pRuleChecker);
			void If(clang::SourceLocation Loc, clang::SourceRange, ConditionValueKind) override;
			void Elif(clang::SourceLocation Loc, clang::SourceRange ConditionRange, ConditionValueKind ConditionValue, clang::SourceLocation IfLoc) override;
			void MacroDefined(const clang::Token& MacroNameTok, const clang::MacroDirective* MD) override;
		};


		// Checker class for rule 20.8
		class Rule_20_8 : public EmboldTidyCheck {

			std::vector<clang::SourceRange> m_sourceRanges;
			std::vector<clang::SourceLocation> m_sourceLocs;
			std::unordered_map<std::string, const clang::MacroDirective*> m_macroMap;

			const clang::MacroDirective* getMacroReplacementToken(const std::string& pMacroStr);

		public:	friend class Rule_20_8_PPCallbacks;

			  Rule_20_8(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			  void registerPPCallbacksImpl() override;
			  void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
			  virtual void onEndOfTranslationUnit() {}
		};

	} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_20_8_H
