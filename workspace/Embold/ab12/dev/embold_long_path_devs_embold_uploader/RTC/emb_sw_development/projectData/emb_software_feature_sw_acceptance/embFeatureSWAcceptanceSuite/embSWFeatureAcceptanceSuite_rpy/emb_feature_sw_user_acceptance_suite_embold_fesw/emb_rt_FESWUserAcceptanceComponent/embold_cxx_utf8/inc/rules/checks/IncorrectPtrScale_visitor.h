#pragma once

#include <unordered_map>
#include <fstream>

#pragma warning(push,0)
#include "clang/AST/RecursiveASTVisitor.h"
#pragma warning(pop)


class IncorrectPtrScaleVisitor : public clang::RecursiveASTVisitor<IncorrectPtrScaleVisitor> {

	const clang::ASTContext& m_astContext;
	std::vector<clang::SourceLocation> m_voilationsSrcLoc;
	std::unordered_map<std::string, clang::QualType> m_assignedValTypes;

	void recordAssignmentOperation(clang::BinaryOperator * pBinOpr);

	void updateAssignmentTypeMap(const std::string& pVarStr, const clang::QualType& pType);

	void updateAssignmentTypeMap(const std::string& pVarStr, clang::DeclRefExpr* pDeclRef);

	bool isIncorrectTypeBasedScaling(const clang::ValueDecl* pValueDecl, const clang::QualType& pOtherType);

public:

	IncorrectPtrScaleVisitor(const clang::ASTContext& pAstContext);

	bool VisitVarDecl(clang::VarDecl* pVarDecl);

	bool VisitBinaryOperator(clang::BinaryOperator *pBinOpr);

	const std::vector<clang::SourceLocation>& getViolationSrcLocations();
};
