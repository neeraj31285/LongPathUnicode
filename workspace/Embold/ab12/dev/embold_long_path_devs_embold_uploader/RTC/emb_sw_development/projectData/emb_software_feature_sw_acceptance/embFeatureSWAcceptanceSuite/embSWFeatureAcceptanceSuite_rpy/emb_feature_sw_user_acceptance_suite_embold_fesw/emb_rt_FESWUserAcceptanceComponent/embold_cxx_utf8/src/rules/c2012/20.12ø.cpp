//===--- 20.12.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "20.12.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/MacroArgs.h"
#include "llvm/ADT/StringRef.h"
#include <iostream>
#include <unordered_set> 

using namespace clang;
using namespace tidy;
namespace emboldCxx {
	namespace c2012 {

		using namespace ast_matchers;
		using namespace std;

		unordered_set<std::string> parameter_set;
		unordered_set <string> macroSet;

		Rule_20_12_PPCallbacks::Rule_20_12_PPCallbacks(Rule_20_12& pRuleChecker)
			: PPCallbacks()
			, m_ruleChecker(pRuleChecker) { }

		void Rule_20_12::registerPPCallbacksImpl() {
			getPreprocessor().addPPCallbacks(std::make_unique<Rule_20_12_PPCallbacks>(*this));
		}

		void Rule_20_12_PPCallbacks::MacroExpands(const Token& MacroNameTok, const MacroDefinition& MD,
			SourceRange	Range, const MacroArgs* Args) {
			MacroInfo* macroInfo = MD.getMacroInfo();
			auto* macroInfo_name = MacroNameTok.getIdentifierInfo();
			m_ruleChecker.macroExapand_map.insert({ macroInfo_name, std::make_pair(macroInfo,Range) });
		}

		void Rule_20_12_PPCallbacks::MacroDefined(const Token& MacroNameTok, const MacroDirective* MD) {
			const std::string& macro_name = MacroNameTok.getIdentifierInfo()->getName().str();
			macroSet.insert(macro_name);
		}

		Rule_20_12::Rule_20_12(llvm::StringRef Name, ClangTidyContext* Context)
			: EmboldTidyCheck(Name, Context) { }


		void Rule_20_12::registerMatchers(ast_matchers::MatchFinder* Finder) {
			Finder->addMatcher(ast_matchers::translationUnitDecl().bind("TUDecl"), this);
		}

		void Rule_20_12::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {
			clang::SourceManager& srcManager = Result.Context->getSourceManager();

			for (auto& itr : macroExapand_map)
			{
				IdentifierInfo* Id_info = itr.first;
				MacroInfo* MI = itr.second.first;
				SourceRange src_range = itr.second.second;

				if (!srcManager.isInSystemHeader(src_range.getBegin()) && !srcManager.isInSystemMacro(src_range.getBegin()))
				{
					StringRef file_src = Lexer::getSourceText(CharSourceRange::getTokenRange(src_range), srcManager, LangOptions()).str();

					if (!file_src.empty())
					{
						int Start = file_src.find("(") + 1;
						int End = file_src.find(")", Start);
						StringRef argument = file_src.substr(Start, End - Start);

						SmallVector<StringRef, 1> argument_v;
						argument.split(argument_v, ",", -1, false);
						for (auto i = argument_v.begin(); i != argument_v.end(); ++i)
						{
							string argument_str = (*i).trim().str();

							if (macroSet.find(argument_str) != macroSet.end())
							{
								for (MacroInfo::param_iterator begin = MI->param_begin(),
									end = MI->param_end();
									begin != end; ++begin) {
									auto B = *begin;
									parameter_set.insert(B->getName().str());
								}

								for (MacroInfo::tokens_iterator I = MI->tokens_begin(),
									E = MI->tokens_end();
									I != E; ++I) {
									const std::string tokenType = I->getName();
									if (tokenType.find("hash") == 0)
									{
										const auto& token = Lexer::findNextToken(I->getLocation(), srcManager, LangOptions()).getValue();
										std::string rawIdentifier = token.getRawIdentifier().str();
										if (parameter_set.find(rawIdentifier) != parameter_set.end())
										{
											diag(I->getLocation());
										}
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