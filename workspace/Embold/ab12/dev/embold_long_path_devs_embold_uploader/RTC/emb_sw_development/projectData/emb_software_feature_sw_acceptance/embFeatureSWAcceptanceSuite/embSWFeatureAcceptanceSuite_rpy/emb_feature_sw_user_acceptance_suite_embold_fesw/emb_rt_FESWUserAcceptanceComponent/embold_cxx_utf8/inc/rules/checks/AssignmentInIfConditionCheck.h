//===--- AssignmentInIfConditionCheck.h - clang-tidy ------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BUGPRONE_ASSIGNMENTINIFCONDITIONCHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BUGPRONE_ASSIGNMENTINIFCONDITIONCHECK_H

#include "EmboldTidyCheck.h"

namespace emboldCxx {

    /// Catches assignments within the condition clause of an if statement.
    ///
    /// For the user-facing documentation see:
    /// http://clang.llvm.org/extra/clang-tidy/checks/bugprone-assignment-in-if-condition.html
    /// 
    class AssignmentInIfConditionCheck : public EmboldTidyCheck {

    public:
        
        AssignmentInIfConditionCheck(llvm::StringRef Name, clang::tidy::ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {
        }
        
        void registerMatchers(clang::ast_matchers::MatchFinder* Finder) override;
        
        void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result) override;
    };

} // namespace emboldCxx

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_BUGPRONE_ASSIGNMENTINIFCONDITIONCHECK_H
