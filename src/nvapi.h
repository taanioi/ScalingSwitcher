#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <stdarg.h>
#include <stdint.h>

enum class NvScaling : int32_t {
    Default            = 0,
    DisplayFullScreen  = 1,
    GpuFullScreen      = 2,
    GpuNoScaling       = 3,
    GpuAspectRatio     = 5,
    DisplayAspectRatio = 6,
    DisplayNoScaling   = 7,
    GpuIntegerScaling  = 8,
    Customized         = 255
};

#pragma pack(push, 8)

struct NV_RESOLUTION {
    uint32_t width;
    uint32_t height;
    uint32_t colorDepth;
};

struct NV_POSITION {
    int32_t x;
    int32_t y;
};

struct NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1 {
    NV_RESOLUTION resolution;
    uint32_t      colorFormat;
    NV_POSITION   position;
    uint32_t      spanningOrientation;
    uint32_t      flags;
};

struct NV_TIMINGEXT {
    uint32_t flag;
    uint16_t rr;
    uint32_t rrx1k;
    uint32_t aspect;
    uint16_t rep;
    uint32_t status;
    uint8_t  name[40];
};

struct NV_TIMING {
    uint16_t     HVisible;
    uint16_t     HBorder;
    uint16_t     HFrontPorch;
    uint16_t     HSyncWidth;
    uint16_t     HTotal;
    uint8_t      HSyncPol;
    uint16_t     VVisible;
    uint16_t     VBorder;
    uint16_t     VFrontPorch;
    uint16_t     VSyncWidth;
    uint16_t     VTotal;
    uint8_t      VSyncPol;
    uint16_t     interlaced;
    uint32_t     pclk;
    NV_TIMINGEXT etc;
};

struct NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1 {
    uint32_t  version;
    int32_t   rotation;
    int32_t   scaling;
    uint32_t  refreshRate1K;
    uint32_t  flags;
    int32_t   connector;
    uint32_t  tvFormat;
    int32_t   timingOverride;
    NV_TIMING timing;
};

struct NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2 {
    uint32_t                                       displayId;
    NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1* details;
    uint32_t                                       targetId;
};

struct NV_DISPLAYCONFIG_PATH_INFO_V2 {
    uint32_t                              version;
    uint32_t                              sourceId;
    uint32_t                              targetInfoCount;
    NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2* targetInfo;
    NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1* sourceModeInfo;
    uint32_t                              flags;
    void*                                 osAdapterID;
};

#pragma pack(pop)

static_assert(sizeof(NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1) == 32, "NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1 must be 32 bytes");
static_assert(sizeof(NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1) == 128, "NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1 must be 128 bytes");
static_assert(sizeof(NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2) == 24, "NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2 must be 24 bytes");
static_assert(sizeof(NV_DISPLAYCONFIG_PATH_INFO_V2) == 48, "NV_DISPLAYCONFIG_PATH_INFO_V2 must be 48 bytes");

constexpr uint32_t MakeNvVersion(size_t structSize, uint32_t version) {
    return (version << 16) | static_cast<uint32_t>(structSize);
}

struct NvDisplayInfo {
    wchar_t   displayName[32];
    wchar_t   monitorName[128];
    uint32_t  displayId;
    uint32_t  width;
    uint32_t  height;
    uint32_t  refreshRateMillihertz;
    int32_t   posX;
    int32_t   posY;
    NvScaling scaling;
    bool      foundInConfig;
    bool      isPrimary;
};

struct NvApplyResult {
    bool      success;
    bool      haveActual;     // actualScaling/width/height/refresh are a fresh driver readback
    NvScaling actualScaling;
    uint32_t  width;
    uint32_t  height;
    uint32_t  refreshRateMillihertz;
    wchar_t   message[256];
};

namespace StrUtil {

constexpr size_t Len(const wchar_t* s) {
    if (!s) return 0;
    size_t n = 0;
    while (s[n] != L'\0') ++n;
    return n;
}

constexpr int AsciiCmpI(const wchar_t* a, const wchar_t* b) {
    if (!a || !b) return (a == b) ? 0 : (a ? 1 : -1);
    for (;;) {
        uint32_t ca = static_cast<uint16_t>(*a++);
        uint32_t cb = static_cast<uint16_t>(*b++);
        if (ca >= L'A' && ca <= L'Z') ca += 32u;
        if (cb >= L'A' && cb <= L'Z') cb += 32u;
        if (ca != cb || ca == 0u) return (ca > cb) - (ca < cb);
    }
}

constexpr bool AsciiEqualsI(const wchar_t* a, const wchar_t* b) {
    return AsciiCmpI(a, b) == 0;
}

__attribute__((optimize("no-tree-loop-distribute-patterns")))
inline void Zero(void* dst, size_t bytes) {
    auto* p = static_cast<uint8_t*>(dst);
    while (bytes--) *p++ = 0;
}

inline void SafeCopy(wchar_t* dst, size_t dstCount, const wchar_t* src) {
    if (!dst || dstCount == 0) return;
    if (!src) {
        dst[0] = L'\0';
        return;
    }
    size_t i = 0;
    while (i + 1 < dstCount && src[i] != L'\0') {
        dst[i] = src[i];
        ++i;
    }
    dst[i] = L'\0';
}

inline void AppendUint(wchar_t* dst, size_t dstCount, size_t& pos, uint32_t val) {
    wchar_t digits[16];
    size_t len = 0;
    do {
        digits[len++] = static_cast<wchar_t>(L'0' + (val % 10u));
        val /= 10u;
    } while (val != 0u);
    while (len > 0 && pos + 1 < dstCount) {
        dst[pos++] = digits[--len];
    }
}

inline void SafeFormat(wchar_t* dst, size_t dstCount, const wchar_t* fmt, ...) {
    if (!dst || dstCount == 0) return;
    if (!fmt) {
        dst[0] = L'\0';
        return;
    }
    size_t pos = 0;
    va_list args;
    va_start(args, fmt);
    while (*fmt != L'\0' && pos + 1 < dstCount) {
        if (*fmt != L'%') {
            dst[pos++] = *fmt++;
            continue;
        }
        ++fmt;
        if (*fmt == L'\0') {
            break;
        }
        switch (*fmt++) {
            case L'%':
                dst[pos++] = L'%';
                break;
            case L's': {
                const wchar_t* s = va_arg(args, const wchar_t*);
                if (s) {
                    while (*s != L'\0' && pos + 1 < dstCount) {
                        dst[pos++] = *s++;
                    }
                }
                break;
            }
            case L'u': {
                uint32_t u = va_arg(args, unsigned int);
                AppendUint(dst, dstCount, pos, u);
                break;
            }
            case L'd': {
                int v = va_arg(args, int);
                uint32_t mag = static_cast<uint32_t>(v);
                if (v < 0) {
                    dst[pos++] = L'-';
                    mag = 0u - mag;
                }
                AppendUint(dst, dstCount, pos, mag);
                break;
            }
            default:
                dst[pos++] = L'%';
                if (pos + 1 < dstCount) {
                    dst[pos++] = *(fmt - 1);
                }
                break;
        }
    }
    va_end(args);
    dst[pos] = L'\0';
}

} // namespace StrUtil

namespace NvApi {

constexpr uint32_t ID_Initialize                      = 0x0150E828;
constexpr uint32_t ID_Unload                          = 0xD22BDD7E;
constexpr uint32_t ID_DISP_GetDisplayConfig           = 0x11ABCCF8;
constexpr uint32_t ID_DISP_SetDisplayConfig           = 0x5D8CF8DE;
constexpr uint32_t ID_EnumNvidiaDisplayHandle         = 0x9ABDD40D;
constexpr uint32_t ID_GetAssociatedNvidiaDisplayName  = 0x22A78B05;
constexpr uint32_t ID_DISP_GetDisplayIdByDisplayName  = 0xAE457190;
constexpr uint32_t ID_GetErrorMessage                 = 0x6C2D048C;

constexpr int32_t  NVAPI_OK                           = 0;
constexpr int32_t  NVAPI_DEVICE_BUSY                  = -108;
constexpr uint32_t NV_DISPLAYCONFIG_SAVE_TO_PERSISTENCE = 0x00000002;
constexpr uint32_t NV_FLAG_NON_NVIDIA_ADAPTER         = 0x00000001;

using FnVoid                           = void    (*)();
using FnQueryInterface                 = void*   (__cdecl*)(uint32_t id);
using FnInitialize                     = int32_t (__cdecl*)();
using FnUnload                         = int32_t (__cdecl*)();
using FnGetDisplayConfig               = int32_t (__cdecl*)(uint32_t* pathInfoCount, NV_DISPLAYCONFIG_PATH_INFO_V2* pathInfo);
using FnSetDisplayConfig               = int32_t (__cdecl*)(uint32_t pathInfoCount, NV_DISPLAYCONFIG_PATH_INFO_V2* pathInfo, uint32_t flags);
using FnEnumNvidiaDisplayHandle        = int32_t (__cdecl*)(uint32_t thisEnum, void** pNvDispHandle);
using FnGetAssociatedNvidiaDisplayName = int32_t (__cdecl*)(void* nvDispHandle, char szDisplayName[64]);
using FnGetDisplayIdByDisplayName      = int32_t (__cdecl*)(const char* displayName, uint32_t* displayId);
using FnGetErrorMessage                = int32_t (__cdecl*)(int32_t nr, char szDesc[64]);

enum ProcIdx {
    P_INIT = 0,
    P_UNLOAD,
    P_GET_CONFIG,
    P_SET_CONFIG,
    P_ENUM_DISP,
    P_DISP_NAME,
    P_DISP_ID,
    P_ERR_MSG,
    P_COUNT
};

// Zero-initialised statics (.bss): single translation unit, so no inline-variable COMDATs in .data.
static HMODULE          g_module;
static FnQueryInterface g_queryInterface;
static FnVoid           g_procs[P_COUNT];
static bool             g_initialized;

template <class Fn> inline Fn Proc(ProcIdx idx) {
    return reinterpret_cast<Fn>(g_procs[idx]);
}

inline const wchar_t* GetScalingLabel(NvScaling scaling) {
    static constexpr const wchar_t* kLabels[9] = {
        L"Default",
        L"Display - Full-screen",
        L"GPU - Full-screen",
        L"GPU - No Scaling",
        L"Default",
        L"GPU - Aspect Ratio",
        L"Display - Aspect Ratio",
        L"Display - No Scaling",
        L"GPU - Integer Scaling",
    };
    const uint32_t idx = static_cast<uint32_t>(scaling);
    if (idx < 9u) return kLabels[idx];
    return (scaling == NvScaling::Customized) ? L"Custom" : L"Default";
}

inline void FormatStatusText(int32_t status, wchar_t* outBuf, size_t outCount) {
    if (auto getErr = Proc<FnGetErrorMessage>(P_ERR_MSG)) {
        char desc[64] = {};
        if (getErr(status, desc) == NVAPI_OK && desc[0] != '\0') {
            desc[63] = '\0';
            if (MultiByteToWideChar(CP_ACP, 0, desc, -1, outBuf, static_cast<int>(outCount)) > 0) {
                return;
            }
        }
    }
    StrUtil::SafeFormat(outBuf, outCount, L"NVAPI status %d", status);
}

inline void UnloadModule() {
    g_queryInterface = nullptr;
    StrUtil::Zero(g_procs, sizeof(g_procs));
    if (g_module) {
        FreeLibrary(g_module);
        g_module = nullptr;
    }
}

inline void Shutdown() {
    if (g_initialized) {
        if (auto unload = Proc<FnUnload>(P_UNLOAD)) {
            unload();
        }
    }
    g_initialized = false;
    UnloadModule();
}

inline bool Initialize(wchar_t* errBuf = nullptr, size_t errCount = 0) {
    if (g_initialized) {
        return true;
    }

    g_module = LoadLibraryExW(L"nvapi64.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!g_module) {
        if (errBuf && errCount) {
            StrUtil::SafeCopy(errBuf, errCount,
                L"nvapi64.dll was not found in System32. Make sure NVIDIA drivers are installed.");
        }
        return false;
    }

    g_queryInterface = reinterpret_cast<FnQueryInterface>(
        reinterpret_cast<FnVoid>(GetProcAddress(g_module, "nvapi_QueryInterface")));
    if (!g_queryInterface) {
        if (errBuf && errCount) {
            StrUtil::SafeCopy(errBuf, errCount,
                L"nvapi_QueryInterface entry point was not found in nvapi64.dll.");
        }
        UnloadModule();
        return false;
    }

    static constexpr uint32_t kIds[P_COUNT] = {
        ID_Initialize,
        ID_Unload,
        ID_DISP_GetDisplayConfig,
        ID_DISP_SetDisplayConfig,
        ID_EnumNvidiaDisplayHandle,
        ID_GetAssociatedNvidiaDisplayName,
        ID_DISP_GetDisplayIdByDisplayName,
        ID_GetErrorMessage,
    };
    for (int i = 0; i < P_COUNT; ++i) {
        g_procs[i] = reinterpret_cast<FnVoid>(g_queryInterface(kIds[i]));
        if (i != P_UNLOAD && i != P_ERR_MSG && !g_procs[i]) {
            if (errBuf && errCount) {
                StrUtil::SafeCopy(errBuf, errCount,
                    L"Required NVAPI display configuration functions are missing.");
            }
            UnloadModule();
            return false;
        }
    }

    int32_t status = Proc<FnInitialize>(P_INIT)();
    if (status != NVAPI_OK) {
        if (errBuf && errCount) {
            wchar_t st[64];
            FormatStatusText(status, st, 64);
            StrUtil::SafeFormat(errBuf, errCount, L"NvAPI_Initialize failed: %s (%d)", st, status);
        }
        UnloadModule();
        return false;
    }

    g_initialized = true;
    return true;
}

class DisplayConfigSnapshot {
public:
    NV_DISPLAYCONFIG_PATH_INFO_V2* paths     = nullptr;
    uint32_t                       pathCount = 0;

    DisplayConfigSnapshot() = default;
    DisplayConfigSnapshot(const DisplayConfigSnapshot&) = delete;
    DisplayConfigSnapshot& operator=(const DisplayConfigSnapshot&) = delete;

    ~DisplayConfigSnapshot() {
        FreeAll();
    }

    bool Query(int32_t* outStatus = nullptr) {
        FreeAll();

        auto getDisplayConfig = Proc<FnGetDisplayConfig>(P_GET_CONFIG);
        uint32_t count = 0;
        int32_t status = getDisplayConfig(&count, nullptr);
        if (status != NVAPI_OK) {
            if (outStatus) *outStatus = status;
            return false;
        }

        if (count == 0) {
            if (outStatus) *outStatus = NVAPI_OK;
            return true;
        }
        if (count > 256) return Fail(outStatus, -1);

        const uint32_t allocCount = count;
        const size_t pathBytes = static_cast<size_t>(allocCount) * sizeof(NV_DISPLAYCONFIG_PATH_INFO_V2);
        if (pathBytes <= sizeof(m_pathBuf)) {
            StrUtil::Zero(m_pathBuf, pathBytes);
            paths = reinterpret_cast<NV_DISPLAYCONFIG_PATH_INFO_V2*>(m_pathBuf);
        } else {
            m_heapPaths = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pathBytes);
            if (!m_heapPaths) return Fail(outStatus, -1);
            paths = static_cast<NV_DISPLAYCONFIG_PATH_INFO_V2*>(m_heapPaths);
        }
        pathCount = allocCount;

        for (uint32_t i = 0; i < allocCount; ++i) {
            paths[i].version = MakeNvVersion(sizeof(NV_DISPLAYCONFIG_PATH_INFO_V2), 2);
        }

        // Pass 2: fetch target counts and adapter flags per path
        status = getDisplayConfig(&count, paths);
        if (status != NVAPI_OK) return Fail(outStatus, status);
        if (count > allocCount) return Fail(outStatus, NVAPI_DEVICE_BUSY);
        pathCount = count;

        size_t subBytes = 0;
        for (uint32_t i = 0; i < count; ++i) {
            const bool isNonNvidia = (paths[i].flags & NV_FLAG_NON_NVIDIA_ADAPTER) != 0;
            const uint32_t tc = paths[i].targetInfoCount;
            if (tc > 256) return Fail(outStatus, -1);
            if (isNonNvidia && paths[i].osAdapterID == nullptr) {
                subBytes += sizeof(LUID);
            }
            if (tc > 0) {
                subBytes += static_cast<size_t>(tc) * sizeof(NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2);
                if (!isNonNvidia) {
                    subBytes += static_cast<size_t>(tc) * sizeof(NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1);
                }
            }
            subBytes += sizeof(NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1);
        }

        uint8_t* cur = nullptr;
        if (subBytes <= sizeof(m_subBuf)) {
            StrUtil::Zero(m_subBuf, subBytes);
            cur = m_subBuf;
        } else {
            m_heapSub = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, subBytes);
            if (!m_heapSub) return Fail(outStatus, -1);
            cur = static_cast<uint8_t*>(m_heapSub);
        }

        auto carve = [&cur](size_t bytes) -> void* {
            void* p = cur;
            cur += bytes;
            return p;
        };

        for (uint32_t i = 0; i < count; ++i) {
            const bool isNonNvidia = (paths[i].flags & NV_FLAG_NON_NVIDIA_ADAPTER) != 0;
            if (isNonNvidia && paths[i].osAdapterID == nullptr) {
                paths[i].osAdapterID = carve(sizeof(LUID));
            }

            const uint32_t tc = paths[i].targetInfoCount;
            if (tc > 0) {
                auto* targets = static_cast<NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2*>(
                    carve(static_cast<size_t>(tc) * sizeof(NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2)));

                if (!isNonNvidia) {
                    auto* detailsArr = static_cast<NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1*>(
                        carve(static_cast<size_t>(tc) * sizeof(NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1)));
                    for (uint32_t j = 0; j < tc; ++j) {
                        detailsArr[j].version = MakeNvVersion(
                            sizeof(NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1), 1);
                        targets[j].details = &detailsArr[j];
                    }
                }
                paths[i].targetInfo = targets;
            }

            paths[i].sourceModeInfo = static_cast<NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1*>(
                carve(sizeof(NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1)));
        }

        // Pass 3: populate full path & target details
        status = getDisplayConfig(&count, paths);
        if (status != NVAPI_OK) return Fail(outStatus, status);
        if (count > allocCount) return Fail(outStatus, NVAPI_DEVICE_BUSY);
        pathCount = count;
        if (outStatus) *outStatus = NVAPI_OK;
        return true;
    }

    bool TryGetTarget(uint32_t displayId, NvScaling& outScaling, uint32_t& outWidth,
                      uint32_t& outHeight, uint32_t& outRefreshMillihertz,
                      int32_t* outPosX = nullptr, int32_t* outPosY = nullptr,
                      bool* outIsPrimary = nullptr) const {
        return VisitTarget(displayId, nullptr, outScaling, outWidth, outHeight,
                           outRefreshMillihertz, outPosX, outPosY, outIsPrimary);
    }

    bool PatchScaling(uint32_t displayId, NvScaling scaling,
                      NvScaling& outPrevScaling, uint32_t& outWidth,
                      uint32_t& outHeight, uint32_t& outRefreshMillihertz) {
        return VisitTarget(displayId, &scaling, outPrevScaling, outWidth, outHeight,
                           outRefreshMillihertz);
    }

private:
    bool VisitTarget(uint32_t displayId, const NvScaling* newScaling,
                     NvScaling& outScaling, uint32_t& outWidth,
                     uint32_t& outHeight, uint32_t& outRefreshMillihertz,
                     int32_t* outPosX = nullptr, int32_t* outPosY = nullptr,
                     bool* outIsPrimary = nullptr) const {
        bool found = false;
        for (uint32_t i = 0; i < pathCount; ++i) {
            const auto& path = paths[i];
            if ((path.flags & NV_FLAG_NON_NVIDIA_ADAPTER) != 0 || !path.targetInfo) continue;

            const uint32_t w = path.sourceModeInfo ? path.sourceModeInfo->resolution.width : 0;
            const uint32_t h = path.sourceModeInfo ? path.sourceModeInfo->resolution.height : 0;

            for (uint32_t j = 0; j < path.targetInfoCount; ++j) {
                const auto& t = path.targetInfo[j];
                if (t.displayId == displayId && t.details) {
                    if (!found) {
                        outScaling           = static_cast<NvScaling>(t.details->scaling);
                        outWidth             = w;
                        outHeight            = h;
                        outRefreshMillihertz = t.details->refreshRate1K;
                        if (outPosX)      *outPosX      = path.sourceModeInfo ? path.sourceModeInfo->position.x : 0;
                        if (outPosY)      *outPosY      = path.sourceModeInfo ? path.sourceModeInfo->position.y : 0;
                        if (outIsPrimary) *outIsPrimary = path.sourceModeInfo && ((path.sourceModeInfo->flags & 1u) != 0);
                        found = true;
                        if (!newScaling) return true;
                    }
                    if (newScaling) {
                        t.details->scaling = static_cast<int32_t>(*newScaling);
                    }
                }
            }
        }
        return found;
    }
    static constexpr size_t kInlinePaths = 4;
    static constexpr size_t kSubPerPath  =
        sizeof(NV_DISPLAYCONFIG_PATH_TARGET_INFO_V2) +
        sizeof(NV_DISPLAYCONFIG_PATH_ADVANCED_TARGET_INFO_V1) +
        sizeof(NV_DISPLAYCONFIG_SOURCE_MODE_INFO_V1) +
        sizeof(LUID);

    alignas(8) uint8_t m_pathBuf[kInlinePaths * sizeof(NV_DISPLAYCONFIG_PATH_INFO_V2)];
    alignas(8) uint8_t m_subBuf[kInlinePaths * kSubPerPath];
    void*              m_heapPaths = nullptr;
    void*              m_heapSub   = nullptr;

    bool Fail(int32_t* outStatus, int32_t code) {
        if (outStatus) *outStatus = code;
        FreeAll();
        return false;
    }

    void FreeAll() {
        HANDLE heap = (m_heapPaths || m_heapSub) ? GetProcessHeap() : nullptr;
        if (m_heapPaths) {
            HeapFree(heap, 0, m_heapPaths);
            m_heapPaths = nullptr;
        }
        if (m_heapSub) {
            HeapFree(heap, 0, m_heapSub);
            m_heapSub = nullptr;
        }
        paths     = nullptr;
        pathCount = 0;
    }
};

// Resolves EDID monitor names directly into displays[] in one CCD pass, with GDI fallback.
inline void ResolveMonitorNames(NvDisplayInfo* displays, size_t count) {
    size_t resolved = 0;
    UINT32 numPaths = 0, numModes = 0;
    if (GetDisplayConfigBufferSizes(QDC_ONLY_ACTIVE_PATHS, &numPaths, &numModes) == ERROR_SUCCESS &&
        numPaths > 0 && numPaths <= 256) {
        DISPLAYCONFIG_PATH_INFO stackPaths[8];
        DISPLAYCONFIG_MODE_INFO stackModes[16];
        HANDLE heap = (numPaths > 8 || numModes > 16) ? GetProcessHeap() : nullptr;
        auto* heapPaths = (numPaths > 8)
            ? static_cast<DISPLAYCONFIG_PATH_INFO*>(HeapAlloc(heap, 0, numPaths * sizeof(DISPLAYCONFIG_PATH_INFO)))
            : nullptr;
        auto* heapModes = (numModes > 16)
            ? static_cast<DISPLAYCONFIG_MODE_INFO*>(HeapAlloc(heap, 0, numModes * sizeof(DISPLAYCONFIG_MODE_INFO)))
            : nullptr;

        DISPLAYCONFIG_PATH_INFO* paths = (numPaths > 8) ? heapPaths : stackPaths;
        DISPLAYCONFIG_MODE_INFO* modes = (numModes > 16) ? heapModes : stackModes;

        if (paths && (numModes == 0 || modes) &&
            QueryDisplayConfig(QDC_ONLY_ACTIVE_PATHS, &numPaths, paths, &numModes, modes, nullptr) == ERROR_SUCCESS) {
            for (UINT32 i = 0; i < numPaths && resolved < count; ++i) {
                DISPLAYCONFIG_SOURCE_DEVICE_NAME srcName = {};
                srcName.header.type      = DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME;
                srcName.header.size      = sizeof(srcName);
                srcName.header.adapterId = paths[i].sourceInfo.adapterId;
                srcName.header.id        = paths[i].sourceInfo.id;
                if (DisplayConfigGetDeviceInfo(&srcName.header) != ERROR_SUCCESS ||
                    srcName.viewGdiDeviceName[0] == L'\0') {
                    continue;
                }

                size_t matchIdx = count;
                for (size_t k = 0; k < count; ++k) {
                    if (displays[k].monitorName[0] == L'\0' &&
                        StrUtil::AsciiEqualsI(displays[k].displayName, srcName.viewGdiDeviceName)) {
                        matchIdx = k;
                        break;
                    }
                }
                if (matchIdx == count) continue;

                DISPLAYCONFIG_TARGET_DEVICE_NAME targetName = {};
                targetName.header.type      = DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME;
                targetName.header.size      = sizeof(targetName);
                targetName.header.adapterId = paths[i].targetInfo.adapterId;
                targetName.header.id        = paths[i].targetInfo.id;
                if (DisplayConfigGetDeviceInfo(&targetName.header) == ERROR_SUCCESS &&
                    targetName.monitorFriendlyDeviceName[0] != L'\0') {
                    StrUtil::SafeCopy(displays[matchIdx].monitorName, 128, targetName.monitorFriendlyDeviceName);
                    ++resolved;
                }
            }
        }

        if (heapPaths) HeapFree(heap, 0, heapPaths);
        if (heapModes) HeapFree(heap, 0, heapModes);
    }

    bool needPrimaryFallback = false;
    for (size_t k = 0; k < count; ++k) {
        if (!displays[k].foundInConfig) needPrimaryFallback = true;
        if (displays[k].monitorName[0] != L'\0') continue;
        DISPLAY_DEVICEW dd = {};
        dd.cb = sizeof(dd);
        if (EnumDisplayDevicesW(displays[k].displayName, 0, &dd, 0) && dd.DeviceString[0] != L'\0') {
            StrUtil::SafeCopy(displays[k].monitorName, 128, dd.DeviceString);
        }
    }

    if (needPrimaryFallback) {
        for (DWORD devIdx = 0; ; ++devIdx) {
            DISPLAY_DEVICEW dd = {};
            dd.cb = sizeof(dd);
            if (!EnumDisplayDevicesW(nullptr, devIdx, &dd, 0)) break;
            if ((dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) != 0) {
                for (size_t k = 0; k < count; ++k) {
                    if (!displays[k].foundInConfig &&
                        StrUtil::AsciiEqualsI(dd.DeviceName, displays[k].displayName)) {
                        displays[k].isPrimary = true;
                    }
                }
                break;
            }
        }
    }
}

inline const wchar_t* ShortDisplayName(const wchar_t* gdiName) {
    if (!gdiName) return L"";
    if (gdiName[0] == L'\\' && gdiName[1] == L'\\' && gdiName[2] == L'.' && gdiName[3] == L'\\') {
        return gdiName + 4;
    }
    return gdiName;
}

inline const wchar_t* GetSpatialHint(const NvDisplayInfo* displays, size_t count, size_t idx) {
    if (!displays || idx >= count) return nullptr;
    const NvDisplayInfo& target = displays[idx];
    if (target.isPrimary) return L"Primary";
    if (count <= 1 || !target.foundInConfig) return nullptr;

    uint32_t primW = 0, primH = 0;
    for (size_t i = 0; i < count; ++i) {
        if (displays[i].foundInConfig &&
            (displays[i].isPrimary || (displays[i].posX == 0 && displays[i].posY == 0))) {
            primW = displays[i].width;
            primH = displays[i].height;
            if (displays[i].isPrimary) break;
        }
    }

    const int32_t cx = target.posX + static_cast<int32_t>(target.width / 2u);
    const int32_t cy = target.posY + static_cast<int32_t>(target.height / 2u);
    if (cx < 0) return L"Left";
    if (primW > 0 && cx >= static_cast<int32_t>(primW)) return L"Right";
    if (cy < 0) return L"Above";
    if (primH > 0 && cy >= static_cast<int32_t>(primH)) return L"Below";
    return nullptr;
}

inline void SortDisplays(NvDisplayInfo* displays, size_t count) {
    auto comesBefore = [](const NvDisplayInfo& a, const NvDisplayInfo& b) -> bool {
        if (a.foundInConfig != b.foundInConfig) return a.foundInConfig;
        if (a.isPrimary != b.isPrimary) return a.isPrimary;
        if (a.posX != b.posX) return a.posX < b.posX;
        if (a.posY != b.posY) return a.posY < b.posY;
        return StrUtil::AsciiCmpI(a.displayName, b.displayName) < 0;
    };
    for (size_t i = 1; i < count; ++i) {
        NvDisplayInfo key = displays[i];
        size_t j = i;
        while (j > 0 && comesBefore(key, displays[j - 1])) {
            displays[j] = displays[j - 1];
            --j;
        }
        displays[j] = key;
    }
}

inline size_t GetDisplays(NvDisplayInfo* outDisplays, size_t maxDisplays) {
    if (!Initialize() || !outDisplays || maxDisplays == 0) {
        return 0;
    }

    auto enumDisp  = Proc<FnEnumNvidiaDisplayHandle>(P_ENUM_DISP);
    auto getName   = Proc<FnGetAssociatedNvidiaDisplayName>(P_DISP_NAME);
    auto getDispId = Proc<FnGetDisplayIdByDisplayName>(P_DISP_ID);

    size_t count = 0;
    for (uint32_t index = 0; count < maxDisplays; ++index) {
        void* handle = nullptr;
        if (enumDisp(index, &handle) != NVAPI_OK) {
            break;
        }

        char ansiName[64] = {};
        if (getName(handle, ansiName) != NVAPI_OK || ansiName[0] == '\0') {
            continue;
        }
        ansiName[63] = '\0';

        uint32_t displayId = 0;
        if (getDispId(ansiName, &displayId) != NVAPI_OK) {
            continue;
        }

        NvDisplayInfo& info = outDisplays[count];
        StrUtil::Zero(&info, sizeof(info));
        if (MultiByteToWideChar(CP_ACP, 0, ansiName, -1, info.displayName, 32) <= 0) {
            continue;
        }
        info.displayId = displayId;
        ++count;
    }

    if (count == 0) {
        return 0;
    }

    {
        DisplayConfigSnapshot config;
        if (config.Query()) {
            for (size_t i = 0; i < count; ++i) {
                NvDisplayInfo& info = outDisplays[i];
                if (config.TryGetTarget(info.displayId, info.scaling, info.width,
                                        info.height, info.refreshRateMillihertz,
                                        &info.posX, &info.posY, &info.isPrimary)) {
                    info.foundInConfig = true;
                }
            }
        }
    }
    ResolveMonitorNames(outDisplays, count);
    if (count > 1) {
        SortDisplays(outDisplays, count);
    }

    return count;
}

inline NvApplyResult SetScaling(uint32_t displayId, NvScaling scaling) {
    NvApplyResult result = {};

    wchar_t initErr[128] = {};
    if (!Initialize(initErr, 128)) {
        StrUtil::SafeCopy(result.message, 256, initErr);
        return result;
    }

    DisplayConfigSnapshot snap;
    NvScaling prevScaling = NvScaling::Default;
    uint32_t  prevW = 0, prevH = 0, prevHz = 0;
    bool      hadPrev = false;

    int32_t lastStatus = 0;
    for (int attempt = 0; attempt < 5; ++attempt) {
        if (!snap.Query(&lastStatus)) {
            if (lastStatus == NVAPI_DEVICE_BUSY) {
                Sleep(150);
                continue;
            }
            break;
        }
        hadPrev = snap.PatchScaling(displayId, scaling, prevScaling, prevW, prevH, prevHz);
        if (!hadPrev) {
            StrUtil::SafeCopy(result.message, 256,
                L"The selected display was not found in the NVIDIA display configuration.");
            return result;
        }

        lastStatus = Proc<FnSetDisplayConfig>(P_SET_CONFIG)(
            snap.pathCount, snap.paths, NV_DISPLAYCONFIG_SAVE_TO_PERSISTENCE);
        if (lastStatus == NVAPI_DEVICE_BUSY) {
            Sleep(150);
            continue;
        }
        break;
    }

    if (lastStatus != NVAPI_OK) {
        if (hadPrev) {
            result.haveActual            = true;
            result.actualScaling         = prevScaling;
            result.width                 = prevW;
            result.height                = prevH;
            result.refreshRateMillihertz = prevHz;
        }
        wchar_t statusText[64];
        FormatStatusText(lastStatus, statusText, 64);
        StrUtil::SafeFormat(result.message, 256,
            L"Could not apply scaling: %s (%d).", statusText, lastStatus);
        return result;
    }

    // Verify with fresh readback from NVIDIA driver (retry briefly if driver is re-syncing).
    // The readback also feeds the caller's display state, so no second full query is needed.
    NvScaling& actualScaling = result.actualScaling;
    bool&      haveActual    = result.haveActual;
    for (int attempt = 0; attempt < 3; ++attempt) {
        int32_t verifyStatus = 0;
        if (snap.Query(&verifyStatus)) {
            haveActual = snap.TryGetTarget(displayId, actualScaling, result.width,
                                           result.height, result.refreshRateMillihertz);
            break;
        }
        if (verifyStatus == NVAPI_DEVICE_BUSY) {
            Sleep(150);
            continue;
        }
        break;
    }

    if (haveActual && actualScaling == scaling) {
        result.success = true;
        StrUtil::SafeFormat(result.message, 256,
            L"Applied and saved: %s.", GetScalingLabel(actualScaling));
    } else if (haveActual) {
        StrUtil::SafeFormat(result.message, 256,
            L"Driver kept %s (could not apply %s).",
            GetScalingLabel(actualScaling), GetScalingLabel(scaling));
    } else {
        StrUtil::SafeCopy(result.message, 256,
            L"Could not verify applied scaling from NVIDIA driver.");
    }
    return result;
}

} // namespace NvApi
