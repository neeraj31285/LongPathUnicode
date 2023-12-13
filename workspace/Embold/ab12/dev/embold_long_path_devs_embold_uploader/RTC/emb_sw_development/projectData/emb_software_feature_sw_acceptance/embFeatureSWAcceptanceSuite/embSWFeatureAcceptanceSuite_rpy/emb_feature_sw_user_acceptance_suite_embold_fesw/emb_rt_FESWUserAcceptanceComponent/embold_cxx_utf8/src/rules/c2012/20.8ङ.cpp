//===--- 20.8.cpp - clang-tidy-misra --------------------------------------===//
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "20.8.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include <iostream>
#include <vector>
#include <regex> 

using namespace clang;
using namespace tidy;
namespace emboldCxx {
	namespace c2012 {

		using namespace ast_matchers;
		using namespace llvm;
		using namespace std;

		Rule_20_8_PPCallbacks::Rule_20_8_PPCallbacks(Rule_20_8& pRuleChecker)
			: PPCallbacks()
			, m_ruleChecker(pRuleChecker) { }


		const MacroDirective* Rule_20_8::getMacroReplacementToken(const std::string& pMacroStr)
		{
			auto itr = m_macroMap.find(pMacroStr);
			if (itr == m_macroMap.end()) {
				return nullptr;
			}

			const MacroInfo* macroInfo = itr->second->getMacroInfo();
			if (macroInfo->getNumTokens() > 1) {
				return nullptr;
			}

			const Token& token = macroInfo->getReplacementToken(0);
			if (token.isAnyIdentifier()) {
				const string& tokenStr = token.getIdentifierInfo()->getName().str();
				return getMacroReplacementToken(tokenStr);
			}
			return itr->second;
		}

		Rule_20_8::Rule_20_8(llvm::StringRef Name, ClangTidyContext* Context)
			: EmboldTidyCheck(Name, Context) { }


		void Rule_20_8_PPCallbacks::If(SourceLocation Loc, SourceRange range, ConditionValueKind kind) {

			m_ruleChecker.m_sourceLocs.emplace_back(Loc);
			m_ruleChecker.m_sourceRanges.emplace_back(range);
		}

		void Rule_20_8_PPCallbacks::Elif(SourceLocation Loc, SourceRange ConditionRange, ConditionValueKind ConditionValue, SourceLocation IfLoc) {

			m_ruleChecker.m_sourceLocs.emplace_back(Loc);
			m_ruleChecker.m_sourceRanges.emplace_back(ConditionRange);
		}


		void Rule_20_8_PPCallbacks::MacroDefined(const Token& MacroNameTok, const MacroDirective* MD)
		{
			const std::string& macro_name = MacroNameTok.getIdentifierInfo()->getName().str();
			m_ruleChecker.m_macroMap.emplace(make_pair(macro_name, MD));
		}

		bool is_integer(const std::string& s) {
			return std::regex_match(s, std::regex("[(-|+)|][0-9]+"));
		}


		void Rule_20_8::registerPPCallbacksImpl() {
			getPreprocessor().addPPCallbacks(std::make_unique<Rule_20_8_PPCallbacks>(*this));
		}


		void Rule_20_8::registerMatchers(ast_matchers::MatchFinder* Finder) {
			Finder->addMatcher(ast_matchers::translationUnitDecl().bind("TUDecl"), this);
		}

		void Rule_20_8::checkImpl(const ast_matchers::MatchFinder::MatchResult& Result) {

			clang::SourceManager& srcManager = Result.Context->getSourceManager();
			for (size_t index = 0; index < m_sourceRanges.size(); index++) {

				auto& srcLoc = m_sourceLocs.at(index);
				auto& srcRange = m_sourceRanges.at(index);
				if (!srcManager.isInSystemHeader(srcRange.getBegin()) && !srcManager.isInSystemMacro(srcRange.getBegin())) {

					const string& condStr = Lexer::getSourceText(CharSourceRange::getTokenRange(srcRange), srcManager, LangOptions()).str();
					const auto& token = Lexer::findNextToken(srcLoc, srcManager, LangOptions()).getValue();
					if (!token.isAnyIdentifier()) {
						if (condStr != "1" && condStr != "0" && is_integer(condStr)) {
							diag(srcRange.getBegin());
						}
					}
					else {
						const string& macroName = token.getRawIdentifier().str();
						if (condStr.size() != macroName.size()) {
							continue;
						}
						const MacroDirective* macroDirv = getMacroReplacementToken(macroName);
						if (macroDirv != nullptr) {
							const Token& macroToken = macroDirv->getMacroInfo()->getReplacementToken(0);
							const string& macroDefn = Lexer::getSpelling(macroToken, srcManager, LangOptions());
							if (macroDefn != "1" && macroDefn != "0") {
								diag(srcRange.getBegin());
							}
						}
					}
				}
			}
		}


	} // namespace c2012
} // namespace emboldCxx