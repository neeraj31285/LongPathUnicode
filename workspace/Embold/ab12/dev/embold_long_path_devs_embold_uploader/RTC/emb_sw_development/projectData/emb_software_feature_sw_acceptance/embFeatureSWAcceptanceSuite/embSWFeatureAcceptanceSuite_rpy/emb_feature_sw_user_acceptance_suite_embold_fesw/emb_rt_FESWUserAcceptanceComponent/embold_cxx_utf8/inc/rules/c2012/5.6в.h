
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_5_6_H
#define CLANG_TIDY_MISRA_C_2012_RULE_5_6_H

#include <iostream>
#include <unordered_map>
#include "EmboldTidyCheckæ.h"

namespace emboldCxx {
	namespace c2012 {

		class Rule_5_6 : public EmboldTidyCheck {

			std::unordered_map<std::string, const clang::NamedDecl*> typedef_map;
			std::unordered_map<std::string, const clang::NamedDecl*> nontypedef_map;

		public:

			Rule_5_6(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void onEndOfTranslationUnit() override;
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};

	} // namespace c2012
} // namespace emboldCxx

#endif //CLANG_TIDY_MISRA_C_2012_RULE_5_6_H
