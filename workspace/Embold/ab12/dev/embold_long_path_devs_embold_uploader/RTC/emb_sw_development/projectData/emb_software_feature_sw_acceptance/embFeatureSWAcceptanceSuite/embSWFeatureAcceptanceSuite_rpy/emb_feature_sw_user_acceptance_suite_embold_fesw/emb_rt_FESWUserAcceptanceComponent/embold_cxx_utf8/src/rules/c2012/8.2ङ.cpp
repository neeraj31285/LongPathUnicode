//===--- 8.2.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "8.2.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_8_2::Rule_8_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_8_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(functionDecl().bind("FunctionDecl"), this);
            Finder->addMatcher(loc(functionType()).bind("funcTypeLoc"), this);
            Finder->addMatcher(varDecl().bind("VarDecl"), this);
        }

        void Rule_8_2::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const FunctionDecl* FD = Result.Nodes.getNodeAs<FunctionDecl>("FunctionDecl");
            const TypeLoc* TL = Result.Nodes.getNodeAs<TypeLoc>("funcTypeLoc");
            const VarDecl* VD = Result.Nodes.getNodeAs<VarDecl>("VarDecl");

            if (TL)
            {
                if (TL->getSourceRange().isValid())
                {
                    FunctionTypeLoc FTL = TL->getAs<FunctionTypeLoc>();
                    if (!FTL.isNull())
                    {
                        if (FTL.getSourceRange().isValid())
                        {
                            unsigned int numParams = FTL.getNumParams();
                            for (unsigned int i = 0; i < numParams; ++i)
                            {
                                ParmVarDecl* param = FTL.getParam(i);
                                if (param != nullptr)
                                {
                                    std::string name = param->getNameAsString();
                                    if (name.empty())
                                    {
                                        diag(FTL.getSourceRange().getBegin());
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (FD != nullptr)
            {
                if (!FD->isInvalidDecl() && !FD->isImplicit())
                {
                    clang::Decl::Kind kind = FD->getKind();
                    if (kind != clang::Decl::Kind::Empty)
                    {
                        if (!FD->hasPrototype())
                        {
                            diag(FD->getSourceRange().getBegin());
                        }
                    }
                }
            }

            if (VD)
            {
                if (!VD->isInvalidDecl() && !VD->isImplicit())
                {
                    const TypeSourceInfo* TSI = VD->getTypeSourceInfo();
                    if (TSI)
                    {
                        const Type* type = TSI->getType().getTypePtrOrNull();
                        if (type)
                        {
                            if (type->isFunctionPointerType())
                            {
                                const FunctionType* FT = VD->getFunctionType();
                                if (FT)
                                {
                                    if (FT->isFunctionNoProtoType())
                                    {
                                        diag(VD->getSourceRange().getBegin());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx

