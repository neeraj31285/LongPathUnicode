//===--- 20.13.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "20.13.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <iostream>
#include <unordered_set> 

using namespace clang;
using namespace tidy;
namespace emboldCxx {
	namespace c2012 {

		using namespace ast_matchers;
		using namespace std;

		unordered_set<std::string> valid_directives = { "define", "elif", "else", "endif", "error",
														"if", "ifdef", "ifndef", "include", "pragma",
														 "undef", "warning", "asm", "endasm" };

		Rule_20_13_PPCallbacks::Rule_20_13_PPCallbacks(Rule_20_13& pRuleChecker)
			: PPCallbacks()
			, m_ruleChecker(pRuleChecker) { }

		void Rule_20_13::registerPPCallbacksImpl() {
			getPreprocessor().addPPCallbacks(std::make_unique<Rule_20_13_PPCallbacks>(*this));
		}

		llvm::Optional<SourceRange> GetSourceRange(SourceLocation beginLoc, SourceLocation endLoc)
		{
			llvm::Optional<SourceLocation> Begin = beginLoc;
			llvm::Optional<SourceLocation> End = endLoc;

			if (!Begin || !End)
			{
				return llvm::None;
			}
			return SourceRange((*Begin), (*End));
		}

		void Rule_20_13_PPCallbacks::Endif(SourceLocation Loc, SourceLocation IfLoc) {
			llvm::Optional<SourceRange> Range = GetSourceRange(IfLoc, Loc);
			m_ruleChecker.m_sourceRanges.emplace_back(*Range);
		}

		Rule_20_13::Rule_20_13(llvm::StringRef Name, ClangTidyContext* Context)
			: EmboldTidyCheck(Name, Context) { }


		void Rule_20_13::registerMatchers(ast_matchers::MatchFinder* Finder) {
			Finder->addMatcher(ast_matchers::translationUnitDecl().bind("TUDecl"), this);
		}

		void Rule_20_13::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
			clang::SourceManager& srcManager = Result.Context->getSourceManager();

			for (size_t index = 0; index < m_sourceRanges.size(); index++) {
				auto& srcRange = m_sourceRanges.at(index);
				if (!srcManager.isInSystemHeader(srcRange.getBegin()) && !srcManager.isInSystemMacro(srcRange.getBegin()))
				{
					const string file_src = Lexer::getSourceText(CharSourceRange::getTokenRange(srcRange), srcManager, LangOptions()).str();
					auto begin_loc = srcRange.getBegin();
					int offset = file_src.size();

					for (auto i = 0; i < offset; i++)
					{
						SourceLocation src_loc = begin_loc.getLocWithOffset(i);
						const auto& token = Lexer::findNextToken(src_loc, srcManager, LangOptions()).getValue();
						{
							std::string rawIdentifier = token.getRawIdentifier().str();
							if (token.is(tok::hash))
							{
								const auto& token_d = Lexer::findNextToken(token.getLocation(), srcManager, LangOptions()).getValue();
								if (valid_directives.find(token_d.getRawIdentifier().str()) == valid_directives.end())
								{
									diag(token_d.getLocation());
								}
							}
						}
					}
				}
			}
		}


	} // namespace c2012
} // namespace emboldCxx