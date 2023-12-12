//===--- 6.1.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "6.1一.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include <vector>

using namespace llvm;
using namespace clang;
using namespace tidy;

namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;

        Rule_6_1::Rule_6_1(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        void Rule_6_1::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::fieldDecl(isBitField()).bind("FieldDecl"), this);
        }

        StringRef Rule_6_1::GetSignedIntType(const clang::NamedDecl* D, const clang::SourceManager& SM)
        {
            clang::SourceLocation SpellingLoc = SM.getExpansionLoc(D->getBeginLoc());
            std::pair<clang::FileID, unsigned int> dcmpdLocStart = SM.getDecomposedExpansionLoc(SpellingLoc);

            clang::DeclarationNameInfo NameInfo = clang::DeclarationNameInfo(D->getDeclName(), D->getLocation());
            clang::SourceLocation SpellingLocIdent = SM.getExpansionLoc(NameInfo.getBeginLoc());
            std::pair<clang::FileID, unsigned int> dcmpdLocStartIdent = SM.getDecomposedExpansionLoc(SpellingLocIdent);

            clang::FileID fileId = SM.getFileID(SpellingLoc);
            StringRef characterData2 = SM.getBufferData(fileId);

            StringRef nodeSignature = characterData2.substr(dcmpdLocStart.second, dcmpdLocStartIdent.second - dcmpdLocStart.second);

            nodeSignature = nodeSignature.ltrim("typedef");
            nodeSignature = nodeSignature.trim();

            return nodeSignature;
        }

        void Rule_6_1::checkImpl(
            const ast_matchers::MatchFinder::MatchResult& Result) {
            const FieldDecl* D = Result.Nodes.getNodeAs<FieldDecl>("FieldDecl");

            if (D != nullptr) {

                bool flag = true;
                StringRef explicit_signed;

                const TypedefType* typedefType = D->getType()->getAs<TypedefType>();
                if (typedefType != nullptr)
                {
                    explicit_signed = Rule_6_1::GetSignedIntType(D->getType()->getAs<TypedefType>()->getDecl(), D->getASTContext().getSourceManager());
                }
                else
                {
                    explicit_signed = Rule_6_1::GetSignedIntType(D, D->getASTContext().getSourceManager());
                }

                static std::vector< std::string > vect{ "signed int", "unsigned int", "_Bool" };
                for (unsigned int i = 0; i < vect.size(); i++)
                {
                    if ((vect[i] == D->getType().getCanonicalType().getAsString()) || (vect[i] == explicit_signed.str()))
                    {
                        flag = false;
                    }
                }

                if (flag)
                {
                    diag(D->getSourceRange().getBegin());
                }
            }
        }
    } // namespace c2012
} // namespace emboldCxx


