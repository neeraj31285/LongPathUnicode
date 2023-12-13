
# Create a variable containing the source files for your target
set(LOCAL_SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/EmboldTidyCheckב.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/RuleHeadlineTextsé.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/EmboldTidyModuleג.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/GammaRuleFactoryכ.cpp"
)

SET(LOCAL_HEADERS
	"${PROJECT_SOURCE_DIR}/inc/emboldCxxPch.h"
    "${PROJECT_SOURCE_DIR}/inc/EmboldTidyCheck.h"
    "${PROJECT_SOURCE_DIR}/inc/RuleHeadlineTexts.h"
	"${PROJECT_SOURCE_DIR}/inc/GammaRuleFactory.h"
	"${PROJECT_SOURCE_DIR}/inc/GammaRuleChecker.hpp"
)

# Add any additional source files if needed
target_sources(emboldCxx
    PUBLIC
        "${PROJECT_SOURCE_DIR}/inc/EmboldTidyModule.h"
		"${PROJECT_SOURCE_DIR}/inc/GammaRuleChecker.h"
    PRIVATE
        "${LOCAL_HEADERS}"
		"${LOCAL_SOURCES}"
)

INCLUDE(${CMAKE_CURRENT_LIST_DIR}/rules/c2012/CMakeLists.txt)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/rules/checks/CMakeLists.txt)
INCLUDE(${CMAKE_CURRENT_LIST_DIR}/rules/cpp2008/CMakeLists.txt)
