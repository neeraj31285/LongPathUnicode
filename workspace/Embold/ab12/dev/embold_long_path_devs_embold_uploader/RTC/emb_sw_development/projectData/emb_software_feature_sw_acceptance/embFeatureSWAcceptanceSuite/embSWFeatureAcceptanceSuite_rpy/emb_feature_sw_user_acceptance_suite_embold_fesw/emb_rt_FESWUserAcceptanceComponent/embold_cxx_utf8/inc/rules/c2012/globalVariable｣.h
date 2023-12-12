#pragma once

#include "EmboldTidyCheckæ.h"

namespace emboldCxx {

	namespace c2012 {
		class Rule_globalVariable : public EmboldTidyCheck
		{
		public:
			Rule_globalVariable(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
		};
	}
}
