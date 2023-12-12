
#include "EmboldTidyCheckæ.h"
#include "RuleHeadlineTextsഘ.h"
#include "clang/Lex/Preprocessor.h"
#include <iostream>

namespace emboldCxx {

    using namespace clang;
    using namespace clang::tidy;

    EmboldTidyCheck::EmboldTidyCheck(llvm::StringRef pCheckName, clang::tidy::ClangTidyContext* pContext)
        : ClangTidyCheck(pCheckName, pContext), m_checkName(pCheckName),
        IgnoreInvalidLocations(Options.get("IgnoreInvalidLocations", true)),
        IgnoreBuiltInLocations(Options.get("IgnoreBuiltInLocations", true)),
        IgnoreCommandLineLocations(Options.get("IgnoreCommandLineLocations", false)) {
    }


    void EmboldTidyCheck::registerPPCallbacksImpl() {
    }


    void EmboldTidyCheck::checkImpl(const ast_matchers::MatchFinder::MatchResult&) {
    }


    bool EmboldTidyCheck::isC() const {
        return (PP->getLangOpts().C99 || PP->getLangOpts().C11);
    }


    bool EmboldTidyCheck::isCPlusPlus() const {
        return PP->getLangOpts().CPlusPlus;
    }


    Preprocessor& EmboldTidyCheck::getPreprocessor() const {
        assert(PP && "No preprocessor!");
        return *PP;
    }


    void EmboldTidyCheck::check(const ast_matchers::MatchFinder::MatchResult& Result) {
        if (checkerIsActive()) {
            //std::cout << "\n[Running] " << m_checkName;
            checkImpl(Result);
        }
    }


    void EmboldTidyCheck::diag(SourceLocation Loc, DiagnosticIDs::Level Level/* = clang::DiagnosticIDs::Warning*/) {
        if (!isIgnored(Loc)) {
            ClangTidyCheck::diag(Loc, ruleHeadlines.at(m_checkName), Level);
        }
    }


    void EmboldTidyCheck::registerPPCallbacks(const SourceManager& SM,
        Preprocessor* PP,
        Preprocessor* ModuleExpanderPP) {
        this->PP = PP;
        if (checkerIsActive()) {
            registerPPCallbacksImpl();
        }
    }


    void EmboldTidyCheck::storeOptions(ClangTidyOptions::OptionMap& Opts) {
        Options.store(Opts, "IgnoreInvalidLocations", IgnoreInvalidLocations);
        Options.store(Opts, "IgnoreBuiltInLocations", IgnoreBuiltInLocations);
        Options.store(Opts, "IgnoreCommandLineLocations", IgnoreCommandLineLocations);
    }


    std::string EmboldTidyCheck::srcLocToTokenString(const SourceManager& SM, const SourceLocation start) {
        const LangOptions LO = PP->getLangOpts();
        const SourceLocation spellLoc = SM.getSpellingLoc(start);
        unsigned tokenLength = Lexer::MeasureTokenLength(spellLoc, SM, LO);
        return std::string(SM.getCharacterData(spellLoc), SM.getCharacterData(spellLoc) + tokenLength);
    }


    bool EmboldTidyCheck::isIgnored(clang::SourceLocation loc) {
        if (loc.isInvalid()) {
            return IgnoreInvalidLocations;
        }
        if (loc.isFileID()) {
            return false;
        }
        /* if (isBuiltIn(loc)) {
           return IgnoreBuiltInLocations;
         }
         if (isCommandLine(loc)) {
           return IgnoreCommandLineLocations;
         }*/
        return false;
    }


    bool EmboldTidyCheck::checkerIsActive() const {
        using std::string;

        assert((isCPlusPlus() || isC()) && "Language must be either C or C++!");

        assert(((this->m_checkName.find("misra-c2012-") != string::npos) ||
            (this->m_checkName.find("misra-cpp2008-") != string::npos) ||
            (this->m_checkName.find("global-variable") != string::npos) ||
            (this->m_checkName.find("empty-catch") != string::npos) ||
            (this->m_checkName.find("deactivated-code") != string::npos) ||
            (this->m_checkName.find("fixed-size-memory-operation") != string::npos) ||
            (this->m_checkName.find("shallow-copy-construct") != string::npos) ||
            (this->m_checkName.find("shallow-copy-assignment") != string::npos) ||
            (this->m_checkName.find("bugprone-assignment-in-if-condition") != string::npos)) &&
            "Rule must belong either to MISRA C++:2008 or to C: 2012");

        const bool enableMisraCpp2008 =
            isCPlusPlus() && this->m_checkName.find("misra-cpp2008-") != string::npos;
        const bool enableMisraC2012 =
            isC() && this->m_checkName.find("misra-c2012-") != string::npos;
        const bool enableGlobalVariable =
            this->m_checkName.find("global-variable") != string::npos;
        const bool enableEmptyCatch =
            this->m_checkName.find("empty-catch") != string::npos;
        const bool enableDeactivatedCode =
            this->m_checkName.find("deactivated-code") != string::npos;
        const bool enableFixedSizedMemOp =
            this->m_checkName.find("fixed-size-memory-operation") != string::npos;
        const bool enableAssignmentInIf =
            this->m_checkName.find("bugprone-assignment-in-if-condition") != string::npos;
        
        return enableMisraCpp2008 || enableMisraC2012 || enableGlobalVariable || enableEmptyCatch || enableFixedSizedMemOp ||
            enableDeactivatedCode || enableAssignmentInIf;
    }


    //bool EmboldTidyCheck::isBuiltIn(clang::SourceLocation loc) const {
    //  const SourceManager &sourceManager = getCI().getSourceManager();
    //  const char *const filename = sourceManager.getPresumedLoc(loc).getFilename();
    //  return (strcmp(filename, "<built-in>") == 0);
    //}


    //bool EmboldTidyCheck::isCommandLine(SourceLocation loc) {
    //  const SourceManager &sourceManager = getCI().getSourceManager();
    //  const char *const filename = sourceManager.getPresumedLoc(loc).getFilename();
    //  return (strcmp(filename, "<command line>") == 0);
    //}


    //ASTContext &EmboldTidyCheck::getASTContext() const {
    //  assert(getCI().hasASTContext() && "Compiler instance needs AST context!");
    //  return getCI().getASTContext();
    //}


    //CompilerInstance &EmboldTidyCheck::getCI() const {
    //  assert(CI && "Need CompilerInstance");
    //  return *CI;
    //}
}
