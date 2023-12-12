//===--- ruleOfThree.h -------------------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
// Rule text
// --------------------------------------------------------------------------
// Avoid using global variables
// --------------------------------------------------------------------------
//===----------------------------------------------------------------------===//
#ifndef CLANG_TIDY_MISRA_C_2012_RULE_dynamic_cast_H
#define CLANG_TIDY_MISRA_C_2012_RULE_dynamic_cast_H

#include "EmboldTidyCheckæ.h"



namespace emboldCxx {
namespace c2012 {

class Rule_dynamic_cast : public EmboldTidyCheck {
public:
  Rule_dynamic_cast(llvm::StringRef Name, clang::tidy::ClangTidyContext *Context);
  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override;
  void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};

} // c2012
} // namespace emboldCxx



#endif // CLANG_TIDY_MISRA_C_2012_RULE_dynamic_cast_H
