//===--- 21.6.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "21.6.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"
#include "rules/common/BannedInclude.h"
#include "rules/common/BannedMacro.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_21_6::Rule_21_6(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_21_6::registerPPCallbacksImpl() {
            using BannedIncludePPCallback = common::BannedInclude<Rule_21_6>;
            getPreprocessor().addPPCallbacks(std::make_unique<BannedIncludePPCallback>(
                *this, BannedIncludePPCallback::StringSet{ "stdio.h" }));
        }

        void Rule_21_6::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::callExpr(hasDeclaration(
                anyOf(functionDecl(hasName("printf")).bind("functionDecl"),
                    functionDecl(hasName("remove")).bind("functionDecl"),
                    functionDecl(hasName("rename")).bind("functionDecl"),
                    functionDecl(hasName("tmpfile")).bind("functionDecl"),
                    functionDecl(hasName("tmpnam")).bind("functionDecl"),
                    functionDecl(hasName("fflush")).bind("functionDecl"),
                    functionDecl(hasName("freopen")).bind("functionDecl"),
                    functionDecl(hasName("setbuf")).bind("functionDecl"),
                    functionDecl(hasName("setvbuf")).bind("functionDecl"),
                    functionDecl(hasName("fprintf")).bind("functionDecl"),
                    functionDecl(hasName("__isoc99_fscanf")).bind("functionDecl"),
                    functionDecl(hasName("fscanf")).bind("functionDecl"),
                    functionDecl(hasName("snprintf")).bind("functionDecl"),
                    functionDecl(hasName("sprintf")).bind("functionDecl"),
                    functionDecl(hasName("sscanf")).bind("functionDecl"),
                    functionDecl(hasName("vfprintf")).bind("functionDecl"),
                    functionDecl(hasName("vfscanf")).bind("functionDecl"),
                    functionDecl(hasName("vprintf")).bind("functionDecl"),
                    functionDecl(hasName("vscanf")).bind("functionDecl"),
                    functionDecl(hasName("vsnprintf")).bind("functionDecl"),
                    functionDecl(hasName("vsprintf")).bind("functionDecl"),
                    functionDecl(hasName("vsscanf")).bind("functionDecl"),
                    functionDecl(hasName("fgetc")).bind("functionDecl"),
                    functionDecl(hasName("fgets")).bind("functionDecl"),
                    functionDecl(hasName("fputc")).bind("functionDecl"),
                    functionDecl(hasName("fputs")).bind("functionDecl"),
                    functionDecl(hasName("getchar")).bind("functionDecl"),
                    functionDecl(hasName("putchar")).bind("functionDecl"),
                    functionDecl(hasName("puts")).bind("functionDecl"),
                    functionDecl(hasName("ungetc")).bind("functionDecl"),
                    functionDecl(hasName("fread")).bind("functionDecl"),
                    functionDecl(hasName("fwrite")).bind("functionDecl"),
                    functionDecl(hasName("fgetpos")).bind("functionDecl"),
                    functionDecl(hasName("fseek")).bind("functionDecl"),
                    functionDecl(hasName("fsetpos")).bind("functionDecl"),
                    functionDecl(hasName("ftell")).bind("functionDecl"),
                    functionDecl(hasName("rewind")).bind("functionDecl"),
                    functionDecl(hasName("clearerr")).bind("functionDecl"),
                    functionDecl(hasName("feof")).bind("functionDecl"),
                    functionDecl(hasName("ferror")).bind("functionDecl"),
                    functionDecl(hasName("perror")).bind("functionDecl"),
                    functionDecl(hasName("__isoc99_scanf")).bind("functionDecl"),
                    functionDecl(hasName("scanf")).bind("functionDecl"),
                    functionDecl(hasName("fopen")).bind("functionDecl"),
                    functionDecl(hasName("fclose")).bind("functionDecl")))).bind("callExpr"), this);
            Finder->addMatcher(ast_matchers::callExpr(hasDeclaration(anyOf(
                functionDecl(hasName("gets")),
                functionDecl(hasName("_IO_getc")),
                functionDecl(hasName("_IO_putc"))))).bind("callExpr_"), this);
        }

        void Rule_21_6::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {

            auto* E = Result.Nodes.getNodeAs<CallExpr>("callExpr");
            auto* C = Result.Nodes.getNodeAs<CallExpr>("callExpr_");
            const FunctionDecl* D = Result.Nodes.getNodeAs<FunctionDecl>("functionDecl");

            if (C != nullptr) {
                diag(C->getBeginLoc());
            }
            if (D != nullptr) {
                StringRef filePath = Result.Context->getSourceManager().getFilename(D->getSourceRange().getBegin());
                if (filePath.empty()) {
                    filePath =
                        Result.Context->getSourceManager().getFilename(D->getLocation());
                }
                StringRef fileName = llvm::sys::path::filename(filePath);
                if (fileName.equals("stdio.h")) {
                    diag(E->getBeginLoc());
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx