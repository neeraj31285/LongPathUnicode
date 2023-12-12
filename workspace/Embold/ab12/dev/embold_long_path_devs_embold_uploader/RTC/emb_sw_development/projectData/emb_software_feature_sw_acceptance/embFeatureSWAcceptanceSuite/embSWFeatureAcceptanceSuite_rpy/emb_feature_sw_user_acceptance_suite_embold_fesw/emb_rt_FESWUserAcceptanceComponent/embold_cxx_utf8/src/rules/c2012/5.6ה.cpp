
#include <string>
#include <iostream>
#include <unordered_map>

#include "5.6в.h"
using namespace std;

using namespace clang;
using namespace tidy;
namespace emboldCxx {
	namespace c2012 {

		using namespace ast_matchers;


		static bool isInUserCode(const NamedDecl* D) {
			clang::SourceManager& SM = D->getASTContext().getSourceManager();
			return !(SM.isInSystemHeader(D->getLocation()) || SM.isInSystemMacro(D->getLocation()));
		}

		Rule_5_6::Rule_5_6(llvm::StringRef Name, ClangTidyContext* Context)
			: EmboldTidyCheck(Name, Context) {
		}

		void Rule_5_6::registerMatchers(MatchFinder* Finder) {
			Finder->addMatcher(namedDecl().bind("NamedDecl"), this);
		}

		static bool isExceptionalCase(const NamedDecl* D) {
			bool found_exceptionalCase = false;

			if (D->getKind() == clang::Decl::Kind::Typedef)
			{
				const clang::TypedefDecl* typedefdecl = clang::dyn_cast<clang::TypedefDecl>(const_cast<clang::NamedDecl*>(D));
				if (typedefdecl != nullptr)
				{
					clang::TypeSourceInfo* typeSourceInfo = typedefdecl->getTypeSourceInfo();
					if (typeSourceInfo != nullptr)
					{
						const clang::QualType type = typeSourceInfo->getType();
						if (!type.isNull())
						{
							if (type->isRecordType() == 1)
							{
								if (type->getAsRecordDecl()->getName().str() == D->getName().str())
								{
									found_exceptionalCase = true;
								}
							}
							if (type->isEnumeralType() == 1)
							{
								if (type->getAs<clang::EnumType>()->getDecl()->getName().str() == D->getName().str())
								{
									found_exceptionalCase = true;
								}
							}
						}
					}
				}
			}
			return found_exceptionalCase;
		}

		void Rule_5_6::checkImpl(const MatchFinder::MatchResult& Result) {
			const NamedDecl* D = Result.Nodes.getNodeAs<NamedDecl>("NamedDecl");

			if (isInUserCode(D) == 0) {
				return;
			}

			if ((D->getKind() == clang::Decl::Kind::Typedef) && (isExceptionalCase(D) != 1))
			{
				if (typedef_map.find(D->getName().str()) == typedef_map.end())
				{
					typedef_map[D->getName().str()] = D;
				}
				else
				{
					diag(D->getSourceRange().getBegin());
					std::string desc = " typedef name '" + D->getName().str() + "' is reused ";
					auto traceLoc = typedef_map.at(D->getName().str())->getSourceRange().getBegin();
					ClangTidyCheck::diag(traceLoc, desc, DiagnosticIDs::Note);
				}
			}
			else
			{
				nontypedef_map[D->getName().str()] = D;
			}
		}

		void Rule_5_6::onEndOfTranslationUnit() {
			for (auto decl : typedef_map)
			{
				std::string declName = decl.first;
				if (nontypedef_map.find(declName) != nontypedef_map.end())
				{
					auto violation_loc = nontypedef_map.at(declName)->getSourceRange().getBegin();
					diag(violation_loc);
					auto trace_loc = decl.second->getSourceRange().getBegin();
					std::string desc = " typedef name '" + declName + "' is reused ";
					ClangTidyCheck::diag(trace_loc, desc, DiagnosticIDs::Note);
				}
			}
		}

	} // namespace c2012
} // namespace emboldCxx

