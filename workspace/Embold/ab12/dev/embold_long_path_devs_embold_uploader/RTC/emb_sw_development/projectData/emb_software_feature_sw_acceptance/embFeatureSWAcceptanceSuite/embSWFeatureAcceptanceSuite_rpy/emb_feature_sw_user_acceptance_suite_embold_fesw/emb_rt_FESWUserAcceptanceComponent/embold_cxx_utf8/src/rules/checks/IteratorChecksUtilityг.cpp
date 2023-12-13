#include "IteratorChecksUtility.h"

using namespace clang;

std::set<std::string> IteratorChecksUtility::containerTypes = { "vector"
																,"set"
																,"unordered_set"
																,"multiset"
																,"unordered_multiset"
																,"map"
																,"unordered_map"
																,"multimap"
																,"unordered_multimap"
																,"deque"
																,"list"
																,"forward_list" };

std::set<std::string> IteratorChecksUtility::iteratorTypes = { "reverse_iterator"
																,"__deque_iterator"
																,"__forward_list_iterator"
																,"__forward_list_const_iterator"
																,"__list_iterator"
																,"__list_const_iterator"
																,"__map_iterator"
																,"__hash_map_iterator"
																,"__map_const_iterator"
																,"__hash_map_const_iterator"
																,"__tree_const_iterator"
																,"__hash_const_iterator"
																,"__wrap_iter" };

bool IteratorChecksUtility::isContainerType(const std::string& objectType)
{
	bool isContainer = false;

	if (containerTypes.find(objectType) != containerTypes.end())
		isContainer = true;

	return isContainer;
}

bool IteratorChecksUtility::isIteratorType(const std::string& varType)
{
	bool isIterator = false;

	if (iteratorTypes.find(varType) != iteratorTypes.end())
		isIterator = true;

	return isIterator;
}

// TODO: Get rid of this function after upgrading to clang 13
Expr* IteratorChecksUtility::GetCXXMemberCallExprArgument(clang::Expr* expr)
{
	Expr* argument = expr;

	while (isa<CXXConstructExpr>(argument))
	{
		CXXConstructExpr* constructExpr = dyn_cast<CXXConstructExpr>(argument);
		argument = constructExpr->getArg(0)->IgnoreParenImpCasts();
	}

	return argument;
}

ValueDecl* IteratorChecksUtility::GetObjectDecl(clang::Expr* objectExpr)
{
	ValueDecl* objectDecl = nullptr;

	if (isa<DeclRefExpr>(objectExpr))
	{
		DeclRefExpr* objectDeclRef = dyn_cast<DeclRefExpr>(objectExpr);
		objectDecl = objectDeclRef->getDecl();
	}
	else if (isa<MemberExpr>(objectExpr))
	{
		MemberExpr* objectMemberExpr = dyn_cast<MemberExpr>(objectExpr);
		objectDecl = objectMemberExpr->getMemberDecl();
	}

	return objectDecl;
}

std::string IteratorChecksUtility::GetType(clang::QualType type)
{
	if (type.getBaseTypeIdentifier() != nullptr)
		return type.getBaseTypeIdentifier()->getName().str();
	else
		return type.getAsString();
}