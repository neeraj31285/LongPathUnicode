//===--- ForbiddenMacro.h - clang-tidy-misra --------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BANNED_MACRO_H
#define BANNED_MACRO_H

#include "clang/Lex/Preprocessor.h"
#include <string>
#include <unordered_set>

namespace emboldCxx {
    namespace common {

        template <typename Checker> class BannedMacro : public clang::PPCallbacks {
        public:
            using StringSet = std::unordered_set<std::string>;
            BannedMacro(Checker& RuleChecker, StringSet&& BannedMacros)
                : PPCallbacks(), RuleChecker(RuleChecker), BannedMacroSet(BannedMacros) {}

            virtual void MacroExpands(const clang::Token& MacroNameTok,
                const clang::MacroDefinition& MD, clang::SourceRange Range,
                const clang::MacroArgs* Args) override {
                const std::string& macroName = MacroNameTok.getIdentifierInfo()->getName().str();
                if (BannedMacroSet.count(macroName)) {
                    RuleChecker.diag(Range.getBegin());
                }
            }

        private:
            Checker& RuleChecker;
            const StringSet BannedMacroSet;
        };

    } // namespace common
} // namespace emboldCxx



#endif // BANNED_MACRO_H
