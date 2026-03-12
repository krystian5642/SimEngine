#include "ShaderCompiler.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

#include <dxcapi.h>
#include <combaseapi.h>
#include "Core/Logging/Log.h"

static std::wstring ToWString(const std::string& str)
{
    if (str.empty()) return {};
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring result(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), result.data(), len);
    return result;
}

VkShaderModule ShaderCompiler::CompileHLSLShaderToSPIRV(VkDevice device, const std::string& filename)
{
    HRESULT hres;

    std::wstring wFilename = ToWString(filename);

    ComPtr<IDxcLibrary> library;
    hres = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
    if (FAILED(hres))
        throw std::runtime_error("Could not init DXC Library");

    ComPtr<IDxcCompiler3> compiler;
    hres = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));
    if (FAILED(hres))
        throw std::runtime_error("Could not init DXC Compiler");

    ComPtr<IDxcUtils> utils;
    hres = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));
    if (FAILED(hres))
        throw std::runtime_error("Could not init DXC Utiliy");

    uint32_t codePage = DXC_CP_ACP;
    ComPtr<IDxcBlobEncoding> sourceBlob;
    hres = utils->LoadFile(wFilename.c_str(), &codePage, &sourceBlob);
    if (FAILED(hres))
        throw std::runtime_error("Could not load shader file");

    LPCWSTR targetProfile{};
    size_t lastDot = wFilename.rfind(L'.');
    if (lastDot != std::wstring::npos) {
        size_t secondDot = wFilename.rfind(L'.', lastDot - 1);
        if (secondDot != std::wstring::npos) {
            std::wstring extension = wFilename.substr(secondDot + 1, lastDot - secondDot - 1);
            if (extension == L"vs") targetProfile = L"vs_6_1";
            if (extension == L"fs") targetProfile = L"ps_6_1";
            if (extension == L"cs") targetProfile = L"cs_6_1";
        }
    }

    std::vector<LPCWSTR> arguments = {
        wFilename.c_str(),
        L"-E", L"main",
        L"-T", targetProfile,
        L"-spirv"
    };

    DxcBuffer buffer{};
    buffer.Encoding = DXC_CP_ACP;
    buffer.Ptr = sourceBlob->GetBufferPointer();
    buffer.Size = sourceBlob->GetBufferSize();

    ComPtr<IDxcResult> result{ nullptr };
    hres = compiler->Compile(
        &buffer,
        arguments.data(),
        (uint32_t)arguments.size(),
        nullptr,
        IID_PPV_ARGS(&result));

    if (SUCCEEDED(hres))
        result->GetStatus(&hres);

    if (FAILED(hres) && (result)) {
        ComPtr<IDxcBlobEncoding> errorBlob;
        hres = result->GetErrorBuffer(&errorBlob);
        if (SUCCEEDED(hres) && errorBlob) {
           std::cerr << "Shader compilation failed :\n\n" << (const char*)errorBlob->GetBufferPointer();
           throw std::runtime_error("Compilation failed");
        }
    }

    ComPtr<IDxcBlob> code;
    result->GetResult(&code);

    VkShaderModuleCreateInfo shaderModuleCI{};
    shaderModuleCI.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleCI.codeSize = code->GetBufferSize();
    shaderModuleCI.pCode = (uint32_t*)code->GetBufferPointer();
    VkShaderModule shaderModule;
    vkCreateShaderModule(device, &shaderModuleCI, nullptr, &shaderModule);

    return shaderModule;
}