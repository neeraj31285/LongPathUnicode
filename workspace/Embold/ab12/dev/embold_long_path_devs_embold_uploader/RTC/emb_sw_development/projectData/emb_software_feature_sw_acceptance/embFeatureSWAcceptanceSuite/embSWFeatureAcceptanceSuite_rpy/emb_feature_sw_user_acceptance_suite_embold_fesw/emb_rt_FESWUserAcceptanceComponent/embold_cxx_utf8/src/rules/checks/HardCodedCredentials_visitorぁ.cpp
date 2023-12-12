#include "CheckerUtilsг.h"
#include "HardCodedCredentials_visitorż.h"
#include "logger.h"

using namespace clang;

static Acellere::Corona::CXX::Logger logger(Acellere::Corona::CXX::Logger::Category::CXXParser);

static std::regex password_pattern("[a-zA-z0-9_-]*(password|pwd|passwd|pass)[a-zA-z0-9_-]*", std::regex_constants::icase);
static std::regex username_pattern("[a-zA-z0-9_-]*(username|user)[a-zA-z0-9_-]*", std::regex_constants::icase);

HardCodedCredentialsVisitor::HardCodedCredentialsVisitor(clang::ASTContext& pAstContext)
	: m_astContext(pAstContext)
{
}

bool HardCodedCredentialsVisitor::VisitCXXOperatorCallExpr(clang::CXXOperatorCallExpr* pCXXOperatorCallExpr)
{
	if (CheckerUtils::isInUserCode(pCXXOperatorCallExpr->getBeginLoc(), m_astContext))
	{
		if (pCXXOperatorCallExpr->getOperator() == clang::OverloadedOperatorKind::OO_EqualEqual || pCXXOperatorCallExpr->isAssignmentOp())
		{
			DeclRefExpr* declRefExpr = dyn_cast<DeclRefExpr>(pCXXOperatorCallExpr->getArg(0));
			StringLiteral* stringLiteral = dyn_cast<StringLiteral>(pCXXOperatorCallExpr->getArg(1)->IgnoreParenImpCasts());

			if (declRefExpr && stringLiteral)
			{
				std::string variableName = declRefExpr->getDecl()->getNameAsString();
				if (std::regex_match(variableName, username_pattern) || std::regex_match(variableName, password_pattern))
					m_voilationsSrcLoc.emplace_back(pCXXOperatorCallExpr->getBeginLoc());
			}
		}
	}
	return true;
}

bool HardCodedCredentialsVisitor::VisitCXXMemberCallExpr(clang::CXXMemberCallExpr* pCXXMemberCallExpr)
{
	if (CheckerUtils::isInUserCode(pCXXMemberCallExpr->getBeginLoc(), m_astContext))
	{
		const auto directCalle = pCXXMemberCallExpr->getDirectCallee();
		if (directCalle && directCalle->getNameAsString() == "compare" && pCXXMemberCallExpr->getNumArgs() == 1)
		{
			DeclRefExpr* declRefExpr = dyn_cast<DeclRefExpr>(pCXXMemberCallExpr->getImplicitObjectArgument());
			StringLiteral* stringLiteral = dyn_cast<StringLiteral>(pCXXMemberCallExpr->getArg(0)->IgnoreParenImpCasts());

			if (declRefExpr && stringLiteral)
			{
				std::string variableName = declRefExpr->getDecl()->getNameAsString();
				if (std::regex_match(variableName, username_pattern) || std::regex_match(variableName, password_pattern))
					m_voilationsSrcLoc.emplace_back(pCXXMemberCallExpr->getBeginLoc());
			}
		}
	}
	return true;
}

bool HardCodedCredentialsVisitor::VisitCallExpr(clang::CallExpr* pCallExpr)
{
	if (CheckerUtils::isInUserCode(pCallExpr->getBeginLoc(), m_astContext))
	{
		const auto directCalle = pCallExpr->getDirectCallee();
		if (directCalle && directCalle->getNameAsString() == "strcmp" && pCallExpr->getNumArgs() == 2)
		{
			DeclRefExpr* declRefExpr = dyn_cast<DeclRefExpr>(pCallExpr->getArg(0)->IgnoreParenImpCasts());
			StringLiteral* stringLiteral = dyn_cast<StringLiteral>(pCallExpr->getArg(1)->IgnoreParenImpCasts());

			if (declRefExpr && stringLiteral)
			{
				std::string variableName = declRefExpr->getDecl()->getNameAsString();
				if (std::regex_match(variableName, username_pattern) || std::regex_match(variableName, password_pattern))
					m_voilationsSrcLoc.emplace_back(pCallExpr->getBeginLoc());
			}
		}
	}
	return true;
}

bool HardCodedCredentialsVisitor::VisitVarDecl(clang::VarDecl* pVarDecl)
{
	if (CheckerUtils::isInUserCode(pVarDecl) && pVarDecl->isLocalVarDecl())
	{
		if (pVarDecl->getInit() && (pVarDecl->getInitStyle() == clang::VarDecl::InitializationStyle::CInit))
		{
			std::string variableName = pVarDecl->getNameAsString();
			if (std::regex_match(variableName, username_pattern) || std::regex_match(variableName, password_pattern))
			{
				StringLiteral* stringLiteral = GetInitStringLiteral(pVarDecl->getInit()->IgnoreParenImpCasts());

				if (stringLiteral)
					m_voilationsSrcLoc.emplace_back(pVarDecl->getBeginLoc());
			}
		}
	}
	return true;
}

clang::StringLiteral* HardCodedCredentialsVisitor::GetInitStringLiteral(clang::Expr* initExpr)
{
	StringLiteral* stringLiteral = nullptr;

	if (isa<StringLiteral>(initExpr))
	{
		stringLiteral = dyn_cast<StringLiteral>(initExpr);
	}
	else
	{
		if (isa<CXXConstructExpr>(initExpr))
		{
			CXXConstructExpr* constructExpr = dyn_cast<CXXConstructExpr>(initExpr);

			if (constructExpr->getNumArgs() == 1)
				initExpr = constructExpr->getArg(0)->IgnoreParenImpCasts();
		}

		if (isa<CXXBindTemporaryExpr>(initExpr))
		{
			CXXBindTemporaryExpr* temporaryExpr = dyn_cast<CXXBindTemporaryExpr>(initExpr);

			if (isa<CXXConstructExpr>(temporaryExpr->getSubExpr()->IgnoreParenImpCasts()))
			{
				CXXConstructExpr* constructExpr = dyn_cast<CXXConstructExpr>(temporaryExpr->getSubExpr()->IgnoreParenImpCasts());

				if (constructExpr->getNumArgs() == 1)
					stringLiteral = dyn_cast<StringLiteral>(constructExpr->getArg(0)->IgnoreParenImpCasts());
			}
		}
	}

	return stringLiteral;
}

const std::vector<clang::SourceLocation>& HardCodedCredentialsVisitor::getViolationSrcLocations()
{
	return m_voilationsSrcLoc;
}


