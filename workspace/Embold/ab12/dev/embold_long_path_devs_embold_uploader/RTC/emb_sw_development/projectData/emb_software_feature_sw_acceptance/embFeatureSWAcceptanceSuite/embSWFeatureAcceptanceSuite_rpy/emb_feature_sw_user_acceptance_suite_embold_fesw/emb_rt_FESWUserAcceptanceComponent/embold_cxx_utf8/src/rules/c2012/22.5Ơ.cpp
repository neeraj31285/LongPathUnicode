//===--- 22.5.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "22.5क.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/Support/Path.h"
#include <iostream>

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;
        using namespace std;
        Rule_22_5::Rule_22_5(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_22_5::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::unaryOperator(hasOperatorName("*"),
                hasDescendant(declRefExpr(hasType(pointerType(pointee(asString("FILE")))))
                    .bind("DeclRefExpr"))), this);
        }

        void Rule_22_5::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const DeclRefExpr* D = Result.Nodes.getNodeAs<DeclRefExpr>("DeclRefExpr");
            if (isa<VarDecl>(D->getDecl())) {
                const VarDecl* varDecl = static_cast<const VarDecl*>(D->getDecl());
                const Type* varType = varDecl->getType().getTypePtrOrNull();
                if (varType
                    && varType->isPointerType()) {
                    const TagDecl* filerec = varType->getPointeeType()
                        .getTypePtr()->getAsTagDecl();
                    if (filerec) {
                        StringRef filePath = Result.Context->getSourceManager()
                            .getFilename(filerec->getBeginLoc()).str();
                        if (filePath.endswith("io.h")) {
                            diag(D->getSourceRange().getBegin());
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx



     /*if (isa<VarDecl>(D->getDecl())) {
            cout<<"In if\n";
            const VarDecl *varDecl = static_cast< const VarDecl *>(D->getDecl());
            cout << "var name: " << varDecl->getQualifiedNameAsString() << "\n";
            const TagDecl *filerec = varDecl->getType().getTypePtr()->getAsTagDecl();
            StringRef filePath;
            if (filerec) {
                cout<< "Found tag decl\n";
                filePath = Result.Context->getSourceManager().getFilename(filerec->getLocStart());
                cout<<"File: " << filePath.str();
            }
        }*/
