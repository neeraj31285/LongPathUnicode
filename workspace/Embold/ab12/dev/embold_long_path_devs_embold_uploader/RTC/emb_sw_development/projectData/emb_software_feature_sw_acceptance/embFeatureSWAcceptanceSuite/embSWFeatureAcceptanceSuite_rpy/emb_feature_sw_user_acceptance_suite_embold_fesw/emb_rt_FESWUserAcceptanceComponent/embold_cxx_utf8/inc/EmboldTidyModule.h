#pragma once
#include "clang-tidy/ClangTidyModule.h"

namespace emboldCxx {

    class EmboldTidyModule : public clang::tidy::ClangTidyModule
    {
        using GetRelativePathFPtr = std::string(*)(const std::string&);
        static GetRelativePathFPtr m_cbGetRelativePath;

        static void addChecksConsumerFactories();

    public:

        static const bool initialize(GetRelativePathFPtr pCallBack);

        static std::string getRelativePath(const std::string& pPath);

        void addCheckFactories(clang::tidy::ClangTidyCheckFactories& CheckFactories) override;
    };
}