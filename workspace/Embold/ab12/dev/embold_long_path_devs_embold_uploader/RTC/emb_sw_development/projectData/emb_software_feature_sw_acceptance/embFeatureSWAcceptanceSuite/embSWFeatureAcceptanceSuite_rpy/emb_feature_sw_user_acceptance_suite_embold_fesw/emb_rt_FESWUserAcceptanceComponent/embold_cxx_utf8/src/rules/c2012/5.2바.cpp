//===--- 5.2.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "5.2.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace clang;
using namespace tidy;
namespace emboldCxx {
    namespace c2012 {

        using namespace ast_matchers;
        using namespace llvm;
        using namespace std;

        const static uint16_t C90_SIGNIFICANT_IDENTIFIER_LENGTH = 31;
        static uint16_t currScopeLevel = 1;
        typedef std::list<std::pair<clang::NamedDecl*, uint16_t>> TyIdentifierList;
        typedef std::vector<std::set<NamedDecl*>> TyDiagIdentifierList;
        static TyIdentifierList identifierList;
        static TyDiagIdentifierList matchingIdentifiers;

        //-------------------------------------------
        // triggers when the TU ends
        void Rule_5_2_PPCallbacks::EndOfMainFile() {
            identifierList.clear();
            matchingIdentifiers.clear();
        }
        //--------------------------------------------

        class Rule_5_2_ASTVisitor : public RecursiveASTVisitor<Rule_5_2_ASTVisitor> {
            friend class RecursiveASTVisitor<Rule_5_2_ASTVisitor>;

        public:
            Rule_5_2_ASTVisitor(const clang::TranslationUnitDecl* TUDecl) : TU(TUDecl) {}

            void TraverseTU() {
                TraverseTranslationUnitDecl(const_cast<clang::TranslationUnitDecl*>(TU));
                removeDeclsInScope(currScopeLevel);
                // printIdentifierList();
            }

        private:
            bool TraverseCompoundStmt(CompoundStmt* S) {
                currScopeLevel += 1;
                // fprintf(stderr, "Entered scope: %u\n", currScopeLevel);
                RecursiveASTVisitor::TraverseCompoundStmt(S);
                removeDeclsInScope(currScopeLevel);
                // fprintf(stderr, "Leaving scope: %u\n", currScopeLevel);
                currScopeLevel -= 1;
                return true;
            }

            bool TraverseFunctionDecl(FunctionDecl* D) {
                if (D->isThisDeclarationADefinition()) {
                    currScopeLevel += 1;
                    // fprintf(stderr, "Entered function scope: %u\n", currScopeLevel);
                    for (clang::ParmVarDecl* P : D->parameters()) {
                        RecursiveASTVisitor::TraverseParmVarDecl(P);
                    }
                    RecursiveASTVisitor::TraverseCompoundStmt(
                        static_cast<clang::CompoundStmt*>(D->getBody()));
                    removeDeclsInScope(currScopeLevel);
                    // fprintf(stderr, "Leaving function scope: %u\n", currScopeLevel);
                    // printIdentifierList();
                    currScopeLevel -= 1;
                    return true;
                }
            }

            bool TraverseRecordDecl(RecordDecl* D) {
                currScopeLevel += 1;
                // fprintf(stderr, "Entered record scope: %u\n", currScopeLevel);)
                RecursiveASTVisitor::TraverseRecordDecl(D);
                removeDeclsInScope(currScopeLevel);
                // LOG(fprintf(stderr, "Leaving record scope: %u\n", currScopeLevel);)
                currScopeLevel -= 1;
                return true;
            }

            bool TraverseTypedefDecl(TypedefDecl* D) {
                currScopeLevel += 1;
                // LOG(fprintf(stderr, "Entered typedef scope: %u\n", currScopeLevel);)
                RecursiveASTVisitor::TraverseTypedefDecl(D);
                removeDeclsInScope(currScopeLevel);
                // LOG(fprintf(stderr, "Leaving typedef scope: %u\n", currScopeLevel);)
                currScopeLevel -= 1;
                return true;
            }

            bool TraverseForStmt(ForStmt* S) {
                currScopeLevel += 1;
                // fprintf(stderr, "Entered for scope: %u\n", currScopeLevel);
                RecursiveASTVisitor::TraverseStmt(S->getInit());
                if (clang::isa<clang::CompoundStmt>(S->getBody())) {
                    RecursiveASTVisitor::TraverseCompoundStmt(
                        static_cast<clang::CompoundStmt*>(S->getBody()));
                }
                else {
                    RecursiveASTVisitor::TraverseStmt(S->getBody());
                }
                removeDeclsInScope(currScopeLevel);
                // fprintf(stderr, "Leaving for scope: %u\n", currScopeLevel);
                currScopeLevel -= 1;
                return true;
            }

            bool TraverseWhileStmt(WhileStmt* S) {
                currScopeLevel += 1;
                // fprintf(stderr, "Entered for scope: %u\n", currScopeLevel);
                RecursiveASTVisitor::TraverseStmt(S->getCond());
                if (clang::isa<clang::CompoundStmt>(S->getBody())) {
                    RecursiveASTVisitor::TraverseCompoundStmt(
                        static_cast<clang::CompoundStmt*>(S->getBody()));
                }
                else {
                    RecursiveASTVisitor::TraverseStmt(S->getBody());
                }
                removeDeclsInScope(currScopeLevel);
                // fprintf(stderr, "Leaving for scope: %u\n", currScopeLevel);
                currScopeLevel -= 1;
                return true;
            }

            bool TraverseEnumDecl(EnumDecl* D) {
                currScopeLevel += 1;
                // fprintf(stderr, "Entered enum scope: %u\n", currScopeLevel);
                RecursiveASTVisitor::TraverseEnumDecl(D);
                removeDeclsInScope(currScopeLevel);
                // fprintf(stderr, "Leaving enum scope: %u\n", currScopeLevel);
                currScopeLevel -= 1;
                return true;
            }

            bool VisitNamedDecl(NamedDecl* D) {
                if (!isThisDeclInSystemHeader(D)) {
                    identifierList.push_back(
                        std::pair<clang::NamedDecl*, uint16_t>(D, currScopeLevel));
                }
                return true;
            }

            static void printIdentifierList() {
                fprintf(stderr, "Identifier list:\n");
                for (std::pair<NamedDecl*, uint16_t> prevDecl : identifierList) {
                    NamedDecl* prevDeclName = prevDecl.first;
                    uint16_t prevDeclLevel = prevDecl.second;
                    fprintf(stderr, "\t%s->%u\n", prevDeclName->getName().str().c_str(),
                        prevDeclLevel);
                }
            }

            static bool isEqualTillLength(StringRef str1, StringRef str2,
                uint16_t significantLength) {
                if (str1.size() > significantLength && str2.size() > significantLength) {
                    StringRef sigStr1 = str1.take_front(significantLength);
                    StringRef sigStr2 = str2.take_front(significantLength);
                    return sigStr1 == sigStr2;
                }
                else {
                    return str1 == str2;
                }
            }

            static bool isInternalIdentifier(NamedDecl* D) {
                bool isInternal = false;
                if (D->getDeclContext()->isTranslationUnit()) {
                    if (clang::isa<VarDecl>(D)) {
                        clang::VarDecl* VD = static_cast<clang::VarDecl*>(D);
                        if (StorageClass::SC_Static == VD->getStorageClass()) {
                            isInternal = true;
                        }
                    }
                    if (clang::isa<FunctionDecl>(D)) {
                        clang::FunctionDecl* FD = static_cast<clang::FunctionDecl*>(D);
                        if (StorageClass::SC_Static == FD->getStorageClass()) {
                            isInternal = true;
                        }
                    }
                }
                else {
                    isInternal = true;
                }
                return isInternal;
            }

            static std::set<NamedDecl*>
                getMatchingDeclsInScope(NamedDecl* D, uint16_t currScopeLevel) {
                auto remStartIter = identifierList.begin();
                std::set<NamedDecl*> diagIdentifiers;
                while (remStartIter != identifierList.end()) {
                    bool eraseThis = false;
                    std::pair<NamedDecl*, uint16_t> prevDecl = *remStartIter;
                    if (prevDecl.second == currScopeLevel) {
                        NamedDecl* prevDeclName = prevDecl.first;
                        if (isInternalIdentifier(D) || isInternalIdentifier(prevDeclName)) {
                            StringRef sourceIdentifier = D->getName();
                            StringRef targetIdentifier = prevDeclName->getName();
                            if (isEqualTillLength(sourceIdentifier, targetIdentifier,
                                C90_SIGNIFICANT_IDENTIFIER_LENGTH)) {
                                diagIdentifiers.insert(D);
                                diagIdentifiers.insert(prevDeclName);
                                eraseThis = true;
                            }
                            //else {
                            //  fprintf(stderr, "%s != %s\n", sourceIdentifier.str().c_str(),
                            //          targetIdentifier.str().c_str());
                            //}
                        }
                        //else {
                        //  fprintf(stderr, "External identifiers\n");
                        //}
                    }
                    //else {
                    //  fprintf(stderr, "Not in same scope\n");
                    //}

                    if (eraseThis) {
                        remStartIter = identifierList.erase(remStartIter);
                    }
                    else {
                        ++remStartIter;
                    }
                }
                return diagIdentifiers;
            }

            static void removeDeclsInScope(uint16_t currScopeLevel) {
                // printIdentifierList();
                if (identifierList.empty()) {
                    return;
                }
                const std::pair<NamedDecl*, uint16_t> lastDecl = identifierList.back();
                if (lastDecl.second == currScopeLevel) {
                    identifierList.pop_back();
                    NamedDecl* prevDeclName = lastDecl.first;
                    // fprintf(stderr, "Removed identifier: %s->%u\n",
                    //        prevDeclName->getName().str().c_str(), currScopeLevel);
                    std::set<NamedDecl*> diagIdentifiers = getMatchingDeclsInScope(prevDeclName, currScopeLevel);
                    if (diagIdentifiers.size() > 0) {
                        matchingIdentifiers.push_back(diagIdentifiers);
                        // for (NamedDecl *D : diagIdentifiers) {
                        //  fprintf(stderr, "Matching identifier: %s->%u\n",
                        //          D->getName().str().c_str(), currScopeLevel);
                        // }
                    }
                    //else {
                    //  fprintf(stderr, "No matching identifier found\n");
                    //}
                    // recursive call
                    // will break when no identifiers found in current scope
                    removeDeclsInScope(currScopeLevel);
                }
            }

            static bool isThisDeclInSystemHeader(NamedDecl* D) {
                clang::SourceManager& SM = D->getASTContext().getSourceManager();
                if (SM.isInSystemHeader(D->getBeginLoc()) ||
                    SM.isInSystemMacro(D->getBeginLoc())) {
                    return true;
                }
                else {
                    return false;
                }
            }

            static bool isThisDeclInMainFile(NamedDecl* D) {
                clang::SourceManager& SM = D->getASTContext().getSourceManager();
                if (SM.isInMainFile(D->getBeginLoc())) {
                    return true;
                }
                else {
                    return false;
                }
            }

            const clang::TranslationUnitDecl* TU;
        };

        static void LogAST(Decl* D) {
            std::string astStr;
            llvm::raw_string_ostream ss(astStr);
            D->dump(ss);
            std::string logMsg = "AST: " + ss.str();
            fprintf(stderr, logMsg.c_str());
        }

        Rule_5_2::Rule_5_2(llvm::StringRef Name, ClangTidyContext* Context)
            : EmboldTidyCheck(Name, Context) {}

        // Register callback for end of main file (TU)
        void Rule_5_2::registerPPCallbacksImpl() {
            getPreprocessor().addPPCallbacks(
                std::make_unique<Rule_5_2_PPCallbacks>(*this));
        }

        void Rule_5_2::registerMatchers(ast_matchers::MatchFinder* Finder) {
            Finder->addMatcher(ast_matchers::translationUnitDecl().bind("TUDecl"), this);
        }

        void Rule_5_2::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
            const TranslationUnitDecl* TU =
                Result.Nodes.getNodeAs<TranslationUnitDecl>("TUDecl");
            Rule_5_2_ASTVisitor(TU).TraverseTU();
            for (std::set<NamedDecl*> dlist : matchingIdentifiers) {
                for (NamedDecl* D : dlist) {
                    diag(D->getBeginLoc());
                    for (NamedDecl* alias : dlist) {
                        if (D != alias) {
                            std::string noteMsg = "matches with ";
                            if (alias->getName().size() > C90_SIGNIFICANT_IDENTIFIER_LENGTH) {
                                noteMsg = "matches first " +
                                    std::to_string(C90_SIGNIFICANT_IDENTIFIER_LENGTH) +
                                    " case-sensitive characters with ";
                            }
                            noteMsg += "'" + alias->getName().str() + "'";
                            ClangTidyCheck::diag(alias->getBeginLoc(), noteMsg,
                                DiagnosticIDs::Note);
                        }
                    }
                }
            }
        }

    } // namespace c2012
} // namespace emboldCxx


