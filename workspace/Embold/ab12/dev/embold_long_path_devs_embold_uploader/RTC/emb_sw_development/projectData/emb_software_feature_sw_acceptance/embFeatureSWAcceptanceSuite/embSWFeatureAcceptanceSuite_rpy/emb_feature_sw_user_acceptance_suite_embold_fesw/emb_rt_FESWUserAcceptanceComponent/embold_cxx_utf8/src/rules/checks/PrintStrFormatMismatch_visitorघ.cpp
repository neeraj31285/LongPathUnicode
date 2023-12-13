#include "CheckerUtils.h"
#include "PrintStrFormatMismatch_visitor.h"

using namespace std;
using namespace clang;

PrintStrFormatMismatchVisitor::PrintStrFormatMismatchVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext), m_parentFuncDecl(nullptr)
{
}

const std::vector<clang::SourceLocation>& PrintStrFormatMismatchVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool PrintStrFormatMismatchVisitor::VisitCallExpr(clang::CallExpr* pCallExpr)
{
	// If this is a call to a builtin, return the builtin ID. If not, return 0.
	unsigned int builtIn = pCallExpr->getBuiltinCallee();

	if (CheckerUtils::isInUserCode(pCallExpr->getBeginLoc(), m_astContext) 
		&& (builtIn == PRINTF || builtIn == FPRINTF || builtIn == SNPRINTF || builtIn == SPRINTF))
	{
		unsigned int formatSpecifierArgIndex;
		unsigned int minArgs;

		if (builtIn == PRINTF)
		{
			formatSpecifierArgIndex = 0;
			minArgs = 1;
		}
		else if (builtIn == FPRINTF || builtIn == SPRINTF)
		{
			formatSpecifierArgIndex = 1;
			minArgs = 2;
		}
		else if (builtIn == SNPRINTF)
		{
			formatSpecifierArgIndex = 2;
			minArgs = 3;
		}

		unsigned int numArgs = pCallExpr->getNumArgs() - minArgs;

		Expr* formatSpecifierArg = pCallExpr->getArg(formatSpecifierArgIndex)->IgnoreParenImpCasts();

		if (isa<StringLiteral>(formatSpecifierArg))
		{
			StringLiteral* formatSpecifierStringLiteral = dyn_cast<StringLiteral>(formatSpecifierArg);
			llvm::StringRef formatSpecifier = formatSpecifierStringLiteral->getString();

			bool hasDotAsterisk = formatSpecifier.contains(".*");
			bool hasMoreThanOnePercentSign = formatSpecifier.contains("%%");

			if (!(hasDotAsterisk || hasMoreThanOnePercentSign))
			{
				size_t count = formatSpecifier.count('%');
				size_t countJustified = formatSpecifier.count("%*");
				size_t countFieldWidth = formatSpecifier.count("%-*");

				// TODO: When counting runs of % characters, don't allow overlaps. 
				// So "%%%d" should result in a value of 1
				//size_t countEscape = formatSpecifier.count("%%");
				//count = count + countJustified - (countEscape * 2);

				count = count + countJustified + countFieldWidth;

				if (numArgs != count)
					m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
			}

		}
	}
	return true;
}