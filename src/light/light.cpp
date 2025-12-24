#include "light.h"

#include <dxc/dxcapi.h>

#include <iostream>
#include <fstream>

namespace light {

    void test()
    {
        CComPtr<IDxcUtils> utils;
        CComPtr<IDxcCompiler3> compiler;
        
        DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
        DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
        
        const char* hlsl = R"(
        float4 main() : SV_Target
        {
            return float4(1,0,0,1);
        }
        )";
        CComPtr<IDxcBlobEncoding> source;
        utils->CreateBlob(hlsl, (uint32_t)strlen(hlsl), CP_UTF8, &source);
        
        DxcBuffer buffer{};
        buffer.Ptr = source->GetBufferPointer();
        buffer.Size = source->GetBufferSize();
        buffer.Encoding = DXC_CP_UTF8;
        
        LPCWSTR args[] = {
            L"-E", L"main",          // Entry point
            L"-spirv",
            L"-T", L"ps_6_0",        // Shader model
            L"-Zi",                  // Debug info
            L"-Qembed_debug",        // Embed debug info
            L"-Od"                   // Disable optimization
        };
        
        CComPtr<IDxcResult> result;
        compiler->Compile(
            &buffer,
            args,
            7,
            nullptr,
            IID_PPV_ARGS(&result)
        );
        
        CComPtr<IDxcBlobUtf8> errors;
        result->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&errors), nullptr);
        
        if (errors && errors->GetStringLength() > 0)
        {
            std::cout << errors->GetStringPointer() << std::endl;
        }
        
        CComPtr<IDxcBlob> shader;
        result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shader), nullptr);
        
        std::ofstream file("out.spv");
        file.write((const char*)shader->GetBufferPointer(), shader->GetBufferSize());
        file.close();
    }

}
