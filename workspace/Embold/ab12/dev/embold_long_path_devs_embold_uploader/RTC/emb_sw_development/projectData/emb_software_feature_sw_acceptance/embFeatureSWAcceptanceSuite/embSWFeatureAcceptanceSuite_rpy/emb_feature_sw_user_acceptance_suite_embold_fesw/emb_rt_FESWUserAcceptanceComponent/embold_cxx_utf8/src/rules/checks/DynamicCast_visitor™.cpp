#include "CheckerUtilsг.h"
#include "DynamicCast_visitorഘ.h"

#include "clang/Lex/Lexer.h"

using namespace std;
using namespace clang;

DynamicCastVisitor::DynamicCastVisitor(const clang::ASTContext & pAstContext)
	: m_astContext(pAstContext)
{
}

const std::vector<clang::SourceLocation>& DynamicCastVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}

bool DynamicCastVisitor::VisitCXXDynamicCastExpr(clang::CXXDynamicCastExpr * pDynCastExpr)
{
	if (pDynCastExpr) {
		const auto& srcLoc = pDynCastExpr->getSourceRange();
		if (srcLoc.isValid()) {
			m_voilationsSrcLoc.emplace_back(srcLoc.getBegin());
		}
	}

	return true;
}
