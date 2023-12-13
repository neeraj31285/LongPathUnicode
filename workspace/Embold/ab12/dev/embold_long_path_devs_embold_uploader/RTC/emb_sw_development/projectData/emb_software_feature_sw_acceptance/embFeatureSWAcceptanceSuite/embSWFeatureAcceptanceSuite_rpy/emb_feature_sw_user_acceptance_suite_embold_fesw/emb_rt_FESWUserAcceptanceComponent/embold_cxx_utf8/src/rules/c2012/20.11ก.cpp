//===--- 20.11.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "20.11.h"
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

		unordered_set<std::string> parameters;

		Rule_20_11_PPCallbacks::Rule_20_11_PPCallbacks(Rule_20_11& pRuleChecker)
			: PPCallbacks()
			, m_ruleChecker(pRuleChecker) { }

		void Rule_20_11::registerPPCallbacksImpl() {
			getPreprocessor().addPPCallbacks(std::make_unique<Rule_20_11_PPCallbacks>(*this));
		}

		void Rule_20_11_PPCallbacks::MacroDefined(const Token& MacroNameTok, const MacroDirective* MD) {
			const MacroInfo* macroInfo = MD->getMacroInfo();

			for (MacroInfo::param_iterator begin = macroInfo->param_begin(),
				end = macroInfo->param_end();
				begin != end; ++begin) {
				auto B = *begin;
				parameters.insert(B->getName().str());
			}

			for (MacroInfo::tokens_iterator I = macroInfo->tokens_begin(),
				E = macroInfo->tokens_end();
				I != E; ++I) {
				const std::string tokenType = I->getName();
				if (tokenType.find("hash") == 0)
				{
					m_ruleChecker.m_map.emplace_back(I->getLocation());
				}
			}
		}

		Rule_20_11::Rule_20_11(llvm::StringRef Name, ClangTidyContext* Context)
			: EmboldTidyCheck(Name, Context) { }


		void Rule_20_11::registerMatchers(ast_matchers::MatchFinder* Finder) {
			Finder->addMatcher(ast_matchers::translationUnitDecl().bind("TUDecl"), this);
		}

		void Rule_20_11::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
			clang::SourceManager& srcManager = Result.Context->getSourceManager();

			for (size_t index = 0; index < m_map.size(); index++)
			{
				auto& srcLocation = m_map.at(index);
				if (!srcManager.isInSystemHeader(srcLocation) && !srcManager.isInSystemMacro(srcLocation))
				{
					const auto& token = Lexer::findNextToken(srcLocation, srcManager, LangOptions()).getValue();
					std::string rawIdentifier = token.getRawIdentifier().str();
					if (parameters.find(rawIdentifier) != parameters.end())
					{
						const auto& next_token = Lexer::findNextToken(token.getLocation(), srcManager, LangOptions()).getValue();
						if (next_token.is(tok::hashhash))
						{
							diag(next_token.getLocation());
						}
					}
				}
			}

		}


	} // namespace c2012
} // namespace emboldCxx