#include "globalVariable.h"

#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

using namespace clang;
using namespace clang::tidy;
using namespace clang::ast_matchers;

namespace emboldCxx {
    namespace c2012 {

        Rule_globalVariable::Rule_globalVariable(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {
        }

        void Rule_globalVariable::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::varDecl(hasGlobalStorage(), unless(isStaticStorageClass()), unless(hasDeclContext(recordDecl()))).bind("VarDecl"), this);
        }

        void Rule_globalVariable::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const VarDecl* D = Result.Nodes.getNodeAs<VarDecl>("VarDecl");
            if (D != nullptr)
            {
                if (D->getSourceRange().isValid())
                {
                    diag(D->getSourceRange().getBegin());
                }
            }
        }
    }
}