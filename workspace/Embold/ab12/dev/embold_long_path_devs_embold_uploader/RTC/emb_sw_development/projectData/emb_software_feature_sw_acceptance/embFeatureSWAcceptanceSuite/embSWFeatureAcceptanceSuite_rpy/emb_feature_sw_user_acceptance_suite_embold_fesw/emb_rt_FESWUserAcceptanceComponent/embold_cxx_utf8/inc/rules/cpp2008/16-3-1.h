//===--- 16-3-1.h - clang-tidy-misra ----------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef CLANG_TIDY_MISRA_CPP_2008_RULE_16_3_1_H
#define CLANG_TIDY_MISRA_CPP_2008_RULE_16_3_1_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {
	namespace cpp2008 {

		class Rule_16_3_1 : public EmboldTidyCheck {
		public:
			Rule_16_3_1(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context);

		protected:
			virtual void registerPPCallbacksImpl() override;
		};

	} // cpp2008
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_CPP_2008_RULE_16_3_1_H
