SET(LOCAL_SOURCES 
	"${CMAKE_CURRENT_LIST_DIR}/AssignmentInIfConditionCheckΑ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprConstArg_checkerв.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprConstArg_visitorô.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprNoReturn_checkerб.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprNoReturn_visitorè.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprConstReturn_checkerख.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/AssignOprConstReturn_visitorग.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/DestructorNotVirtual_checkerי.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/DestructorNotVirtual_visitorה.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ConstOverrideMismatch_checkerв.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ConstOverrideMismatch_visitorí.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/NonPODMemOperation_checkerд.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/NonPODMemOperation_visitorł.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ShallowCopyConstruct_checkerА.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ShallowCopyConstruct_visitoró.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ShallowCopyAssignment_checkeræ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/ShallowCopyAssignment_visitorぅ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/PrintStrFormatMismatch_checkerえ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/PrintStrFormatMismatch_visitorघ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InfiniteLoop_checkerş.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InfiniteLoop_visitorñ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedLoopBoundary_checkerñ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedLoopBoundary_visitorÜ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedMemAllocation_checkerş.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedMemAllocation_visitorख.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/MainReturnsVoid_checkerı.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/MainReturnsVoid_visitorø.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/IncorrectPtrScale_checkerघ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/IncorrectPtrScale_visitorक.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/BitwiseOprandsMismatch_checkerא.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/BitwiseOprandsMismatch_visitorå.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedParamAsIndex_checkerÄ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnvalidatedParamAsIndex_visitorš.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnconditionalPtrDeref_checkerお.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnconditionalPtrDeref_visitoræ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InvalidIterator_checkerग.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InvalidIterator_visitorę.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InvalidIteratorDereferencing_checkerक.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InvalidIteratorDereferencing_visitorഖ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/IteratorChecksUtilityг.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InconsistentNullPtrCheck_checkerß.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/InconsistentNullPtrCheck_visitorぅ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnusedParameter_checkerÖ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/UnusedParameter_visitorÜ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HiddenBaseMember_checkerø.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HiddenBaseMember_visitorß.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HiddenParameter_checkeré.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HiddenParameter_visitoræ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/CharSignedDeclaration_checkerഖ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/CharSignedDeclaration_visitorå.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2121_visitorえ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2121_checker나.cpp"
	#"${CMAKE_CURRENT_LIST_DIR}/MisraRule_3_2_checkerך.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2115_checkerぁ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2115_visitorş.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2116_checkerń.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Misra_2116_visitor아.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/DynamicCast_checkerı.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/DynamicCast_visitorえ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HardCodedCredentials_checkerぇ.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/HardCodedCredentials_visitorあ.cpp"
	
	)

SET(LOCAL_HEADERS
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignmentInIfConditionCheck.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprConstArg_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprConstArg_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprNoReturn_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprNoReturn_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprConstReturn_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/AssignOprConstReturn_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/DestructorNotVirtual_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/DestructorNotVirtual_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ConstOverrideMismatch_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ConstOverrideMismatch_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/NonPODMemOperation_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/NonPODMemOperation_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ShallowCopyConstruct_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ShallowCopyConstruct_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ShallowCopyAssignment_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/ShallowCopyAssignment_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/PrintStrFormatMismatch_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/PrintStrFormatMismatch_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InfiniteLoop_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InfiniteLoop_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedLoopBoundary_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedLoopBoundary_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedMemAllocation_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedMemAllocation_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/MainReturnsVoid_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/MainReturnsVoid_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/IncorrectPtrScale_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/IncorrectPtrScale_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/BitwiseOprandsMismatch_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/BitwiseOprandsMismatch_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedParamAsIndex_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnvalidatedParamAsIndex_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnconditionalPtrDeref_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnconditionalPtrDeref_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InvalidIterator_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InvalidIterator_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InvalidIteratorDereferencing_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InvalidIteratorDereferencing_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/IteratorChecksUtility.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InconsistentNullPtrCheck_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/InconsistentNullPtrCheck_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnusedParameter_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/UnusedParameter_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HiddenBaseMember_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HiddenBaseMember_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HiddenParameter_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HiddenParameter_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/CharSignedDeclaration_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/CharSignedDeclaration_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2121_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2121_checker.h"
	#"${PROJECT_SOURCE_DIR}/inc/rules/checks/MisraRule_3_2_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2115_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2115_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2116_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/Misra_2116_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/DynamicCast_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/DynamicCast_visitor.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HardCodedCredentials_checker.h"
	"${PROJECT_SOURCE_DIR}/inc/rules/checks/HardCodedCredentials_visitor.h"
	)

target_sources(emboldCxx
    PRIVATE
        "${LOCAL_SOURCES}"
		"${LOCAL_HEADERS}"
)

SOURCE_GROUP("Source Files\\checks" FILES ${LOCAL_SOURCES})
SOURCE_GROUP("Header Files\\checks" FILES ${LOCAL_HEADERS})
