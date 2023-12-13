#include "emboldCxxPch.h"
#include "EmboldTidyModule.h"


namespace emboldCxx {

    using namespace c2012;
    using namespace cpp2008;

    EmboldTidyModule::GetRelativePathFPtr EmboldTidyModule::m_cbGetRelativePath;

    const bool EmboldTidyModule::initialize(GetRelativePathFPtr pCallBack){
        m_cbGetRelativePath = pCallBack;
        addChecksConsumerFactories();
        return true;
    }

    std::string EmboldTidyModule::getRelativePath(const std::string& pPath){
        try{
            return m_cbGetRelativePath(pPath);
        }
        catch (const std::bad_function_call& e){
            return std::string();
        }
    }

    void EmboldTidyModule::addChecksConsumerFactories()
    {
        AssignOprConstArgChecker::initialize();
        AssignOprNoReturnChecker::initialize();
        AssignOprConstReturnChecker::initialize();
        DestructorNotVirtualChecker::initialize();
        ConstOverrideMismatchChecker::initialize();
        NonPODMemOperationChecker::initialize();
        ShallowCopyConstructChecker::initialize();
        ShallowCopyAssignmentChecker::initialize();
        PrintStrFormatMismatchChecker::initialize();
        InfiniteLoopChecker::initialize();
        UnvalidatedLoopBoundaryChecker::initialize();
        UnvalidatedMemAllocChecker::initialize();
        MainReturnsVoidChecker::initialize();
        IncorrectPtrScaleChecker::initialize();
        BitOprandsMismatchChecker::initialize();
        UnvalidatedParamAsIndexChecker::initialize();
        UnconditionalPtrDerefChecker::initialize();
        InvalidIteratorChecker::initialize();
        InvalidIteratorDereferencingChecker::initialize();
        InconsistentNullPtrCheckChecker::initialize();
        UnusedParameterChecker::initialize();
        HiddenBaseMemberChecker::initialize();
        HiddenParameterChecker::initialize();
        CharSignedDeclarationChecker::initialize();
        Misra_2121Checker::initialize();
        Misra_2115Checker::initialize();
        Misra_2116Checker::initialize();
        DynamicCastChecker::initialize();
        HardCodedCredentialsChecker::initialize();
    }


    void EmboldTidyModule::addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories)
    {
        CheckFactories.registerCheck<Rule_globalVariable>("global-variable");
        CheckFactories.registerCheck<Rule_emptyCatch>("empty-catch");
        CheckFactories.registerCheck<Rule_20_8>("misra-c2012-20.8");
        CheckFactories.registerCheck<Rule_20_11>("misra-c2012-20.11");
        CheckFactories.registerCheck<Rule_20_12>("misra-c2012-20.12");
        CheckFactories.registerCheck<Rule_20_13>("misra-c2012-20.13");
        CheckFactories.registerCheck<AssignmentInIfConditionCheck>("bugprone-assignment-in-if-condition");
        CheckFactories.registerCheck<Rule_fixedSizeMemoryOperation>("fixed-size-memory-operation");
        CheckFactories.registerCheck<Rule_deactivatedCode>("deactivated-code");
        CheckFactories.registerCheck<Rule_17_1>("misra-c2012-17.1");
        CheckFactories.registerCheck<Rule_17_2>("misra-c2012-17.2");
        CheckFactories.registerCheck<Rule_17_7>("misra-c2012-17.7");
        CheckFactories.registerCheck<Rule_18_4>("misra-c2012-18.4");
        CheckFactories.registerCheck<Rule_18_7>("misra-c2012-18.7");
        CheckFactories.registerCheck<clang::tidy::bugprone::MacroParenthesesCheck>("misra-c2012-20.7");

        CheckFactories.registerCheck<Rule_17_0_1>("misra-c2012-21.1");
        CheckFactories.registerCheck<Rule_21_6>("misra-c2012-21.6");
        CheckFactories.registerCheck<Rule_21_8>("misra-c2012-21.8");
        CheckFactories.registerCheck<Rule_21_12>("misra-c2012-21.12");
        CheckFactories.registerCheck<Rule_21_10>("misra-c2012-21.10");
        CheckFactories.registerCheck<Rule_22_5>("misra-c2012-22.5");
        CheckFactories.registerCheck<Rule_2_7>("misra-c2012-2.7");
         
        //crashes on linux, open-cv, libpcap.
        //CheckFactories.registerCheck<Rule_5_2>("misra-c2012-5.2");

        CheckFactories.registerCheck<Rule_5_6>("misra-c2012-5.6");
        CheckFactories.registerCheck<Rule_5_7>("misra-c2012-5.7");
        CheckFactories.registerCheck<Rule_6_1>("misra-c2012-6.1");
        CheckFactories.registerCheck<Rule_6_2>("misra-c2012-6.2");
        CheckFactories.registerCheck<Rule_7_2>("misra-c2012-7.2");
        CheckFactories.registerCheck<Rule_7_4>("misra-c2012-7.4");
        CheckFactories.registerCheck<Rule_8_2>("misra-c2012-8.2");
        CheckFactories.registerCheck<Rule_8_4>("misra-c2012-8.4");
        CheckFactories.registerCheck<Rule_8_5>("misra-c2012-8.5");

        CheckFactories.registerCheck<Rule_8_8>("misra-c2012-8.8");
        CheckFactories.registerCheck<Rule_8_10>("misra-c2012-8.10");
        CheckFactories.registerCheck<Rule_8_11>("misra-c2012-8.11");
        CheckFactories.registerCheck<Rule_9_3>("misra-c2012-9.3");
        CheckFactories.registerCheck<Rule_10_5>("misra-c2012-10.5");
        CheckFactories.registerCheck<Rule_11_1>("misra-c2012-11.1");
        CheckFactories.registerCheck<Rule_11_2>("misra-c2012-11.2");
        CheckFactories.registerCheck<Rule_11_4>("misra-c2012-11.4");
        CheckFactories.registerCheck<Rule_11_5>("misra-c2012-11.5");
        CheckFactories.registerCheck<Rule_2_13_3>("misra-cpp2008-2-13-3");
        CheckFactories.registerCheck<Rule_4_10_2>("misra-cpp2008-4-10-2");
        CheckFactories.registerCheck<Rule_6_4_1>("misra-cpp2008-6-4-1");
        CheckFactories.registerCheck<Rule_6_4_2>("misra-cpp2008-6-4-2");
        CheckFactories.registerCheck<Rule_6_4_4>("misra-cpp2008-6-4-4");
        CheckFactories.registerCheck<Rule_9_5_1>("misra-cpp2008-9-5-1");
        CheckFactories.registerCheck<Rule_10_3_2>("misra-cpp2008-10-3-2");
        CheckFactories.registerCheck<Rule_10_3_3>("misra-cpp2008-10-3-3");
        CheckFactories.registerCheck<Rule_11_0_1>("misra-cpp2008-11-0-1");
        CheckFactories.registerCheck<Rule_12_8_2>("misra-cpp2008-12-8-2");
        CheckFactories.registerCheck<Rule_15_5_1>("misra-cpp2008-15-5-1");
        CheckFactories.registerCheck<Rule_16_3_1>("misra-cpp2008-16-3-1");
        CheckFactories.registerCheck<Rule_16_3_2>("misra-cpp2008-16-3-2");
        CheckFactories.registerCheck<Rule_17_0_1>("misra-cpp2008-17-0-1");
        CheckFactories.registerCheck<Rule_17_0_5>("misra-cpp2008-17-0-5");
        CheckFactories.registerCheck<Rule_18_0_1>("misra-cpp2008-18-0-1");
        CheckFactories.registerCheck<Rule_18_0_2>("misra-cpp2008-18-0-2");
        CheckFactories.registerCheck<Rule_18_0_3>("misra-cpp2008-18-0-3");
        CheckFactories.registerCheck<Rule_18_0_4>("misra-cpp2008-18-0-4");
        CheckFactories.registerCheck<Rule_18_0_5>("misra-cpp2008-18-0-5");
        CheckFactories.registerCheck<Rule_18_2_1>("misra-cpp2008-18-2-1");
        CheckFactories.registerCheck<Rule_18_4_1>("misra-cpp2008-18-4-1");
        CheckFactories.registerCheck<Rule_18_7_1>("misra-cpp2008-18-7-1");
        CheckFactories.registerCheck<Rule_19_3_1>("misra-cpp2008-19-3-1");
        CheckFactories.registerCheck<Rule_27_0_1>("misra-cpp2008-27-0-1");
    }
}