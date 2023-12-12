//===--- 18-2-1.cpp - clang-tidy-misra ------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "18-2-1ń.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "rules/common/BannedMacroş.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace cpp2008 {

        Rule_18_2_1::Rule_18_2_1(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_18_2_1::registerPPCallbacksImpl() {
            using BannedMacroPPCallback = common::BannedMacro<Rule_18_2_1>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedMacroPPCallback>(
                *this, BannedMacroPPCallback::StringSet{ "offsetof" }));
        }

    } // namespace cpp2008
} // namespace emboldCxx
