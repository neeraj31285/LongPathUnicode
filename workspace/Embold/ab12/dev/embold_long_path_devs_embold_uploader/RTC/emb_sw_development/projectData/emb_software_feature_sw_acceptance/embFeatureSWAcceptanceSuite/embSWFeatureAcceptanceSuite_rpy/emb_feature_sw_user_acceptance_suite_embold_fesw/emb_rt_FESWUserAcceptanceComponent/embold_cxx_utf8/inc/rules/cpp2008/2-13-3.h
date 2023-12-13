//===--- 2-13-3.h - clang-tidy-misra ----------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_TIDY_MISRA_CPP_2008_RULE_2_13_3_H
#define CLANG_TIDY_MISRA_CPP_2008_RULE_2_13_3_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace cpp2008 {

		class Rule_2_13_3 : public EmboldTidyCheck {

		public:

			Rule_2_13_3(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);
			virtual void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
			virtual void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;

		private:
			bool isUnsignedIntegerLiteral(const clang::IntegerLiteral* il, const clang::ast_matchers::MatchFinder::MatchResult& Result);

			/// \brief Make sure the suffix 'U' exists.
			/// \param il Unsigned integer literal.
			void dealWithUnsignedInteger(const clang::IntegerLiteral* intLiteral, const clang::ast_matchers::MatchFinder::MatchResult& Result);
		};

	} // cpp2008
} // namespace emboldCxx

#endif // CLANG_TIDY_MISRA_CPP_2008_RULE_2_13_3_H
