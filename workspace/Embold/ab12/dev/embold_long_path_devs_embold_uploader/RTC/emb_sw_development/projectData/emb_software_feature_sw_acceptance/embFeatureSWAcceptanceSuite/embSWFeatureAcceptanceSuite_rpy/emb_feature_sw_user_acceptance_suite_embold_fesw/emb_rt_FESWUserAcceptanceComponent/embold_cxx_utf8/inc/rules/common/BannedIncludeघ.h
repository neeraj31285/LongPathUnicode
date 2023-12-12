//===--- BannedInclude.h - clang-tidy-misra ---------------------*- C++ -*-===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef BANNED_INCLUDE_H
#define BANNED_INCLUDE_H

#include "clang/Lex/Preprocessor.h"
#include <string>
#include <unordered_set>

namespace emboldCxx {
    namespace common {

        template <typename Checker> class BannedInclude : public clang::PPCallbacks {
        public:
            using StringSet = std::unordered_set<std::string>;
            BannedInclude(Checker& RuleChecker, StringSet&& SetOfBannedIncludes)
                : clang::PPCallbacks(), RuleChecker(RuleChecker),
                SetOfBannedIncludes(SetOfBannedIncludes) {}

            virtual void InclusionDirective(clang::SourceLocation HashLoc,
                                            const clang::Token& IncludeTok,
                                            llvm::StringRef FileName,
                                            bool IsAngled,
                                            clang::CharSourceRange FilenameRange,
                                            const clang::FileEntry* File,
                                            llvm::StringRef SearchPath,
                                            llvm::StringRef RelativePath,
                                            const clang::Module* Imported,
                                            clang::SrcMgr::CharacteristicKind FileType) override {
                if (SetOfBannedIncludes.count(FileName.str()) > 0) {
                    RuleChecker.diag(HashLoc);
                }
            }

        private:
            Checker& RuleChecker;
            const StringSet SetOfBannedIncludes;
        };

    } // namespace common
} // namespace emboldCxx

#endif // BANNED_INCLUDE_H
