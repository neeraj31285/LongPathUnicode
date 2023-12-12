#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/Lex/Preprocessor.h"

namespace clang {
	class Preprocessor;
}

namespace emboldCxx {

	class EmboldTidyCheck : public clang::tidy::ClangTidyCheck
	{
		const std::string m_checkName;
		const bool IgnoreInvalidLocations;
		const bool IgnoreBuiltInLocations;
		const bool IgnoreCommandLineLocations;

		clang::Preprocessor* PP;
		clang::Preprocessor* ModuleExpanderPP;

	protected:

		virtual void registerPPCallbacksImpl();

		virtual void checkImpl(const clang::ast_matchers::MatchFinder::MatchResult& Result);

		bool isC() const;

		bool isCPlusPlus() const;

		bool checkerIsActive() const;

		clang::Preprocessor& getPreprocessor() const;

		/// Return the text of the token at \p start.
		std::string srcLocToTokenString(const clang::SourceManager& SM, const clang::SourceLocation start);

	public:

		EmboldTidyCheck(llvm::StringRef pCheckName, clang::tidy::ClangTidyContext* Context);

		bool isIgnored(clang::SourceLocation loc);

		void diag(clang::SourceLocation Loc, clang::DiagnosticIDs::Level Level = clang::DiagnosticIDs::Warning);

		virtual void storeOptions(clang::tidy::ClangTidyOptions::OptionMap& Opts) override;

		virtual void registerPPCallbacks(const clang::SourceManager& SM, clang::Preprocessor* PP, clang::Preprocessor* ModuleExpanderPP) override final;

		virtual void check(const clang::ast_matchers::MatchFinder::MatchResult& Result) override final;
	};
}
