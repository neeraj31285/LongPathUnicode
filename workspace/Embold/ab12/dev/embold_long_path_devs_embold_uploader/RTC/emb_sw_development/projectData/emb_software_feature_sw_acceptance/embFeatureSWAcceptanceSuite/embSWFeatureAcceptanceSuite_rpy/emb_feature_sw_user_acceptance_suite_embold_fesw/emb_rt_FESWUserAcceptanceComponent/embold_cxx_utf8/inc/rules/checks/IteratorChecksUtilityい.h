
#include <set>
#include <string>

class IteratorChecksUtility
{

private:
	static std::set<std::string> containerTypes;
	static std::set<std::string> iteratorTypes;

public:

	static bool isContainerType(const std::string& objectType);

	static bool isIteratorType(const std::string& varType);

	static clang::Expr* GetCXXMemberCallExprArgument(clang::Expr* expr);

	static clang::ValueDecl* GetObjectDecl(clang::Expr* expr);

	static std::string GetType(clang::QualType type);
};
