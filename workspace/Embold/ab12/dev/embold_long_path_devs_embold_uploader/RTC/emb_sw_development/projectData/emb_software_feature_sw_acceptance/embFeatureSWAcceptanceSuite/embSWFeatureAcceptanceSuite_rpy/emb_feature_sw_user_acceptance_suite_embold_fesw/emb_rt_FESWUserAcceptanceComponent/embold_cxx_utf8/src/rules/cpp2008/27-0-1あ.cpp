//===--- 27-0-1.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "27-0-1ş.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "rules/common/BannedIncludeघ.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace cpp2008 {

        Rule_27_0_1::Rule_27_0_1(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_27_0_1::registerPPCallbacksImpl() {
            using BannedIncludePPCallback = common::BannedInclude<Rule_27_0_1>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedIncludePPCallback>(
                *this, BannedIncludePPCallback::StringSet{ "cstdio" }));
        }

    } // namespace cpp2008
} // namespace emboldCxx
