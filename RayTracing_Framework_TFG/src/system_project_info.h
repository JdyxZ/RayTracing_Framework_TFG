#ifndef SYSTEM_PROJECT_INFO_H
#define SYSTEM_PROJECT_INFO_H

#include <comdef.h>
#include <tlhelp32.h>
#include <wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

struct SystemProjectInfo
{
public:

    // Singleton pattern
    static SystemProjectInfo& instance() 
    {
        static SystemProjectInfo _instance; 
        return _instance;
    }

    // System 
    string platform = getPlatform();
    int cpu_threads = getActiveThreads();

    // Project 
    string version = "1.0";
    string build_configuration = getBuildConfiguration();
    string compiler = getCompilerInfo();

private:

    string GetCPUModel()
    {
        array<int, 4> cpuInfo;
        array<char, 64> cpuBrand = { 0 };

        // Get CPU vendor (GenuineIntel or AuthenticAMD)
        __cpuid(cpuInfo.data(), 0);
        const string vendor = string(reinterpret_cast<char*>(&cpuInfo[1]), 4) +
            string(reinterpret_cast<char*>(&cpuInfo[3]), 4) +
            string(reinterpret_cast<char*>(&cpuInfo[2]), 4);

        // Get CPU brand string (e.g., "Intel i7-9700K" or "AMD Ryzen 9 5950X")
        __cpuid(cpuInfo.data(), 0x80000002);
        memcpy(cpuBrand.data(), cpuInfo.data(), sizeof(cpuInfo));
        __cpuid(cpuInfo.data(), 0x80000003);
        memcpy(cpuBrand.data() + 16, cpuInfo.data(), sizeof(cpuInfo));
        __cpuid(cpuInfo.data(), 0x80000004);
        memcpy(cpuBrand.data() + 32, cpuInfo.data(), sizeof(cpuInfo));

        return cpuBrand.data();
    }

    string GetGPUModel()
    {
        HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres)) return "WMI initialization failed";

        IWbemLocator* pLoc = nullptr;
        hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
        if (FAILED(hres)) return "Failed to create WbemLocator";

        IWbemServices* pSvc = nullptr;
        hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
        if (FAILED(hres))
        {
            pLoc->Release();
            return "Failed to connect to WMI";
        }

        hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            return "Failed to set proxy blanket";
        }

        IEnumWbemClassObject* pEnumerator = nullptr;
        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT Name FROM Win32_VideoController"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator
        );

        if (FAILED(hres))
        {
            pSvc->Release();
            pLoc->Release();
            return "Failed to execute WMI query";
        }

        string gpuName;
        IWbemClassObject* pclsObj = nullptr;
        ULONG uReturn = 0;

        while (pEnumerator)
        {
            hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
            if (uReturn == 0) break;

            VARIANT vtProp;
            hres = pclsObj->Get(L"Name", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                gpuName = _bstr_t(vtProp.bstrVal);
                VariantClear(&vtProp);
            }
            pclsObj->Release();
        }

        pEnumerator->Release();
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();

        return gpuName.empty() ? "No GPU detected" : gpuName;
    }

    string getPlatform()
    {
        return trim(GetCPUModel()) + ", " + trim(GetGPUModel());
    }

    int getActiveThreads()
    {
        DWORD processId = GetCurrentProcessId();
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        DWORD threadCount = 0;

        if (snapshot != INVALID_HANDLE_VALUE)
        {
            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);

            if (Thread32First(snapshot, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == processId) {
                        threadCount++;
                    }
                } while (Thread32Next(snapshot, &te32));
            }
            CloseHandle(snapshot);
        }

        return threadCount;
    }

    string getBuildConfiguration()
    {
        #ifdef _DEBUG
                return "Debug";
        #elif defined(NDEBUG)
                return "Release";
        #else
                return "Unknown";
        #endif
    }

    string getCompilerInfo()
    {
        string info;

        // Compiler
        #if defined(_MSC_VER)
                info = "MSVC " + std::to_string(_MSC_VER);
        #elif defined(__clang__)
                info = "Clang " + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
        #elif defined(__GNUC__)
                info = "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
        #elif defined(__INTEL_COMPILER)
                info = "ICC " + std::to_string(__INTEL_COMPILER);
        #else
                info = "Unknown Compiler";
        #endif
        return info;
    }
};


#endif
