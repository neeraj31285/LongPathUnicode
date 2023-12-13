//===--- dynamic_cast.cpp - clang-tidy-misra ---------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "dynamic_cast.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
namespace c2012 {

using namespace ast_matchers;

Rule_dynamic_cast::Rule_dynamic_cast(llvm::StringRef Name, ClangTidyContext *Context)
    : EmboldTidyCheck(Name, Context) {}

void Rule_dynamic_cast::registerMatchers(ast_matchers::MatchFinder *Finder) {
  Finder->addMatcher(ast_matchers::stmt(cxxDynamicCastExpr().bind("CXXDynamicCastExpr")),this);
}

void Rule_dynamic_cast::checkImpl(const ast_matchers::MatchFinder::MatchResult &Result) {
const CXXDynamicCastExpr *D = Result.Nodes.getNodeAs<CXXDynamicCastExpr>("CXXDynamicCastExpr");

  if(D != nullptr )
  {
    if(D->getSourceRange().isValid() == 1)
    {
      diag(D->getSourceRange().getBegin());
    }
  }
}

} // namespace c2012
} // namespace emboldCxx

