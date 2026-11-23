#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include "nvapi.h"

// ----------------------------------------------------------------------------
// Scaling Option Metadata
// ----------------------------------------------------------------------------
enum class ScalerGroup : uint8_t {
    Display = 0,
    Gpu     = 1,
};

struct ModeItem {
    NvScaling      value;
    const wchar_t* title;
    const wchar_t* desc;
};

static constexpr ModeItem kDisplayModes[] = {
    { NvScaling::DisplayNoScaling,   L"No Scaling",   L"1:1 unscaled centered by monitor" },
    { NvScaling::DisplayAspectRatio, L"Aspect Ratio", L"Preserve proportions using monitor scaler" },
    { NvScaling::DisplayFullScreen,  L"Full-screen",  L"Stretch to fill screen using monitor scaler" },
};
static constexpr size_t kDisplayModeCount = sizeof(kDisplayModes) / sizeof(kDisplayModes[0]);

static constexpr ModeItem kGpuModes[] = {
    { NvScaling::GpuIntegerScaling,  L"Integer Scaling", L"Crisp pixel-perfect integer scaling" },
    { NvScaling::GpuNoScaling,       L"No Scaling",      L"1:1 unscaled centered by GPU" },
    { NvScaling::GpuAspectRatio,     L"Aspect Ratio",    L"Preserve proportions using GPU" },
    { NvScaling::GpuFullScreen,      L"Full-screen",     L"Stretch to fill screen using GPU" },
};
static constexpr size_t kGpuModeCount = sizeof(kGpuModes) / sizeof(kGpuModes[0]);

// ----------------------------------------------------------------------------
// Fluent Theme Palettes (WinUI 3 tokens)
// ----------------------------------------------------------------------------
using Col = D2D1_COLOR_F;
using Pk  = uint32_t;   // packed 0xAARRGGBB (theme tables stay 4 bytes per colour)

static constexpr Pk Rgb(uint32_t hex, float a = 1.0f) {
    return (static_cast<uint32_t>(a * 255.0f + 0.5f) << 24) | (hex & 0xFFFFFFu);
}

static Col ToCol(Pk c) {
    constexpr float k = 1.0f / 255.0f;
    return { static_cast<float>((c >> 16) & 0xFF) * k, static_cast<float>((c >> 8) & 0xFF) * k,
             static_cast<float>(c & 0xFF) * k, static_cast<float>(c >> 24) * k };
}

struct ThemeColors {
    Pk canvas;
    Pk glow0, glow1;
    Pk card, cardHover, cardPress;
    Pk borderTop, borderMid, borderBot;
    float borderMidPos;
    Pk rowSel, rowHover, divider;
    Pk railFill, railBorder;
    Pk accent, accentHover, accentPress, accentDisabled, onAccent, onAccentDisabled;
    Pk text, textSec, textMuted, textDisabled;
    Pk badgeBg, badgeBorder, badgeText, badgeDot, neutralBg, neutralBorder;
    Pk ibBg[4], ibBorder[4];
    Pk ibIconOk, ibIconErr;
    Pk radioBorder;
};

static constexpr ThemeColors kDarkTheme = {
    /* canvas       */ Rgb(0x202020),
    /* glow         */ Rgb(0x0078D4, 0.34f), Rgb(0x60CDFF, 0.12f),
    /* card         */ Rgb(0xFFFFFF, 0.051f), Rgb(0xFFFFFF, 0.082f), Rgb(0xFFFFFF, 0.030f),
    /* border       */ Rgb(0xFFFFFF, 0.115f), Rgb(0xFFFFFF, 0.068f), Rgb(0xFFFFFF, 0.060f), 0.18f,
    /* row          */ Rgb(0xFFFFFF, 0.068f), Rgb(0xFFFFFF, 0.055f), Rgb(0xFFFFFF, 0.065f),
    /* rail         */ Rgb(0x000000, 0.22f), Rgb(0xFFFFFF, 0.070f),
    /* accent       */ Rgb(0x60CDFF), Rgb(0x56BDE8), Rgb(0x4CA9D4), Rgb(0xFFFFFF, 0.158f), Rgb(0x000000), Rgb(0xFFFFFF, 0.530f),
    /* text         */ Rgb(0xFFFFFF), Rgb(0xFFFFFF, 0.72f), Rgb(0xFFFFFF, 0.50f), Rgb(0xFFFFFF, 0.36f),
    /* badge        */ Rgb(0x6CCB5F, 0.14f), Rgb(0x6CCB5F, 0.32f), Rgb(0x7AE26E), Rgb(0x6CCB5F),
                       Rgb(0xFFFFFF, 0.06f), Rgb(0xFFFFFF, 0.08f),
    /* infobar bg   */ { Rgb(0xFFFFFF, 0.048f), Rgb(0x60CDFF, 0.11f), Rgb(0x393D1B, 0.85f), Rgb(0x442726, 0.88f) },
    /* infobar bd   */ { Rgb(0xFFFFFF, 0.080f), Rgb(0x60CDFF, 0.28f), Rgb(0x6CCB5F, 0.26f), Rgb(0xFF99A4, 0.28f) },
    /* infobar icon */ Rgb(0x6CCB5F), Rgb(0xFF99A4),
    /* radio        */ Rgb(0xFFFFFF, 0.54f),
};

static constexpr ThemeColors kLightTheme = {
    /* canvas       */ Rgb(0xF3F3F3),
    /* glow         */ Rgb(0x0078D4, 0.13f), Rgb(0x60CDFF, 0.05f),
    /* card         */ Rgb(0xFFFFFF, 0.74f), Rgb(0xFFFFFF, 0.92f), Rgb(0xF5F5F5, 0.95f),
    /* border       */ Rgb(0x000000, 0.060f), Rgb(0x000000, 0.070f), Rgb(0x000000, 0.150f), 0.82f,
    /* row          */ Rgb(0x005FB8, 0.065f), Rgb(0x000000, 0.035f), Rgb(0x000000, 0.060f),
    /* rail         */ Rgb(0x000000, 0.045f), Rgb(0x000000, 0.060f),
    /* accent       */ Rgb(0x005FB8), Rgb(0x196EC0), Rgb(0x327EC8), Rgb(0x000000, 0.217f), Rgb(0xFFFFFF), Rgb(0xFFFFFF),
    /* text         */ Rgb(0x1A1A1A), Rgb(0x000000, 0.62f), Rgb(0x000000, 0.45f), Rgb(0x000000, 0.36f),
    /* badge        */ Rgb(0x0F7B0F, 0.10f), Rgb(0x0F7B0F, 0.24f), Rgb(0x0F7B0F), Rgb(0x16A34A),
                       Rgb(0x000000, 0.05f), Rgb(0x000000, 0.08f),
    /* infobar bg   */ { Rgb(0xFFFFFF, 0.65f), Rgb(0xE8F3FC), Rgb(0xDFF6DD), Rgb(0xFDE7E9) },
    /* infobar bd   */ { Rgb(0x000000, 0.060f), Rgb(0x005FB8, 0.22f), Rgb(0x0F7B0F, 0.22f), Rgb(0xC42B1C, 0.22f) },
    /* infobar icon */ Rgb(0x0F7B0F), Rgb(0xC42B1C),
    /* radio        */ Rgb(0x000000, 0.45f),
};

// ----------------------------------------------------------------------------
// UI Geometry (all in DIPs; Direct2D scales to the monitor DPI)
// ----------------------------------------------------------------------------
static constexpr int kBaseWidth   = 468;
static constexpr int kBaseHeight  = 584;
static constexpr int IDI_APP_ICON = 101;
static constexpr UINT_PTR kTimerIdentify = 1;
static constexpr size_t   kMaxDropVis    = 6;
static constexpr float    kDropLeft      = 20.0f;
static constexpr float    kDropTop       = 166.0f;
static constexpr float    kDropRight     = 448.0f;
static constexpr float    kDropRowH      = 52.0f;

static constexpr D2D1_RECT_F kEyebrow     = { 20,  16, 260,  34 };
static constexpr D2D1_RECT_F kTitle       = { 20,  34, 260,  66 };
static constexpr D2D1_RECT_F kSecDisplay  = { 20,  78, 350,  98 };
static constexpr D2D1_RECT_F kIdentifyBtn = { 358, 74, 448,  98 };
static constexpr D2D1_RECT_F kDisplayCard = { 20, 102, 448, 162 };
static constexpr D2D1_RECT_F kSecScaler   = { 20, 176, 448, 196 };
static constexpr D2D1_RECT_F kRail        = { 20, 200, 448, 240 };
static constexpr D2D1_RECT_F kTabDisplay  = { 24, 204, 232, 236 };
static constexpr D2D1_RECT_F kTabGpu      = { 236, 204, 444, 236 };
static constexpr D2D1_RECT_F kSecMode     = { 20, 254, 448, 274 };
static constexpr D2D1_RECT_F kModeCard    = { 20, 278, 448, 470 };
static constexpr D2D1_RECT_F kRows[4] = {
    { 20, 278, 448, 326 }, { 20, 326, 448, 374 }, { 20, 374, 448, 422 }, { 20, 422, 448, 470 },
};
static constexpr D2D1_RECT_F kInfoBar     = { 20, 482, 448, 522 };
static constexpr D2D1_RECT_F kRefreshBtn  = { 236, 534, 336, 568 };
static constexpr D2D1_RECT_F kApplyBtn    = { 346, 534, 448, 568 };

enum class StatusKind : uint8_t {
    Neutral,
    Busy,
    Success,
    Error,
};

enum class HitTarget : uint8_t {
    None,
    DisplayCard,
    IdentifyBtn,
    SegmentDisplay,
    SegmentGpu,
    Row0,
    Row1,
    Row2,
    Row3,
    RefreshBtn,
    ApplyBtn,
    DropRow0,
    DropRow1,
    DropRow2,
    DropRow3,
    DropRow4,
    DropRow5,
};

enum Fmt { F_HERO, F_SECTION, F_ROWTITLE, F_BODY, F_STATUS, F_BUTTON, F_BADGE, F_NUM, F_IDNUM, F_COUNT };

// ----------------------------------------------------------------------------
// Consolidated Application & Graphics State (.bss, zero-initialised)
// ----------------------------------------------------------------------------
struct RenderCtx {
    HMODULE                   modD2d;
    HMODULE                   modDw;
    HMODULE                   modDwm;
    ID2D1Factory*             d2d;
    IDWriteFactory*           dw;
    ID2D1StrokeStyle*         stroke;
    ID2D1HwndRenderTarget*    rt;
    ID2D1SolidColorBrush*     brush;
    ID2D1LinearGradientBrush* border;
    ID2D1RadialGradientBrush* glow;
    ID2D1DCRenderTarget*      idRt;
    ID2D1SolidColorBrush*     idBrush;
    IDWriteTextFormat*        fmt[F_COUNT];
    Pk                        lastBrush;
};

struct AppState {
    const ThemeColors* theme;
    UINT               dpi;
    float              scale;
    float              invScale;
    float              twDisplay;
    float              twGpu;
    float              twRefresh;
    float              twIdentify;
    float              badgeWidths[11];
    HitTarget          hover;
    HitTarget          pressed;
    ScalerGroup        activeGroup;
    NvScaling          selectedMode;
    StatusKind         statusKind;
    bool               hasSelectedMode;
    bool               busy;
    bool               trackingMouse;
    bool               dropdownOpen;
    bool               identifyOpen;
    HCURSOR            curArrow;
    HCURSOR            curHand;
    HCURSOR            curWait;
    size_t             displayCount;
    size_t             selectedDispIdx;
    size_t             dropScroll;
    size_t             dropHighlight;
    wchar_t            secDisplayLabel[48];
    wchar_t            applyLabel[32];
    wchar_t            dispMetas[16][96];
    wchar_t            statusText[256];
    NvDisplayInfo      displays[16];
    HWND               identifyHwnds[16];
};

static RenderCtx g_gfx;
static AppState  g_ui;

using FnRegGetValueW          = LSTATUS (WINAPI*)(HKEY, LPCWSTR, LPCWSTR, DWORD, LPDWORD, PVOID, LPDWORD);
using FnDwmSetWindowAttribute = HRESULT (WINAPI*)(HWND, DWORD, LPCVOID, DWORD);
using FnD2D1CreateFactory     = HRESULT (WINAPI*)(D2D1_FACTORY_TYPE, REFIID, const D2D1_FACTORY_OPTIONS*, void**);
using FnDWriteCreateFactory   = HRESULT (WINAPI*)(DWRITE_FACTORY_TYPE, REFIID, IUnknown**);

static constexpr GUID kIidID2D1Factory   = { 0x06152247, 0x6f50, 0x465a, { 0x92, 0x45, 0x11, 0x8b, 0xfd, 0x3b, 0x60, 0x07 } };
static constexpr GUID kIidIDWriteFactory = { 0xb859ee5a, 0xd838, 0x4b5b, { 0xa2, 0xe8, 0x1a, 0xdc, 0x7d, 0x93, 0xdb, 0x48 } };

template <class T> static void Rel(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

template <class Fn> static Fn LoadProc(HMODULE mod, const char* name) {
    return reinterpret_cast<Fn>(reinterpret_cast<void(*)()>(GetProcAddress(mod, name)));
}

static void Repaint(HWND hwnd) {
    InvalidateRect(hwnd, nullptr, FALSE);
}

static int ScaleDpi(int px, UINT dpi) {
    return (px * static_cast<int>(dpi) + 48) / 96;
}

static constexpr const wchar_t* kBadgeLabels[11] = {
    L"No display",
    L"Not active",
    L"Default",
    L"Display \x00B7 Full-screen",
    L"GPU \x00B7 Full-screen",
    L"GPU \x00B7 No Scaling",
    L"GPU \x00B7 Aspect Ratio",
    L"Display \x00B7 Aspect Ratio",
    L"Display \x00B7 No Scaling",
    L"GPU \x00B7 Integer",
    L"Custom",
};

static size_t GetBadgeIndex(const NvDisplayInfo* disp) {
    if (!disp) return 0;
    if (!disp->foundInConfig) return 1;
    static constexpr uint8_t kMap[9] = { 2, 3, 4, 5, 2, 6, 7, 8, 9 };
    const uint32_t s = static_cast<uint32_t>(disp->scaling);
    if (s < 9u) return kMap[s];
    return (disp->scaling == NvScaling::Customized) ? 10u : 2u;
}

static void SetDpiState(UINT dpi) {
    g_ui.dpi      = dpi;
    g_ui.scale    = static_cast<float>(dpi) / 96.0f;
    g_ui.invScale = 96.0f / static_cast<float>(dpi);
    if (g_gfx.rt) {
        const float d = static_cast<float>(dpi);
        g_gfx.rt->SetDpi(d, d);
    }
}

// ----------------------------------------------------------------------------
// Direct2D / DirectWrite lifetime
// ----------------------------------------------------------------------------
static void ReleaseIdentifyGfx() {
    Rel(g_gfx.idBrush);
    Rel(g_gfx.idRt);
}

static void ReleaseTarget() {
    Rel(g_gfx.glow);
    Rel(g_gfx.border);
    Rel(g_gfx.brush);
    Rel(g_gfx.rt);
    g_gfx.lastBrush = 0u;
}

static bool FamilyExists(IDWriteFontCollection* coll, const wchar_t* name) {
    UINT32 idx    = 0;
    BOOL   exists = FALSE;
    return coll && SUCCEEDED(coll->FindFamilyName(name, &idx, &exists)) && exists;
}

static void MakeFmt(Fmt id, const wchar_t* face, float size, DWRITE_FONT_WEIGHT weight,
                    bool wrap, bool trim, DWRITE_TEXT_ALIGNMENT align) {
    IDWriteTextFormat* f = nullptr;
    if (FAILED(g_gfx.dw->CreateTextFormat(face, nullptr, weight, DWRITE_FONT_STYLE_NORMAL,
                                          DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", &f)) || !f) {
        return;
    }
    f->SetTextAlignment(align);
    f->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    f->SetWordWrapping(wrap ? DWRITE_WORD_WRAPPING_WRAP : DWRITE_WORD_WRAPPING_NO_WRAP);
    if (trim) {
        IDWriteInlineObject* sign = nullptr;
        if (SUCCEEDED(g_gfx.dw->CreateEllipsisTrimmingSign(f, &sign)) && sign) {
            DWRITE_TRIMMING tr = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
            f->SetTrimming(&tr, sign);
            sign->Release();
        }
    }
    g_gfx.fmt[id] = f;
}

static void ShutdownD2D() {
    ReleaseIdentifyGfx();
    ReleaseTarget();
    for (int i = 0; i < F_COUNT; ++i) {
        Rel(g_gfx.fmt[i]);
    }
    Rel(g_gfx.stroke);
    Rel(g_gfx.dw);
    Rel(g_gfx.d2d);
    if (g_gfx.modDw)  { FreeLibrary(g_gfx.modDw);  g_gfx.modDw  = nullptr; }
    if (g_gfx.modD2d) { FreeLibrary(g_gfx.modD2d); g_gfx.modD2d = nullptr; }
    if (g_gfx.modDwm) { FreeLibrary(g_gfx.modDwm); g_gfx.modDwm = nullptr; }
}

static float TextWidth(const wchar_t* s, Fmt f, float maxW);   // defined with the drawing helpers

static bool InitD2D() {
    g_gfx.modD2d = LoadLibraryExW(L"d2d1.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    g_gfx.modDw  = LoadLibraryExW(L"dwrite.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (!g_gfx.modD2d || !g_gfx.modDw) return false;

    auto createD2d = LoadProc<FnD2D1CreateFactory>(g_gfx.modD2d, "D2D1CreateFactory");
    auto createDw  = LoadProc<FnDWriteCreateFactory>(g_gfx.modDw, "DWriteCreateFactory");
    if (!createD2d || !createDw) return false;

    if (FAILED(createD2d(D2D1_FACTORY_TYPE_SINGLE_THREADED, kIidID2D1Factory, nullptr,
                         reinterpret_cast<void**>(&g_gfx.d2d))) || !g_gfx.d2d) {
        return false;
    }
    if (FAILED(createDw(DWRITE_FACTORY_TYPE_SHARED, kIidIDWriteFactory,
                        reinterpret_cast<IUnknown**>(&g_gfx.dw))) || !g_gfx.dw) {
        return false;
    }

    D2D1_STROKE_STYLE_PROPERTIES sp = {};
    sp.startCap   = D2D1_CAP_STYLE_ROUND;
    sp.endCap     = D2D1_CAP_STYLE_ROUND;
    sp.dashCap    = D2D1_CAP_STYLE_ROUND;
    sp.lineJoin   = D2D1_LINE_JOIN_ROUND;
    sp.miterLimit = 10.0f;
    sp.dashStyle  = D2D1_DASH_STYLE_SOLID;
    g_gfx.d2d->CreateStrokeStyle(&sp, nullptr, 0, &g_gfx.stroke);

    IDWriteFontCollection* coll = nullptr;
    g_gfx.dw->GetSystemFontCollection(&coll, FALSE);
    const wchar_t* faceText    = FamilyExists(coll, L"Segoe UI Variable Text")    ? L"Segoe UI Variable Text"    : L"Segoe UI";
    const wchar_t* faceDisplay = FamilyExists(coll, L"Segoe UI Variable Display") ? L"Segoe UI Variable Display" : L"Segoe UI";
    Rel(coll);

    struct FmtSpec {
        float   size;
        uint8_t displayFace;
        uint8_t bold;
        uint8_t wrap;
        uint8_t trim;
        uint8_t center;
    };
    static constexpr FmtSpec kFmts[F_COUNT] = {
        { 22.0f, 1, 1, 0, 0, 0 }, // F_HERO
        { 13.0f, 0, 1, 0, 0, 0 }, // F_SECTION
        { 13.5f, 0, 1, 0, 1, 0 }, // F_ROWTITLE
        { 12.0f, 0, 0, 0, 1, 0 }, // F_BODY
        { 12.0f, 0, 0, 1, 0, 0 }, // F_STATUS
        { 13.0f, 0, 1, 0, 0, 1 }, // F_BUTTON
        { 12.0f, 0, 1, 0, 1, 0 }, // F_BADGE
        { 15.0f, 0, 1, 0, 0, 1 }, // F_NUM
        { 24.0f, 1, 1, 0, 0, 1 }, // F_IDNUM
    };
    for (int i = 0; i < F_COUNT; ++i) {
        const FmtSpec& s = kFmts[i];
        MakeFmt(static_cast<Fmt>(i),
                s.displayFace ? faceDisplay : faceText,
                s.size,
                s.bold ? DWRITE_FONT_WEIGHT_SEMI_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
                s.wrap != 0,
                s.trim != 0,
                s.center ? DWRITE_TEXT_ALIGNMENT_CENTER : DWRITE_TEXT_ALIGNMENT_LEADING);
        if (!g_gfx.fmt[i]) return false;
    }

    g_ui.twDisplay  = TextWidth(L"Display",  F_BUTTON, 150.0f);
    g_ui.twGpu      = TextWidth(L"GPU",      F_BUTTON, 150.0f);
    g_ui.twRefresh  = TextWidth(L"Refresh",  F_BUTTON, 90.0f);
    g_ui.twIdentify = TextWidth(L"Identify", F_BODY,   80.0f);
    for (size_t i = 0; i < 11; ++i) {
        g_ui.badgeWidths[i] = TextWidth(kBadgeLabels[i], F_BADGE, 190.0f);
    }
    return true;
}

static bool EnsureTarget(HWND hwnd, UINT32 width, UINT32 height) {
    if (g_gfx.rt) return true;
    if (!g_gfx.d2d) return false;

    D2D1_RENDER_TARGET_PROPERTIES props = {};
    props.type                  = D2D1_RENDER_TARGET_TYPE_DEFAULT;
    props.pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;
    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
    props.dpiX                  = static_cast<float>(g_ui.dpi);
    props.dpiY                  = static_cast<float>(g_ui.dpi);
    props.usage                 = D2D1_RENDER_TARGET_USAGE_NONE;
    props.minLevel              = D2D1_FEATURE_LEVEL_DEFAULT;

    D2D1_HWND_RENDER_TARGET_PROPERTIES hprops = {};
    hprops.hwnd           = hwnd;
    hprops.pixelSize      = { width, height };
    hprops.presentOptions = D2D1_PRESENT_OPTIONS_NONE;

    if (FAILED(g_gfx.d2d->CreateHwndRenderTarget(&props, &hprops, &g_gfx.rt)) || !g_gfx.rt) {
        return false;
    }
    g_gfx.rt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
    g_gfx.rt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);

    const Col zero = {};
    g_gfx.rt->CreateSolidColorBrush(&zero, nullptr, &g_gfx.brush);
    g_gfx.lastBrush = 0u;

    const ThemeColors& T = *g_ui.theme;
    D2D1_GRADIENT_STOP bs[3] = {
        { 0.0f,           ToCol(T.borderTop) },
        { T.borderMidPos, ToCol(T.borderMid) },
        { 1.0f,           ToCol(T.borderBot) },
    };
    ID2D1GradientStopCollection* bc = nullptr;
    if (SUCCEEDED(g_gfx.rt->CreateGradientStopCollection(bs, 3, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &bc)) && bc) {
        D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES lp = { { 0, 0 }, { 0, 1 } };
        g_gfx.rt->CreateLinearGradientBrush(&lp, nullptr, bc, &g_gfx.border);
        bc->Release();
    }

    D2D1_GRADIENT_STOP gs[3] = {
        { 0.0f, ToCol(T.glow0) },
        { 0.5f, ToCol(T.glow1) },
        { 1.0f, ToCol(T.glow1 & 0x00FFFFFFu) },
    };
    ID2D1GradientStopCollection* gc = nullptr;
    if (SUCCEEDED(g_gfx.rt->CreateGradientStopCollection(gs, 3, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &gc)) && gc) {
        D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES rp = { { 410, 20 }, { 0, 0 }, 280, 190 };
        g_gfx.rt->CreateRadialGradientBrush(&rp, nullptr, gc, &g_gfx.glow);
        gc->Release();
    }
    if (!g_gfx.brush || !g_gfx.border || !g_gfx.glow) {
        ReleaseTarget();
        return false;
    }
    return true;
}

// ----------------------------------------------------------------------------
// Theme & DWM
// ----------------------------------------------------------------------------
static bool IsWindowsDarkMode() {
    static FnRegGetValueW s_fn   = nullptr;
    static bool           s_init = false;
    if (!s_init) {
        s_init = true;
        HMODULE hMod = GetModuleHandleW(L"kernelbase.dll");
        if (!hMod) hMod = LoadLibraryExW(L"advapi32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (hMod) s_fn = LoadProc<FnRegGetValueW>(hMod, "RegGetValueW");
    }
    if (!s_fn) return false;
    DWORD val  = 1;
    DWORD size = sizeof(val);
    LSTATUS rc = s_fn(HKEY_CURRENT_USER,
                      L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                      L"AppsUseLightTheme", RRF_RT_REG_DWORD, nullptr, &val, &size);
    return rc == ERROR_SUCCESS && val == 0;
}

static COLORREF ToColorRef(Pk c) {
    return RGB((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
}

static FnDwmSetWindowAttribute g_dwmSet;

static FnDwmSetWindowAttribute GetDwmSet() {
    static bool s_init = false;
    if (!s_init) {
        s_init = true;
        g_gfx.modDwm = LoadLibraryExW(L"dwmapi.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (g_gfx.modDwm) g_dwmSet = LoadProc<FnDwmSetWindowAttribute>(g_gfx.modDwm, "DwmSetWindowAttribute");
    }
    return g_dwmSet;
}

static void ApplyDwmTheme(HWND hwnd) {
    FnDwmSetWindowAttribute dwmSet = GetDwmSet();
    if (!dwmSet) return;

    constexpr DWORD kUseImmersiveDark = 20;
    constexpr DWORD kCaptionColor     = 35;
    constexpr DWORD kTextColor        = 36;

    BOOL     dark    = (g_ui.theme == &kDarkTheme) ? TRUE : FALSE;
    COLORREF caption = ToColorRef(g_ui.theme->canvas);
    COLORREF text    = ToColorRef(g_ui.theme->text);

    dwmSet(hwnd, kUseImmersiveDark, &dark,    sizeof(dark));
    dwmSet(hwnd, kCaptionColor,     &caption, sizeof(caption));
    dwmSet(hwnd, kTextColor,        &text,    sizeof(text));
}

static bool RefreshTheme(HWND hwnd) {
    const ThemeColors* next = IsWindowsDarkMode() ? &kDarkTheme : &kLightTheme;
    if (next == g_ui.theme) return false;
    g_ui.theme = next;
    ReleaseTarget();
    ApplyDwmTheme(hwnd);
    return true;
}

// ----------------------------------------------------------------------------
// Layout & State Helpers
// ----------------------------------------------------------------------------
static const NvDisplayInfo* GetSelectedDisplay() {
    if (g_ui.displayCount == 0 || g_ui.selectedDispIdx >= g_ui.displayCount) {
        return nullptr;
    }
    return &g_ui.displays[g_ui.selectedDispIdx];
}

static bool CanApplyCurrentDisplay() {
    const NvDisplayInfo* d = GetSelectedDisplay();
    return (d != nullptr && d->foundInConfig);
}

static bool CanCommitSelection() {
    const NvDisplayInfo* d = GetSelectedDisplay();
    return !g_ui.busy && d && d->foundInConfig &&
           g_ui.hasSelectedMode && (d->scaling != g_ui.selectedMode);
}

static size_t DropVisibleCount() {
    return (g_ui.displayCount < kMaxDropVis) ? g_ui.displayCount : kMaxDropVis;
}

static D2D1_RECT_F GetDropdownRect() {
    const size_t vis = DropVisibleCount();
    return { kDropLeft, kDropTop, kDropRight, kDropTop + static_cast<float>(vis) * kDropRowH };
}

static bool IsTargetInteractive(HitTarget t) {
    if (g_ui.busy) return false;
    const bool canApply = CanApplyCurrentDisplay();
    switch (t) {
        case HitTarget::DisplayCard:    return g_ui.displayCount > 1;
        case HitTarget::IdentifyBtn:    return !g_ui.dropdownOpen && g_ui.displayCount > 1;
        case HitTarget::SegmentDisplay: return canApply && (g_ui.activeGroup != ScalerGroup::Display);
        case HitTarget::SegmentGpu:     return canApply && (g_ui.activeGroup != ScalerGroup::Gpu);
        case HitTarget::Row0:
        case HitTarget::Row1:
        case HitTarget::Row2:           return canApply;
        case HitTarget::Row3:           return canApply && (g_ui.activeGroup == ScalerGroup::Gpu);
        case HitTarget::RefreshBtn:     return true;
        case HitTarget::ApplyBtn:       return CanCommitSelection();
        default:
            if (t >= HitTarget::DropRow0 && t <= HitTarget::DropRow5) {
                const size_t row = static_cast<size_t>(t) - static_cast<size_t>(HitTarget::DropRow0);
                return g_ui.dropdownOpen && row < DropVisibleCount();
            }
            return false;
    }
}

struct HitRect {
    int16_t   l, t, r, b;
    HitTarget target;
};

static constexpr HitRect MakeHit(const D2D1_RECT_F& r, HitTarget target) {
    return { static_cast<int16_t>(r.left), static_cast<int16_t>(r.top),
             static_cast<int16_t>(r.right), static_cast<int16_t>(r.bottom), target };
}

static HitTarget HitTest(POINT pt) {
    const float x = static_cast<float>(pt.x) * g_ui.invScale;
    const float y = static_cast<float>(pt.y) * g_ui.invScale;

    if (g_ui.dropdownOpen) {
        if (x >= kDisplayCard.left && x < kDisplayCard.right &&
            y >= kDisplayCard.top  && y < kDisplayCard.bottom &&
            IsTargetInteractive(HitTarget::DisplayCard)) {
            return HitTarget::DisplayCard;
        }
        const size_t vis = DropVisibleCount();
        for (size_t v = 0; v < vis; ++v) {
            const float top = kDropTop + static_cast<float>(v) * kDropRowH;
            if (x >= kDropLeft && x < kDropRight && y >= top && y < top + kDropRowH) {
                return static_cast<HitTarget>(static_cast<uint8_t>(HitTarget::DropRow0) + v);
            }
        }
        return HitTarget::None;
    }

    static constexpr HitRect kHitRegions[] = {
        MakeHit(kDisplayCard, HitTarget::DisplayCard),
        MakeHit(kIdentifyBtn, HitTarget::IdentifyBtn),
        MakeHit(kTabDisplay,  HitTarget::SegmentDisplay),
        MakeHit(kTabGpu,      HitTarget::SegmentGpu),
        MakeHit(kRows[0],     HitTarget::Row0),
        MakeHit(kRows[1],     HitTarget::Row1),
        MakeHit(kRows[2],     HitTarget::Row2),
        MakeHit(kRows[3],     HitTarget::Row3),
        MakeHit(kRefreshBtn,  HitTarget::RefreshBtn),
        MakeHit(kApplyBtn,    HitTarget::ApplyBtn),
    };
    for (const HitRect& h : kHitRegions) {
        if (x >= h.l && x < h.r && y >= h.t && y < h.b && IsTargetInteractive(h.target)) {
            return h.target;
        }
    }
    return HitTarget::None;
}

static const ModeItem* GetModes(ScalerGroup group, size_t& outCount) {
    const bool display = (group == ScalerGroup::Display);
    outCount = display ? kDisplayModeCount : kGpuModeCount;
    return display ? kDisplayModes : kGpuModes;
}

// Index of `mode` inside `group`'s list, or the list size when it is not part of that group.
static size_t FindMode(ScalerGroup group, NvScaling mode, size_t& outCount) {
    const ModeItem* items = GetModes(group, outCount);
    size_t i = 0;
    while (i < outCount && items[i].value != mode) ++i;
    return i;
}

static void SyncSelectionFromDisplay() {
    const NvDisplayInfo* disp = GetSelectedDisplay();
    g_ui.hasSelectedMode = false;
    if (!disp || !disp->foundInConfig) return;

    for (int gi = 0; gi < 2; ++gi) {
        const ScalerGroup g = static_cast<ScalerGroup>(gi);
        size_t count = 0;
        if (FindMode(g, disp->scaling, count) < count) {
            g_ui.activeGroup     = g;
            g_ui.selectedMode    = disp->scaling;
            g_ui.hasSelectedMode = true;
            return;
        }
    }
}

static void SetStatus(StatusKind kind, const wchar_t* text) {
    g_ui.statusKind = kind;
    StrUtil::SafeCopy(g_ui.statusText, 256, text);
}

static void RefreshDisplayCache();

static void ReloadDisplays(uint32_t preferredDisplayId, HWND hostHwnd = nullptr) {
    wchar_t initErr[256] = {};
    if (!NvApi::Initialize(initErr, 256)) {
        g_ui.displayCount    = 0;
        g_ui.selectedDispIdx = 0;
        g_ui.hasSelectedMode = false;
        SetStatus(StatusKind::Error, initErr);
        RefreshDisplayCache();
        return;
    }

    g_ui.displayCount    = NvApi::GetDisplays(g_ui.displays, 16);
    g_ui.selectedDispIdx = 0;
    if (preferredDisplayId != 0) {
        for (size_t i = 0; i < g_ui.displayCount; ++i) {
            if (g_ui.displays[i].displayId == preferredDisplayId) {
                g_ui.selectedDispIdx = i;
                break;
            }
        }
    } else if (hostHwnd && g_ui.displayCount > 1) {
        HMONITOR hMon = MonitorFromWindow(hostHwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFOEXW mi = {};
        mi.cbSize = sizeof(mi);
        if (hMon && GetMonitorInfoW(hMon, &mi)) {
            for (size_t i = 0; i < g_ui.displayCount; ++i) {
                if (g_ui.displays[i].foundInConfig &&
                    StrUtil::AsciiEqualsI(g_ui.displays[i].displayName, mi.szDevice)) {
                    g_ui.selectedDispIdx = i;
                    break;
                }
            }
        }
    }

    SyncSelectionFromDisplay();
    if (g_ui.displayCount == 0) {
        SetStatus(StatusKind::Error, L"No NVIDIA displays were found.");
    } else if (g_ui.statusKind == StatusKind::Error) {
        SetStatus(StatusKind::Neutral, L"Ready. Select a scaling mode and click Apply.");
    }
    RefreshDisplayCache();
}

// Display <-> GPU keeps the same row position (GPU lists Integer Scaling first, so
// Display row i maps to GPU row i+1 and GPU Integer Scaling falls back to Display No Scaling).
static bool SwitchScalerGroup(ScalerGroup target) {
    if (g_ui.activeGroup == target) return false;

    if (g_ui.hasSelectedMode) {
        size_t fromCount = 0;
        const size_t i = FindMode(g_ui.activeGroup, g_ui.selectedMode, fromCount);
        if (i < fromCount) {
            size_t j = (target == ScalerGroup::Gpu) ? i + 1 : (i > 0 ? i - 1 : 0);
            size_t toCount = 0;
            const ModeItem* toItems = GetModes(target, toCount);
            if (j >= toCount) j = toCount - 1;
            g_ui.selectedMode = toItems[j].value;
        }
    }
    g_ui.activeGroup = target;
    return true;
}

static bool IsKnownActiveBadge(const NvDisplayInfo* disp) {
    if (!disp || !disp->foundInConfig) return false;
    return disp->scaling != NvScaling::Default && disp->scaling != NvScaling::Customized;
}

static uint32_t HzOf(const NvDisplayInfo& d) {
    return (d.refreshRateMillihertz + 500u) / 1000u;
}

static void FormatDisplayMeta(size_t idx, wchar_t* buf, size_t bufCount) {
    const NvDisplayInfo& d = g_ui.displays[idx];
    const wchar_t* shortName = NvApi::ShortDisplayName(d.displayName);
    const wchar_t* hint      = (g_ui.displayCount > 1)
        ? NvApi::GetSpatialHint(g_ui.displays, g_ui.displayCount, idx)
        : nullptr;

    wchar_t prefix[48] = {};
    if (hint) {
        StrUtil::SafeFormat(prefix, 48, L"%s \x00B7 %s", hint, shortName);
    } else {
        StrUtil::SafeCopy(prefix, 48, shortName);
    }

    if (!d.foundInConfig) {
        StrUtil::SafeFormat(buf, bufCount, L"%s \x00B7 Not active in NVIDIA config", prefix);
    } else if (d.refreshRateMillihertz > 0) {
        StrUtil::SafeFormat(buf, bufCount, L"%s \x00B7 %u \x00D7 %u \x00B7 %u Hz",
                            prefix, d.width, d.height, HzOf(d));
    } else {
        StrUtil::SafeFormat(buf, bufCount, L"%s \x00B7 %u \x00D7 %u",
                            prefix, d.width, d.height);
    }
}

// Rebuilds the cached strings for every display. Call whenever the display list,
// selected display, or its scaling changes; painting then only reads the cache.
static void RefreshDisplayCache() {
    if (g_ui.displayCount > 1) {
        const uint32_t num = static_cast<uint32_t>(g_ui.selectedDispIdx + 1);
        StrUtil::SafeFormat(g_ui.secDisplayLabel, 48, L"Target display (%u of %u)",
                            num, static_cast<uint32_t>(g_ui.displayCount));
        StrUtil::SafeFormat(g_ui.applyLabel, 32, L"Apply to #%u", num);
    } else {
        StrUtil::SafeCopy(g_ui.secDisplayLabel, 48, L"Display");
        StrUtil::SafeCopy(g_ui.applyLabel, 32, L"Apply");
    }

    for (size_t i = 0; i < g_ui.displayCount; ++i) {
        FormatDisplayMeta(i, g_ui.dispMetas[i], 96);
    }
}

// ----------------------------------------------------------------------------
// Direct2D drawing primitives (DIP coordinates, hairlines snapped to device pixels)
// ----------------------------------------------------------------------------
static float Snap(float v) {
    return static_cast<float>(static_cast<int>(v * g_ui.scale + 0.5f)) * g_ui.invScale;
}

static Pk WithAlpha(Pk c, float a) {
    return (static_cast<uint32_t>(static_cast<float>(c >> 24) * a + 0.5f) << 24) | (c & 0xFFFFFFu);
}

__attribute__((noinline))
static void Use(Pk c) {
    if (c == g_gfx.lastBrush) return;
    g_gfx.lastBrush = c;
    const Col k = ToCol(c);
    g_gfx.brush->SetColor(&k);
}

__attribute__((noinline))
static void Solid(float l, float t, float r, float b, float rad, Pk c) {
    D2D1_ROUNDED_RECT rr = { { l, t, r, b }, rad, rad };
    Use(c);
    g_gfx.rt->FillRoundedRectangle(&rr, g_gfx.brush);
}

__attribute__((noinline))
static void Box(float l, float t, float r, float b, float rad, Pk c, float w) {
    D2D1_ROUNDED_RECT rr = { { l, t, r, b }, rad, rad };
    Use(c);
    g_gfx.rt->DrawRoundedRectangle(&rr, g_gfx.brush, w, nullptr);
}

__attribute__((noinline))
static void FillRR(float l, float t, float r, float b, float rad, Pk c) {
    Solid(Snap(l), Snap(t), Snap(r), Snap(b), rad, c);
}

static inline void FillRR(const D2D1_RECT_F& r, float rad, Pk c) {
    FillRR(r.left, r.top, r.right, r.bottom, rad, c);
}

static D2D1_ROUNDED_RECT InsetBorder(float l, float t, float r, float b, float rad, float* outW) {
    const float w = g_ui.invScale;
    const float h = w * 0.5f;
    *outW = w;
    return { { Snap(l) + h, Snap(t) + h, Snap(r) - h, Snap(b) - h }, rad - h, rad - h };
}

__attribute__((noinline))
static void StrokeRR(float l, float t, float r, float b, float rad, Pk c) {
    float w = 1.0f;
    D2D1_ROUNDED_RECT rr = InsetBorder(l, t, r, b, rad, &w);
    Box(rr.rect.left, rr.rect.top, rr.rect.right, rr.rect.bottom, rr.radiusX, c, w);
}

static inline void StrokeRR(const D2D1_RECT_F& r, float rad, Pk c) {
    StrokeRR(r.left, r.top, r.right, r.bottom, rad, c);
}

__attribute__((noinline))
static void Card(const D2D1_RECT_F& rect, float rad, Pk fill) {
    FillRR(rect.left, rect.top, rect.right, rect.bottom, rad, fill);
    float w = 1.0f;
    D2D1_ROUNDED_RECT rr = InsetBorder(rect.left, rect.top, rect.right, rect.bottom, rad, &w);
    g_gfx.border->SetStartPoint(D2D1_POINT_2F{ rr.rect.left, rr.rect.top });
    g_gfx.border->SetEndPoint(D2D1_POINT_2F{ rr.rect.left, rr.rect.bottom });
    g_gfx.rt->DrawRoundedRectangle(&rr, g_gfx.border, w, nullptr);
}

__attribute__((noinline))
static void Line(float x1, float y1, float x2, float y2, Pk c, float w) {
    Use(c);
    g_gfx.rt->DrawLine(D2D1_POINT_2F{ x1, y1 }, D2D1_POINT_2F{ x2, y2 }, g_gfx.brush, w, g_gfx.stroke);
}

__attribute__((noinline))
static void Disc(float cx, float cy, float r, Pk c) {
    D2D1_ELLIPSE e = { { cx, cy }, r, r };
    Use(c);
    g_gfx.rt->FillEllipse(&e, g_gfx.brush);
}

__attribute__((noinline))
static void Ring(float cx, float cy, float r, Pk c, float w) {
    D2D1_ELLIPSE e = { { cx, cy }, r, r };
    Use(c);
    g_gfx.rt->DrawEllipse(&e, g_gfx.brush, w, nullptr);
}

__attribute__((noinline))
static void Text(const wchar_t* s, Fmt f, float l, float t, float r, float b, Pk c) {
    if (!s[0]) return;
    Use(c);
    const D2D1_RECT_F rc = { l, t, r, b };
    g_gfx.rt->DrawTextW(s, static_cast<UINT32>(StrUtil::Len(s)), g_gfx.fmt[f], &rc, g_gfx.brush,
                        D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
}

static inline void Text(const wchar_t* s, Fmt f, const D2D1_RECT_F& r, Pk c) {
    Text(s, f, r.left, r.top, r.right, r.bottom, c);
}

__attribute__((noinline))
static float TextWidth(const wchar_t* s, Fmt f, float maxW) {
    float w = 0.0f;
    IDWriteTextLayout* l = nullptr;
    if (SUCCEEDED(g_gfx.dw->CreateTextLayout(s, static_cast<UINT32>(StrUtil::Len(s)), g_gfx.fmt[f],
                                             maxW, 40.0f, &l)) && l) {
        DWRITE_TEXT_METRICS m = {};
        if (SUCCEEDED(l->GetMetrics(&m))) w = m.width;
        l->Release();
    }
    return w;
}

// ----------------------------------------------------------------------------
// Vector icons
// ----------------------------------------------------------------------------
static void DrawMonitorGlyph(float cx, float cy, float s, Pk c) {
    const float w = 1.5f;
    Box(cx - 9.0f * s, cy - 7.0f * s, cx + 9.0f * s, cy + 4.0f * s, 2.0f * s, c, w);
    Line(cx, cy + 4.0f * s, cx, cy + 7.0f * s, c, w);
    Line(cx - 4.0f * s, cy + 7.5f * s, cx + 4.0f * s, cy + 7.5f * s, c, w);
}

static void DrawChipGlyph(float cx, float cy, Pk c) {
    const float w = 1.4f;
    Box(cx - 4.5f, cy - 4.5f, cx + 4.5f, cy + 4.5f, 1.5f, c, w);
    Box(cx - 1.6f, cy - 1.6f, cx + 1.6f, cy + 1.6f, 0.5f, c, 1.2f);
    for (int i = -1; i <= 1; i += 2) {
        const float k = static_cast<float>(i) * 2.3f;
        Line(cx + k, cy - 4.5f, cx + k, cy - 7.0f, c, w);
        Line(cx + k, cy + 4.5f, cx + k, cy + 7.0f, c, w);
        Line(cx - 4.5f, cy + k, cx - 7.0f, cy + k, c, w);
        Line(cx + 4.5f, cy + k, cx + 7.0f, cy + k, c, w);
    }
}

static void DrawRefreshGlyph(float cx, float cy, Pk c) {
    static constexpr int8_t kArc[10][2] = {
        { 87, 50 }, { 50, 87 }, { 0, 100 }, { -50, 87 }, { -87, 50 },
        { -100, 0 }, { -87, -50 }, { -50, -87 }, { 0, -100 }, { 50, -87 },
    };
    const float r = 0.052f;
    for (int i = 0; i < 9; ++i) {
        Line(cx + kArc[i][0] * r, cy + kArc[i][1] * r, cx + kArc[i + 1][0] * r, cy + kArc[i + 1][1] * r, c, 1.5f);
    }
    const float tx = cx + kArc[9][0] * r;
    const float ty = cy + kArc[9][1] * r;
    Line(tx, ty, tx - 0.4f * 3.6f, ty - 0.907f * 3.6f, c, 1.5f);
    Line(tx, ty, tx - 0.996f * 3.6f, ty + 0.087f * 3.6f, c, 1.5f);
}

static void DrawChevron(float cx, float cy, bool up, Pk c) {
    const float dy = up ? -2.0f : 2.0f;
    Line(cx - 4.0f, cy - dy, cx, cy + dy, c, 1.5f);
    Line(cx, cy + dy, cx + 4.0f, cy - dy, c, 1.5f);
}

static void DrawIdentifyGlyph(float cx, float cy, Pk c) {
    Box(cx - 5.5f, cy - 4.0f, cx + 5.5f, cy + 4.0f, 1.5f, c, 1.2f);
    Disc(cx, cy, 1.3f, c);
}

static void DrawInfoGlyph(float cx, float cy, Pk c) {
    Ring(cx, cy, 7.0f, c, 1.3f);
    Line(cx, cy - 0.5f, cx, cy + 3.2f, c, 1.4f);
    Disc(cx, cy - 3.0f, 1.0f, c);
}

static void DrawRadio(float cx, float cy, bool selected, bool enabled) {
    const ThemeColors& T = *g_ui.theme;
    if (selected) {
        Disc(cx, cy, 9.0f, enabled ? T.accent : T.accentDisabled);
        Disc(cx, cy, 4.0f, enabled ? T.onAccent : T.onAccentDisabled);
    } else {
        Ring(cx, cy, 8.5f, enabled ? T.radioBorder : T.textDisabled, 1.0f);
    }
}

static void DrawModeDiagram(float cx, float cy, NvScaling mode, Pk c) {
    Box(cx - 11.0f, cy - 7.5f, cx + 11.0f, cy + 7.5f, 2.0f, c, 1.2f);
    switch (mode) {
        case NvScaling::DisplayNoScaling:
        case NvScaling::GpuNoScaling:
            Solid(cx - 3.5f, cy - 2.0f, cx + 3.5f, cy + 2.0f, 0.8f, c);
            break;
        case NvScaling::DisplayAspectRatio:
        case NvScaling::GpuAspectRatio:
            Solid(cx - 5.0f, cy - 4.5f, cx + 5.0f, cy + 4.5f, 1.0f, c);
            break;
        case NvScaling::DisplayFullScreen:
        case NvScaling::GpuFullScreen:
            Solid(cx - 8.0f, cy - 4.5f, cx + 8.0f, cy + 4.5f, 1.0f, c);
            break;
        default: {
            const Pk dim = WithAlpha(c, 0.5f);
            Solid(cx - 4.25f, cy - 3.75f, cx - 0.75f, cy - 0.25f, 0.5f, c);
            Solid(cx + 0.75f, cy - 3.75f, cx + 4.25f, cy - 0.25f, 0.5f, dim);
            Solid(cx - 4.25f, cy + 0.25f, cx - 0.75f, cy + 3.75f, 0.5f, dim);
            Solid(cx + 0.75f, cy + 0.25f, cx + 4.25f, cy + 3.75f, 0.5f, c);
            break;
        }
    }
}

static void DrawStatusIcon(float cx, float cy, StatusKind kind) {
    const ThemeColors& T = *g_ui.theme;
    const Pk fg = T.onAccent;
    switch (kind) {
        case StatusKind::Success:
            Disc(cx, cy, 8.0f, T.ibIconOk);
            Line(cx - 3.3f, cy + 0.2f, cx - 0.8f, cy + 2.7f, fg, 1.6f);
            Line(cx - 0.8f, cy + 2.7f, cx + 3.6f, cy - 2.5f, fg, 1.6f);
            break;
        case StatusKind::Error:
            Disc(cx, cy, 8.0f, T.ibIconErr);
            Line(cx - 2.8f, cy - 2.8f, cx + 2.8f, cy + 2.8f, fg, 1.6f);
            Line(cx + 2.8f, cy - 2.8f, cx - 2.8f, cy + 2.8f, fg, 1.6f);
            break;
        case StatusKind::Busy:
            Ring(cx, cy, 7.2f, T.accent, 1.6f);
            Disc(cx, cy, 2.6f, T.accent);
            break;
        default:
            Disc(cx, cy, 8.0f, T.accent);
            Line(cx, cy - 0.2f, cx, cy + 3.4f, fg, 1.6f);
            Disc(cx, cy - 3.2f, 1.0f, fg);
            break;
    }
}

// ----------------------------------------------------------------------------
// Paint
// ----------------------------------------------------------------------------
static void PaintSegment(const D2D1_RECT_F& rc, const wchar_t* label, float tw, bool isGpu,
                         ScalerGroup group, HitTarget hit, bool canApply) {
    const ThemeColors& T = *g_ui.theme;
    const bool active  = (g_ui.activeGroup == group);
    const bool usable  = canApply && !g_ui.busy;
    const bool hovered = usable && (g_ui.hover == hit || g_ui.pressed == hit);

    Pk fg = usable ? T.textSec : T.textDisabled;
    if (active) {
        const Pk fill = usable ? T.accent : T.accentDisabled;
        fg = usable ? T.onAccent : T.onAccentDisabled;
        FillRR(rc, 16.0f, fill);
        Line(rc.left + 14.0f, rc.top + 0.5f, rc.right - 14.0f, rc.top + 0.5f,
             WithAlpha(Rgb(0xFFFFFF), 0.18f), g_ui.invScale);
    } else if (hovered) {
        FillRR(rc, 16.0f, T.cardHover);
    }

    const float gw  = 14.0f + 8.0f + tw;
    const float x0  = (rc.left + rc.right - gw) * 0.5f;
    const float cy  = (rc.top + rc.bottom) * 0.5f;
    if (isGpu) DrawChipGlyph(x0 + 7.0f, cy, fg);
    else       DrawMonitorGlyph(x0 + 7.0f, cy - 0.5f, 0.72f, fg);
    Text(label, F_BUTTON, x0 + 22.0f, rc.top, x0 + 22.0f + tw + 4.0f, rc.bottom, fg);
}

__attribute__((noinline))
static void PaintDropdown(float W) {
    const ThemeColors& T = *g_ui.theme;
    const bool dark = (g_ui.theme == &kDarkTheme);
    const D2D1_RECT_F dropRc = GetDropdownRect();

    Use(WithAlpha(Rgb(0x000000), dark ? 0.48f : 0.18f));
    const D2D1_RECT_F scrimRc = { 0.0f, 163.0f, W, static_cast<float>(kBaseHeight) };
    g_gfx.rt->FillRectangle(&scrimRc, g_gfx.brush);

    const Pk surface = dark ? Rgb(0x2B2B2B) : Rgb(0xFCFCFC);
    const Pk border  = dark ? Rgb(0xFFFFFF, 0.16f) : Rgb(0x000000, 0.14f);
    FillRR(dropRc, 8.0f, surface);
    StrokeRR(dropRc, 8.0f, border);

    const size_t vis       = DropVisibleCount();
    const bool   hasScroll = (g_ui.displayCount > kMaxDropVis);
    const float  rowRight  = dropRc.right - (hasScroll ? 12.0f : 4.0f);
    const float  rightPad  = hasScroll ? 20.0f : 12.0f;

    for (size_t v = 0; v < vis; ++v) {
        const size_t idx = g_ui.dropScroll + v;
        const NvDisplayInfo& d = g_ui.displays[idx];
        const float top  = kDropTop + static_cast<float>(v) * kDropRowH;
        const float bot  = top + kDropRowH;
        const float midY = (top + bot) * 0.5f;

        const HitTarget rowHit = static_cast<HitTarget>(static_cast<uint8_t>(HitTarget::DropRow0) + v);
        const bool selected    = (idx == g_ui.selectedDispIdx);
        const bool pressed     = (g_ui.pressed == rowHit);
        const bool highlighted = (idx == g_ui.dropHighlight) || (g_ui.hover == rowHit);

        if (selected || highlighted || pressed) {
            const Pk fill = pressed ? T.cardPress : (selected ? T.rowSel : T.rowHover);
            FillRR(dropRc.left + 4.0f, top + 3.0f, rowRight, bot - 3.0f, 6.0f, fill);
        }
        if (selected) {
            FillRR(dropRc.left + 4.0f, midY - 9.0f, dropRc.left + 7.0f, midY + 9.0f, 1.5f, T.accent);
        }

        wchar_t numBuf[4] = {};
        StrUtil::SafeFormat(numBuf, 4, L"%u", static_cast<uint32_t>(idx + 1));
        const float numL = dropRc.left + 15.0f;
        const float numT = midY - 13.0f;
        const float numR = dropRc.left + 41.0f;
        const float numB = midY + 13.0f;
        FillRR(numL, numT, numR, numB, 6.0f, selected ? T.accent : T.neutralBg);
        if (!selected) StrokeRR(numL, numT, numR, numB, 6.0f, T.neutralBorder);
        Text(numBuf, F_NUM, numL, numT, numR, numB, selected ? T.onAccent : T.text);

        const size_t bIdx        = GetBadgeIndex(&d);
        const wchar_t* modeLabel = kBadgeLabels[bIdx];
        const bool activeBadge   = IsKnownActiveBadge(&d);
        float mw = g_ui.badgeWidths[bIdx];
        if (mw > 136.0f) mw = 136.0f;
        const float pillR = dropRc.right - rightPad;
        const float pillL = pillR - mw - 14.0f;
        const float pillT = top + 7.0f;
        const float pillB = top + 25.0f;
        FillRR(pillL, pillT, pillR, pillB, 9.0f, activeBadge ? T.badgeBg : T.neutralBg);
        StrokeRR(pillL, pillT, pillR, pillB, 9.0f, activeBadge ? T.badgeBorder : T.neutralBorder);
        Text(modeLabel, F_BADGE, pillL + 7.0f, pillT, pillR - 5.0f, pillB,
             activeBadge ? T.badgeText : T.textMuted);

        const wchar_t* title = d.monitorName[0] ? d.monitorName : NvApi::ShortDisplayName(d.displayName);
        Text(title, F_ROWTITLE, dropRc.left + 51.0f, top + 6.0f, pillL - 8.0f, top + 26.0f, T.text);
        Text(g_ui.dispMetas[idx], F_BODY, dropRc.left + 51.0f, top + 25.0f, dropRc.right - rightPad, top + 44.0f, T.textSec);

        if (v + 1 < vis) {
            const float px = g_ui.invScale;
            const float dy = Snap(bot) - px * 0.5f;
            Line(dropRc.left + 51.0f, dy, dropRc.right - rightPad, dy, T.divider, px);
        }
    }

    if (hasScroll) {
        const float trackT = dropRc.top + 8.0f;
        const float trackH = (dropRc.bottom - dropRc.top) - 16.0f;
        const float thumbH = trackH * (static_cast<float>(vis) / static_cast<float>(g_ui.displayCount));
        const float maxSc  = static_cast<float>(g_ui.displayCount - vis);
        const float thumbT = trackT + (trackH - thumbH) * (static_cast<float>(g_ui.dropScroll) / maxSc);
        FillRR(dropRc.right - 7.0f, thumbT, dropRc.right - 4.0f, thumbT + thumbH, 1.5f, T.textMuted);
    }
}

__attribute__((noinline))
static void PaintAll(float W) {
    const ThemeColors&   T           = *g_ui.theme;
    const NvDisplayInfo* disp        = GetSelectedDisplay();
    const bool           canApply    = CanApplyCurrentDisplay();
    const bool           usable      = canApply && !g_ui.busy;
    const bool           activeBadge = IsKnownActiveBadge(disp);

    // Header glow (extends to 220 DIP to cover full 20+190 radial falloff without clipping)
    D2D1_RECT_F glowRc = { 0, 0, W, 220 };
    g_gfx.rt->FillRectangle(&glowRc, g_gfx.glow);

    // 1. Header
    Text(L"NVIDIA driver scaling", F_BODY, kEyebrow, T.textSec);
    Text(L"Display Scaling", F_HERO, kTitle, T.text);

    const size_t bIdx = GetBadgeIndex(disp);
    const float dotSpace = activeBadge ? 14.0f : 0.0f;
    float badgeW = g_ui.badgeWidths[bIdx] + 24.0f + dotSpace;
    if (badgeW > 190.0f) badgeW = 190.0f;
    const float badgeL = kDisplayCard.right - badgeW;
    FillRR(badgeL, 35.0f, kDisplayCard.right, 63.0f, 14.0f, activeBadge ? T.badgeBg : T.neutralBg);
    StrokeRR(badgeL, 35.0f, kDisplayCard.right, 63.0f, 14.0f, activeBadge ? T.badgeBorder : T.neutralBorder);
    if (activeBadge) Disc(badgeL + 15.0f, 49.0f, 3.25f, T.badgeDot);
    Text(kBadgeLabels[bIdx], F_BADGE, badgeL + 12.0f + dotSpace, 35.0f, kDisplayCard.right - 10.0f, 63.0f,
         activeBadge ? T.badgeText : T.textSec);

    // 2. Display card + Identify trigger (multi-monitor only)
    const bool multi = (g_ui.displayCount > 1);
    Text(g_ui.secDisplayLabel[0] ? g_ui.secDisplayLabel : L"Display", F_SECTION, kSecDisplay, T.textSec);
    if (multi) {
        const bool idUsable  = !g_ui.busy && !g_ui.dropdownOpen;
        const bool idPressed = idUsable && (g_ui.pressed == HitTarget::IdentifyBtn);
        const bool idHovered = idUsable && (g_ui.hover == HitTarget::IdentifyBtn);
        const Pk idFill = idPressed ? T.cardPress : (idHovered ? T.cardHover : T.card);
        const Pk idFg   = idUsable ? T.textSec : T.textDisabled;
        FillRR(kIdentifyBtn, 5.0f, idFill);
        StrokeRR(kIdentifyBtn, 5.0f, T.railBorder);
        const float gw = 12.0f + 5.0f + g_ui.twIdentify;
        const float x0 = (kIdentifyBtn.left + kIdentifyBtn.right - gw) * 0.5f;
        const float cy = (kIdentifyBtn.top + kIdentifyBtn.bottom) * 0.5f;
        DrawIdentifyGlyph(x0 + 6.0f, cy, idFg);
        Text(L"Identify", F_BODY, x0 + 17.0f, kIdentifyBtn.top, x0 + 20.0f + g_ui.twIdentify, kIdentifyBtn.bottom, idFg);
    }

    Pk dispFill = T.card;
    if (multi && !g_ui.busy) {
        if (g_ui.pressed == HitTarget::DisplayCard)    dispFill = T.cardPress;
        else if (g_ui.hover == HitTarget::DisplayCard) dispFill = T.cardHover;
    }
    Card(kDisplayCard, 8.0f, dispFill);
    if (g_ui.dropdownOpen) {
        StrokeRR(kDisplayCard, 8.0f, T.accent);
    }

    const D2D1_RECT_F tileRc = { 32, 114, 68, 150 };
    const Pk tileFg = disp ? T.accent : T.textDisabled;
    FillRR(tileRc, 8.0f, WithAlpha(tileFg, 0.14f));
    if (disp && multi) {
        StrokeRR(tileRc, 8.0f, WithAlpha(T.accent, 0.36f));
        wchar_t numBuf[4] = {};
        StrUtil::SafeFormat(numBuf, 4, L"%u", static_cast<uint32_t>(g_ui.selectedDispIdx + 1));
        Text(numBuf, F_NUM, tileRc, T.accent);
    } else {
        DrawMonitorGlyph(50.0f, 131.0f, 1.0f, tileFg);
    }

    const float dispRight = kDisplayCard.right - (multi ? 40.0f : 16.0f);
    if (disp) {
        const wchar_t* monTitle = disp->monitorName[0] ? disp->monitorName : NvApi::ShortDisplayName(disp->displayName);
        Text(monTitle, F_ROWTITLE, 80, 112, dispRight, 134, T.text);
        Text(g_ui.dispMetas[g_ui.selectedDispIdx], F_BODY, 80, 134, dispRight, 152, T.textSec);
    } else {
        Text(L"No NVIDIA display detected", F_ROWTITLE, 80, 112, dispRight, 134, T.textSec);
        Text(L"Check drivers or display connection", F_BODY, 80, 134, dispRight, 152, T.textMuted);
    }
    if (multi) {
        DrawChevron(kDisplayCard.right - 22.0f, (kDisplayCard.top + kDisplayCard.bottom) * 0.5f,
                    g_ui.dropdownOpen, T.textSec);
    }

    // 3. Segmented switch
    Text(L"Perform scaling on", F_SECTION, kSecScaler, T.textSec);
    FillRR(kRail, 20.0f, T.railFill);
    StrokeRR(kRail, 20.0f, T.railBorder);
    PaintSegment(kTabDisplay, L"Display", g_ui.twDisplay, false, ScalerGroup::Display, HitTarget::SegmentDisplay, canApply);
    PaintSegment(kTabGpu,     L"GPU",     g_ui.twGpu,     true,  ScalerGroup::Gpu,     HitTarget::SegmentGpu,     canApply);

    // 4. Scaling mode card
    Text(L"Scaling mode", F_SECTION, kSecMode, T.textSec);
    Card(kModeCard, 8.0f, T.card);

    size_t itemCount = 0;
    const ModeItem* items = GetModes(g_ui.activeGroup, itemCount);
    const HitTarget rowHits[4] = { HitTarget::Row0, HitTarget::Row1, HitTarget::Row2, HitTarget::Row3 };

    for (size_t i = 0; i < 4; ++i) {
        const D2D1_RECT_F& r = kRows[i];
        const float midY = (r.top + r.bottom) * 0.5f;

        if (i < itemCount) {
            const ModeItem& item = items[i];
            const bool selected = g_ui.hasSelectedMode && (g_ui.selectedMode == item.value);
            const bool hovered  = usable && (g_ui.hover == rowHits[i]);
            const bool pressed  = usable && (g_ui.pressed == rowHits[i]);

            if (selected || hovered || pressed) {
                const Pk fill = pressed ? T.cardPress : (selected ? T.rowSel : T.rowHover);
                FillRR(r.left + 4.0f, r.top + 3.0f, r.right - 4.0f, r.bottom - 3.0f, 6.0f, fill);
            }
            if (selected) {
                FillRR(r.left + 4.0f, midY - 9.0f, r.left + 7.0f, midY + 9.0f, 1.5f,
                       usable ? T.accent : T.accentDisabled);
            }

            DrawRadio(r.left + 28.0f, midY, selected, usable);

            Text(item.title, F_ROWTITLE, r.left + 50.0f, r.top + 6.0f,  r.right - 56.0f, r.top + 26.0f, usable ? T.text : T.textDisabled);
            Text(item.desc,  F_BODY,     r.left + 50.0f, r.top + 25.0f, r.right - 56.0f, r.top + 43.0f, usable ? T.textSec : T.textDisabled);

            const Pk dg = !usable ? T.textDisabled : (selected ? T.accent : T.textMuted);
            DrawModeDiagram(r.right - 25.0f, midY, item.value, dg);
        } else {
            DrawInfoGlyph(r.left + 28.0f, midY, T.textMuted);
            Text(L"Requires monitor hardware scaler", F_BODY, r.left + 50.0f, r.top, r.right - 14.0f, r.bottom, T.textMuted);
        }

        if (i < 3) {
            const float px = g_ui.invScale;
            const float dy = Snap(r.bottom) - px * 0.5f;
            Line(r.left + 48.0f, dy, r.right - 14.0f, dy, T.divider, px);
        }
    }

    // 5. InfoBar
    const int k = static_cast<int>(g_ui.statusKind);
    FillRR(kInfoBar, 6.0f, T.ibBg[k]);
    StrokeRR(kInfoBar, 6.0f, T.ibBorder[k]);
    DrawStatusIcon(kInfoBar.left + 22.0f, (kInfoBar.top + kInfoBar.bottom) * 0.5f, g_ui.statusKind);
    Text(g_ui.statusText, F_STATUS, kInfoBar.left + 42.0f, kInfoBar.top + 3.0f, kInfoBar.right - 12.0f, kInfoBar.bottom - 3.0f, T.text);

    // 6. Buttons
    {
        const bool pressed = !g_ui.busy && (g_ui.pressed == HitTarget::RefreshBtn);
        const bool hovered = !g_ui.busy && (g_ui.hover == HitTarget::RefreshBtn);
        const Pk fill = pressed ? T.cardPress : (hovered ? T.cardHover : T.card);
        const Pk fg   = g_ui.busy ? T.textDisabled : T.text;
        Card(kRefreshBtn, 5.0f, fill);
        const float gw = 14.0f + 8.0f + g_ui.twRefresh;
        const float x0 = (kRefreshBtn.left + kRefreshBtn.right - gw) * 0.5f;
        const float cy = (kRefreshBtn.top + kRefreshBtn.bottom) * 0.5f;
        DrawRefreshGlyph(x0 + 7.0f, cy, fg);
        Text(L"Refresh", F_BUTTON, x0 + 22.0f, kRefreshBtn.top, x0 + 26.0f + g_ui.twRefresh, kRefreshBtn.bottom, fg);
    }
    {
        const bool enabled = CanCommitSelection();
        const bool pressed = enabled && (g_ui.pressed == HitTarget::ApplyBtn);
        const bool hovered = enabled && (g_ui.hover == HitTarget::ApplyBtn);
        const Pk fill = !enabled ? T.accentDisabled
                       : (pressed ? T.accentPress : (hovered ? T.accentHover : T.accent));
        FillRR(kApplyBtn, 5.0f, fill);
        if (enabled) {
            Line(kApplyBtn.left + 5.0f, kApplyBtn.top + 0.5f, kApplyBtn.right - 5.0f, kApplyBtn.top + 0.5f,
                 WithAlpha(Rgb(0xFFFFFF), 0.20f), g_ui.invScale);
        }
        Text(g_ui.applyLabel[0] ? g_ui.applyLabel : L"Apply", F_BUTTON, kApplyBtn,
             enabled ? T.onAccent : T.onAccentDisabled);
    }

    if (g_ui.dropdownOpen) {
        PaintDropdown(W);
    }
}

static void PaintWindow(HWND hwnd) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);

    RECT rc;
    GetClientRect(hwnd, &rc);
    const int width  = rc.right - rc.left;
    const int height = rc.bottom - rc.top;
    bool recreate    = false;

    if (width > 0 && height > 0 &&
        EnsureTarget(hwnd, static_cast<UINT32>(width), static_cast<UINT32>(height))) {
        g_gfx.rt->BeginDraw();
        const Col clear = ToCol(g_ui.theme->canvas);
        g_gfx.rt->Clear(&clear);
        PaintAll(static_cast<float>(width) * g_ui.invScale);
        if (g_gfx.rt->EndDraw() == D2DERR_RECREATE_TARGET) {
            recreate = true;
        }
    }

    EndPaint(hwnd, &ps);

    if (recreate) {
        ReleaseTarget();
        Repaint(hwnd);
    }
}

// ----------------------------------------------------------------------------
// User Actions
// ----------------------------------------------------------------------------

// Re-evaluates hover from the real pointer position (after a modal state or keyboard
// selection change) and arms TME_LEAVE if the pointer is over an interactive target.
static bool SyncHover(HWND hwnd, bool restoreCursor) {
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(hwnd, &pt);
    const HitTarget next    = HitTest(pt);
    const bool      changed = (next != g_ui.hover);
    g_ui.hover = next;

    RECT rc;
    GetClientRect(hwnd, &rc);
    const bool inside = (pt.x >= 0 && pt.x < rc.right && pt.y >= 0 && pt.y < rc.bottom);
    if (inside && next != HitTarget::None && !g_ui.trackingMouse) {
        TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hwnd, 0 };
        TrackMouseEvent(&tme);
        g_ui.trackingMouse = true;
    }
    if (restoreCursor && inside) {
        SetCursor(next != HitTarget::None ? g_ui.curHand : g_ui.curArrow);
    }
    return changed;
}

static constexpr const wchar_t* kIdentifyClass = L"ScalingSwitcherIdentifyWnd";

static void CloseIdentifyWindows(HWND mainHwnd) {
    ReleaseIdentifyGfx();
    if (!g_ui.identifyOpen) return;
    g_ui.identifyOpen = false;
    if (mainHwnd) KillTimer(mainHwnd, kTimerIdentify);
    for (size_t i = 0; i < 16; ++i) {
        if (g_ui.identifyHwnds[i]) {
            HWND hPop = g_ui.identifyHwnds[i];
            g_ui.identifyHwnds[i] = nullptr;
            DestroyWindow(hPop);
        }
    }
}

static LRESULT CALLBACK IdentifyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            const auto* cs = reinterpret_cast<const CREATESTRUCTW*>(lParam);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            return 0;
        }
        case WM_NCHITTEST:
            return HTTRANSPARENT;
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;
        case WM_ERASEBKGND:
            return 1;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);

            RECT rc;
            GetClientRect(hwnd, &rc);
            const uintptr_t idx1 = static_cast<uintptr_t>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));

            if (g_gfx.d2d && rc.right > rc.left && rc.bottom > rc.top &&
                idx1 >= 1 && (idx1 - 1) < g_ui.displayCount) {
                if (!g_gfx.idRt) {
                    D2D1_RENDER_TARGET_PROPERTIES props = {};
                    props.type                  = D2D1_RENDER_TARGET_TYPE_DEFAULT;
                    props.pixelFormat.format    = DXGI_FORMAT_B8G8R8A8_UNORM;
                    props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
                    props.dpiX                  = 96.0f;
                    props.dpiY                  = 96.0f;
                    if (SUCCEEDED(g_gfx.d2d->CreateDCRenderTarget(&props, &g_gfx.idRt)) && g_gfx.idRt) {
                        g_gfx.idRt->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                        g_gfx.idRt->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
                        const Col zero = {};
                        if (FAILED(g_gfx.idRt->CreateSolidColorBrush(&zero, nullptr, &g_gfx.idBrush)) || !g_gfx.idBrush) {
                            ReleaseIdentifyGfx();
                        }
                    }
                }

                if (g_gfx.idRt && g_gfx.idBrush) {
                    const size_t idx = idx1 - 1;
                    const NvDisplayInfo& d = g_ui.displays[idx];
                    const bool isTarget = (idx == g_ui.selectedDispIdx);
                    UINT dpi = GetDpiForWindow(hwnd);
                    if (dpi == 0) dpi = 96;

                    g_gfx.idRt->SetDpi(static_cast<float>(dpi), static_cast<float>(dpi));
                    if (SUCCEEDED(g_gfx.idRt->BindDC(ps.hdc, &rc))) {
                        g_gfx.idRt->BeginDraw();

                        const Col bg = ToCol(Rgb(0x1C1C1C));
                        g_gfx.idRt->Clear(&bg);

                        ID2D1SolidColorBrush* br = g_gfx.idBrush;
                        auto setC = [br](Pk pk) {
                            const Col c = ToCol(pk);
                            br->SetColor(&c);
                        };

                        const D2D1_ROUNDED_RECT badge = { { 15.0f, 15.0f, 71.0f, 71.0f }, 8.0f, 8.0f };
                        setC(isTarget ? kDarkTheme.accent : Rgb(0x323232));
                        g_gfx.idRt->FillRoundedRectangle(&badge, br);

                        wchar_t numBuf[4] = {};
                        StrUtil::SafeFormat(numBuf, 4, L"%u", static_cast<uint32_t>(idx + 1));
                        setC(isTarget ? kDarkTheme.onAccent : Rgb(0xFFFFFF));
                        g_gfx.idRt->DrawTextW(numBuf, static_cast<UINT32>(StrUtil::Len(numBuf)), g_gfx.fmt[F_IDNUM],
                                              &badge.rect, br, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);

                        const wchar_t* title = d.monitorName[0] ? d.monitorName : NvApi::ShortDisplayName(d.displayName);
                        const D2D1_RECT_F tRc = { 84.0f, 18.0f, 278.0f, 42.0f };
                        setC(Rgb(0xFFFFFF));
                        g_gfx.idRt->DrawTextW(title, static_cast<UINT32>(StrUtil::Len(title)), g_gfx.fmt[F_ROWTITLE],
                                              &tRc, br, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);

                        wchar_t sub[96] = {};
                        const wchar_t* shortName = NvApi::ShortDisplayName(d.displayName);
                        if (isTarget) {
                            StrUtil::SafeFormat(sub, 96, L"%s \x00B7 Selected target", shortName);
                        } else {
                            const wchar_t* hint = NvApi::GetSpatialHint(g_ui.displays, g_ui.displayCount, idx);
                            if (hint) StrUtil::SafeFormat(sub, 96, L"%s \x00B7 %s", shortName, hint);
                            else      StrUtil::SafeCopy(sub, 96, shortName);
                        }
                        const D2D1_RECT_F sRc = { 84.0f, 43.0f, 278.0f, 66.0f };
                        setC(isTarget ? kDarkTheme.accent : Rgb(0xB4B4B4));
                        g_gfx.idRt->DrawTextW(sub, static_cast<UINT32>(StrUtil::Len(sub)), g_gfx.fmt[F_BODY],
                                              &sRc, br, D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);

                        if (g_gfx.idRt->EndDraw() == D2DERR_RECREATE_TARGET) {
                            ReleaseIdentifyGfx();
                        }
                    }
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static BOOL CALLBACK IdentifyEnumProc(HMONITOR hMon, HDC, LPRECT, LPARAM lParam) {
    HWND mainHwnd = reinterpret_cast<HWND>(lParam);
    MONITORINFOEXW mi = {};
    mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoW(hMon, &mi)) return TRUE;

    for (size_t i = 0; i < g_ui.displayCount; ++i) {
        if (g_ui.identifyHwnds[i]) continue;
        if (!StrUtil::AsciiEqualsI(mi.szDevice, g_ui.displays[i].displayName)) continue;

        HWND hPop = CreateWindowExW(
            WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
            kIdentifyClass, L"", WS_POPUP,
            mi.rcWork.left, mi.rcWork.top, 10, 10,
            mainHwnd, nullptr, GetModuleHandleW(nullptr),
            reinterpret_cast<LPVOID>(static_cast<uintptr_t>(i + 1)));
        if (!hPop) break;

        g_ui.identifyHwnds[i] = hPop;
        UINT monDpi = GetDpiForWindow(hPop);
        if (monDpi == 0) monDpi = 96;

        const int popW = ScaleDpi(292, monDpi);
        const int popH = ScaleDpi(86,  monDpi);
        int x = mi.rcWork.left   + ScaleDpi(28, monDpi);
        int y = mi.rcWork.bottom - popH - ScaleDpi(28, monDpi);
        if (x + popW > mi.rcWork.right) x = mi.rcWork.left;
        if (y < mi.rcWork.top)          y = mi.rcWork.top;

        if (FnDwmSetWindowAttribute dwmSet = GetDwmSet()) {
            BOOL     dark   = TRUE;
            DWORD    corner = 2;
            COLORREF border = (i == g_ui.selectedDispIdx) ? ToColorRef(kDarkTheme.accent) : RGB(72, 72, 72);
            dwmSet(hPop, 20, &dark,   sizeof(dark));
            dwmSet(hPop, 33, &corner, sizeof(corner));
            dwmSet(hPop, 34, &border, sizeof(border));
        }

        SetWindowPos(hPop, HWND_TOPMOST, x, y, popW, popH, SWP_NOACTIVATE | SWP_SHOWWINDOW);
        break;
    }
    return TRUE;
}

static void IdentifyMonitors(HWND mainHwnd) {
    if (g_ui.displayCount <= 1 || g_ui.busy) return;
    CloseIdentifyWindows(mainHwnd);
    g_ui.identifyOpen = true;
    EnumDisplayMonitors(nullptr, nullptr, IdentifyEnumProc, reinterpret_cast<LPARAM>(mainHwnd));
    SetTimer(mainHwnd, kTimerIdentify, 2200, nullptr);
}

static void OpenDropdown(HWND hwnd) {
    if (g_ui.displayCount <= 1 || g_ui.busy) return;
    if (g_ui.dropdownOpen) {
        g_ui.dropdownOpen = false;
        SyncHover(hwnd, true);
        Repaint(hwnd);
        return;
    }
    g_ui.dropdownOpen  = true;
    g_ui.dropHighlight = g_ui.selectedDispIdx;

    const size_t vis = DropVisibleCount();
    if (g_ui.selectedDispIdx < g_ui.dropScroll) {
        g_ui.dropScroll = g_ui.selectedDispIdx;
    } else if (vis > 0 && g_ui.selectedDispIdx >= g_ui.dropScroll + vis) {
        g_ui.dropScroll = g_ui.selectedDispIdx - vis + 1;
    }
    const size_t maxScroll = (g_ui.displayCount > vis) ? (g_ui.displayCount - vis) : 0;
    if (g_ui.dropScroll > maxScroll) g_ui.dropScroll = maxScroll;

    SyncHover(hwnd, true);
    Repaint(hwnd);
}

static void SelectDisplay(HWND hwnd, size_t idx) {
    if (idx >= g_ui.displayCount) return;
    g_ui.selectedDispIdx = idx;
    g_ui.dropdownOpen    = false;
    SyncSelectionFromDisplay();
    RefreshDisplayCache();

    const NvDisplayInfo& d = g_ui.displays[idx];
    const wchar_t* name      = d.monitorName[0] ? d.monitorName : NvApi::ShortDisplayName(d.displayName);
    const wchar_t* shortName = NvApi::ShortDisplayName(d.displayName);
    wchar_t msg[256] = {};
    if (d.foundInConfig) {
        StrUtil::SafeFormat(msg, 256, L"#%u %s (%s) \x2014 current: %s.",
                            static_cast<uint32_t>(idx + 1), name, shortName,
                            NvApi::GetScalingLabel(d.scaling));
    } else {
        StrUtil::SafeFormat(msg, 256, L"#%u %s (%s) is not active in NVIDIA config.",
                            static_cast<uint32_t>(idx + 1), name, shortName);
    }
    SetStatus(d.foundInConfig ? StatusKind::Neutral : StatusKind::Error, msg);

    if (g_ui.identifyOpen) {
        FnDwmSetWindowAttribute dwmSet = GetDwmSet();
        for (size_t i = 0; i < 16; ++i) {
            if (g_ui.identifyHwnds[i]) {
                if (dwmSet) {
                    COLORREF border = (i == idx) ? ToColorRef(kDarkTheme.accent) : RGB(72, 72, 72);
                    dwmSet(g_ui.identifyHwnds[i], 34, &border, sizeof(border));
                }
                InvalidateRect(g_ui.identifyHwnds[i], nullptr, FALSE);
            }
        }
    }
    SyncHover(hwnd, true);
    Repaint(hwnd);
}

static void OnRefresh(HWND hwnd) {
    CloseIdentifyWindows(hwnd);
    g_ui.dropdownOpen = false;

    uint32_t preferredId = 0;
    const NvDisplayInfo* cur = GetSelectedDisplay();
    if (cur) preferredId = cur->displayId;

    g_ui.busy = true;
    SetCursor(g_ui.curWait);
    SetStatus(StatusKind::Busy, L"Refreshing NVIDIA displays...");
    Repaint(hwnd);
    UpdateWindow(hwnd);

    ReloadDisplays(preferredId);
    if (g_ui.displayCount > 1) {
        wchar_t msg[128] = {};
        StrUtil::SafeFormat(msg, 128, L"Refreshed %u NVIDIA displays.",
                            static_cast<uint32_t>(g_ui.displayCount));
        SetStatus(StatusKind::Neutral, msg);
    } else if (g_ui.displayCount > 0) {
        SetStatus(StatusKind::Neutral, L"Display list refreshed.");
    }

    g_ui.busy = false;
    SyncHover(hwnd, true);
    Repaint(hwnd);
}

static void OnApply(HWND hwnd) {
    if (!CanCommitSelection()) return;

    const NvDisplayInfo* disp = GetSelectedDisplay();
    const uint32_t  targetId   = disp->displayId;
    const uint32_t  dispNum    = static_cast<uint32_t>(g_ui.selectedDispIdx + 1);
    wchar_t dispName[128] = {};
    StrUtil::SafeCopy(dispName, 128, disp->monitorName[0] ? disp->monitorName : NvApi::ShortDisplayName(disp->displayName));
    const NvScaling targetMode = g_ui.selectedMode;

    g_ui.busy = true;
    SetCursor(g_ui.curWait);

    wchar_t busyMsg[256] = {};
    if (g_ui.displayCount > 1) {
        StrUtil::SafeFormat(busyMsg, 256, L"Applying %s to #%u %s...",
                            NvApi::GetScalingLabel(targetMode), dispNum, dispName);
    } else {
        StrUtil::SafeFormat(busyMsg, 256, L"Applying %s...",
                            NvApi::GetScalingLabel(targetMode));
    }
    SetStatus(StatusKind::Busy, busyMsg);
    Repaint(hwnd);
    UpdateWindow(hwnd);

    NvApplyResult res = NvApi::SetScaling(targetId, targetMode);

    // SetScaling already read the live driver state back; reuse it instead of re-querying
    // NVAPI + Windows CCD for the whole display list. Fall back to a full reload otherwise.
    NvDisplayInfo* cur = (g_ui.selectedDispIdx < g_ui.displayCount) ? &g_ui.displays[g_ui.selectedDispIdx] : nullptr;
    if (res.haveActual && cur && cur->displayId == targetId) {
        cur->scaling               = res.actualScaling;
        cur->width                 = res.width;
        cur->height                = res.height;
        cur->refreshRateMillihertz = res.refreshRateMillihertz;
        SyncSelectionFromDisplay();
        RefreshDisplayCache();
    } else {
        ReloadDisplays(targetId);
    }

    if (g_ui.displayCount > 1) {
        wchar_t finalMsg[256] = {};
        StrUtil::SafeFormat(finalMsg, 256, L"#%u %s: %s", dispNum, dispName, res.message);
        SetStatus(res.success ? StatusKind::Success : StatusKind::Error, finalMsg);
    } else {
        SetStatus(res.success ? StatusKind::Success : StatusKind::Error, res.message);
    }

    g_ui.busy = false;
    SyncHover(hwnd, true);
    Repaint(hwnd);
}

static void ActivateTarget(HWND hwnd, HitTarget target) {
    switch (target) {
        case HitTarget::DisplayCard:
            OpenDropdown(hwnd);
            break;
        case HitTarget::IdentifyBtn:
            IdentifyMonitors(hwnd);
            break;
        case HitTarget::SegmentDisplay:
            if (SwitchScalerGroup(ScalerGroup::Display)) {
                SyncHover(hwnd, true);
                Repaint(hwnd);
            }
            break;
        case HitTarget::SegmentGpu:
            if (SwitchScalerGroup(ScalerGroup::Gpu)) {
                SyncHover(hwnd, true);
                Repaint(hwnd);
            }
            break;
        case HitTarget::Row0:
        case HitTarget::Row1:
        case HitTarget::Row2:
        case HitTarget::Row3: {
            size_t idx = static_cast<size_t>(target) - static_cast<size_t>(HitTarget::Row0);
            size_t count = 0;
            const ModeItem* items = GetModes(g_ui.activeGroup, count);
            if (idx < count && (!g_ui.hasSelectedMode || g_ui.selectedMode != items[idx].value)) {
                g_ui.selectedMode    = items[idx].value;
                g_ui.hasSelectedMode = true;
                SyncHover(hwnd, true);
                Repaint(hwnd);
            }
            break;
        }
        case HitTarget::RefreshBtn:
            OnRefresh(hwnd);
            break;
        case HitTarget::ApplyBtn:
            OnApply(hwnd);
            break;
        default:
            if (target >= HitTarget::DropRow0 && target <= HitTarget::DropRow5) {
                const size_t idx = g_ui.dropScroll +
                    (static_cast<size_t>(target) - static_cast<size_t>(HitTarget::DropRow0));
                SelectDisplay(hwnd, idx);
            }
            break;
    }
}

// ----------------------------------------------------------------------------
// Window Procedure & Entry Point
// ----------------------------------------------------------------------------
static void CalcWindowSize(UINT dpi, int& outW, int& outH) {
    constexpr DWORD kStyle   = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    constexpr DWORD kExStyle = WS_EX_APPWINDOW;
    RECT rc = { 0, 0, ScaleDpi(kBaseWidth, dpi), ScaleDpi(kBaseHeight, dpi) };
    AdjustWindowRectExForDpi(&rc, kStyle, FALSE, kExStyle, dpi);
    outW = rc.right - rc.left;
    outH = rc.bottom - rc.top;
}

static bool CenterInWorkArea(int winW, int winH, int& outX, int& outY) {
    RECT workRc = {};
    if (!SystemParametersInfoW(SPI_GETWORKAREA, 0, &workRc, 0)) return false;
    const int workW = workRc.right - workRc.left;
    const int workH = workRc.bottom - workRc.top;
    outX = workRc.left + ((workW > winW) ? (workW - winW) / 2 : 0);
    outY = workRc.top  + ((workH > winH) ? (workH - winH) / 2 : 0);
    return true;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            const UINT winDpi = GetDpiForWindow(hwnd);
            if (winDpi > 0 && winDpi != g_ui.dpi) {
                SetDpiState(winDpi);
                int winW = 0, winH = 0, posX = 0, posY = 0;
                CalcWindowSize(winDpi, winW, winH);
                const bool centered = CenterInWorkArea(winW, winH, posX, posY);
                SetWindowPos(hwnd, nullptr, posX, posY, winW, winH,
                             SWP_NOZORDER | SWP_NOACTIVATE | (centered ? 0u : SWP_NOMOVE));
            }
            if (FnDwmSetWindowAttribute dwmSet = GetDwmSet()) {
                DWORD corner = 2;
                dwmSet(hwnd, 33, &corner, sizeof(corner));
            }
            RefreshTheme(hwnd);
            ReloadDisplays(0, hwnd);

            if (g_ui.displayCount > 0) {
                SetStatus(StatusKind::Neutral, L"Ready. Select a scaling mode and click Apply.");
            }
            return 0;
        }

        case WM_ERASEBKGND:
            return 1;

        case WM_SIZE: {
            if (wParam != SIZE_MINIMIZED && g_gfx.rt) {
                const UINT32 w = LOWORD(lParam);
                const UINT32 h = HIWORD(lParam);
                if (w > 0 && h > 0) {
                    D2D1_SIZE_U sz = { w, h };
                    g_gfx.rt->Resize(&sz);
                }
            }
            return 0;
        }

        case WM_PAINT:
            PaintWindow(hwnd);
            return 0;

        case WM_DPICHANGED: {
            CloseIdentifyWindows(hwnd);
            SetDpiState(HIWORD(wParam));
            const RECT* suggested = reinterpret_cast<const RECT*>(lParam);
            SetWindowPos(hwnd, nullptr,
                         suggested->left, suggested->top,
                         suggested->right - suggested->left,
                         suggested->bottom - suggested->top,
                         SWP_NOZORDER | SWP_NOACTIVATE);
            Repaint(hwnd);
            return 0;
        }

        case WM_SETTINGCHANGE: {
            if ((!lParam || StrUtil::AsciiEqualsI(reinterpret_cast<LPCWSTR>(lParam), L"ImmersiveColorSet")) &&
                RefreshTheme(hwnd)) {
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_DISPLAYCHANGE: {
            if (!g_ui.busy) {
                CloseIdentifyWindows(hwnd);
                g_ui.dropdownOpen = false;
                const NvDisplayInfo* cur = GetSelectedDisplay();
                ReloadDisplays(cur ? cur->displayId : 0);
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_ACTIVATE: {
            if (LOWORD(wParam) == WA_INACTIVE &&
                (g_ui.dropdownOpen || g_ui.hover != HitTarget::None)) {
                g_ui.dropdownOpen = false;
                g_ui.hover        = HitTarget::None;
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_TIMER: {
            if (wParam == kTimerIdentify) {
                CloseIdentifyWindows(hwnd);
            }
            return 0;
        }

        case WM_KEYDOWN: {
            if (!g_ui.busy) {
                if (g_ui.dropdownOpen) {
                    if (wParam == VK_ESCAPE || wParam == VK_F4) {
                        g_ui.dropdownOpen = false;
                        SyncHover(hwnd, true);
                        Repaint(hwnd);
                    } else if (wParam == VK_UP && g_ui.dropHighlight > 0) {
                        --g_ui.dropHighlight;
                        if (g_ui.dropHighlight < g_ui.dropScroll) g_ui.dropScroll = g_ui.dropHighlight;
                        Repaint(hwnd);
                    } else if (wParam == VK_DOWN && g_ui.dropHighlight + 1 < g_ui.displayCount) {
                        ++g_ui.dropHighlight;
                        const size_t vis = DropVisibleCount();
                        if (g_ui.dropHighlight >= g_ui.dropScroll + vis) {
                            g_ui.dropScroll = g_ui.dropHighlight - vis + 1;
                        }
                        Repaint(hwnd);
                    } else if ((wParam == VK_RETURN || wParam == VK_SPACE) && (lParam & (1LL << 30)) == 0) {
                        SelectDisplay(hwnd, g_ui.dropHighlight);
                    }
                    return 0;
                }

                if (wParam == VK_F5) {
                    OnRefresh(hwnd);
                } else if (wParam == VK_F4) {
                    OpenDropdown(hwnd);
                } else if (wParam == 'I' && (lParam & (1LL << 30)) == 0) {
                    IdentifyMonitors(hwnd);
                } else if (wParam >= '1' && wParam <= '9' && (lParam & (1LL << 30)) == 0) {
                    const size_t idx = static_cast<size_t>(wParam - '1');
                    if (g_ui.displayCount > 1 && idx < g_ui.displayCount) SelectDisplay(hwnd, idx);
                } else if (wParam == VK_RETURN && (lParam & (1LL << 30)) == 0) {
                    OnApply(hwnd);
                } else if (wParam == VK_ESCAPE) {
                    DefWindowProcW(hwnd, WM_CLOSE, 0, 0);
                } else if ((wParam == VK_LEFT || wParam == VK_RIGHT) && CanApplyCurrentDisplay()) {
                    if (SwitchScalerGroup(wParam == VK_LEFT ? ScalerGroup::Display : ScalerGroup::Gpu)) {
                        SyncHover(hwnd, true);
                        Repaint(hwnd);
                    }
                } else if ((wParam == VK_UP || wParam == VK_DOWN) && CanApplyCurrentDisplay()) {
                    size_t count = 0;
                    const ModeItem* items = GetModes(g_ui.activeGroup, count);
                    size_t idx = g_ui.hasSelectedMode ? FindMode(g_ui.activeGroup, g_ui.selectedMode, count) : count;
                    if (idx >= count) {
                        idx = 0;
                    } else if (wParam == VK_UP && idx > 0) {
                        --idx;
                    } else if (wParam == VK_DOWN && idx + 1 < count) {
                        ++idx;
                    }
                    if (!g_ui.hasSelectedMode || g_ui.selectedMode != items[idx].value) {
                        g_ui.selectedMode    = items[idx].value;
                        g_ui.hasSelectedMode = true;
                        SyncHover(hwnd, true);
                        Repaint(hwnd);
                    }
                }
            }
            return 0;
        }

        case WM_MOUSEWHEEL: {
            if (g_ui.dropdownOpen && g_ui.displayCount > kMaxDropVis) {
                const short delta = static_cast<short>(HIWORD(wParam));
                const size_t maxScroll = g_ui.displayCount - DropVisibleCount();
                if (delta > 0 && g_ui.dropScroll > 0) {
                    --g_ui.dropScroll;
                } else if (delta < 0 && g_ui.dropScroll < maxScroll) {
                    ++g_ui.dropScroll;
                }
                SyncHover(hwnd, true);
                Repaint(hwnd);
                return 0;
            }
            break;
        }

        case WM_MOUSEMOVE: {
            if (!g_ui.trackingMouse) {
                TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, hwnd, 0 };
                TrackMouseEvent(&tme);
                g_ui.trackingMouse = true;
            }
            POINT pt = { static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam)) };
            HitTarget nextHover = HitTest(pt);
            bool needPaint = (nextHover != g_ui.hover);
            g_ui.hover = nextHover;
            if (g_ui.dropdownOpen && nextHover >= HitTarget::DropRow0 && nextHover <= HitTarget::DropRow5) {
                const size_t row = g_ui.dropScroll +
                    (static_cast<size_t>(nextHover) - static_cast<size_t>(HitTarget::DropRow0));
                if (row < g_ui.displayCount && row != g_ui.dropHighlight) {
                    g_ui.dropHighlight = row;
                    needPaint = true;
                }
            }
            if (needPaint) Repaint(hwnd);
            return 0;
        }

        case WM_MOUSELEAVE: {
            g_ui.trackingMouse = false;
            if (g_ui.hover != HitTarget::None) {
                g_ui.hover = HitTarget::None;
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            SetFocus(hwnd);
            POINT pt = { static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam)) };
            if (g_ui.dropdownOpen) {
                const float x = static_cast<float>(pt.x) * g_ui.invScale;
                const float y = static_cast<float>(pt.y) * g_ui.invScale;
                const D2D1_RECT_F dropRc = GetDropdownRect();
                const bool inCard = (x >= kDisplayCard.left && x < kDisplayCard.right &&
                                     y >= kDisplayCard.top  && y < kDisplayCard.bottom);
                const bool inDrop = (x >= dropRc.left && x < dropRc.right &&
                                     y >= dropRc.top  && y < dropRc.bottom);
                if (!inCard && !inDrop) {
                    g_ui.dropdownOpen = false;
                    SyncHover(hwnd, true);
                    Repaint(hwnd);
                    return 0;
                }
            }
            HitTarget hit = HitTest(pt);
            if (hit != HitTarget::None) {
                g_ui.pressed = hit;
                SetCapture(hwnd);
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_LBUTTONUP: {
            if (g_ui.pressed != HitTarget::None) {
                HitTarget target = g_ui.pressed;
                g_ui.pressed = HitTarget::None;
                ReleaseCapture();

                POINT pt = { static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam)) };
                HitTarget upHit = HitTest(pt);
                Repaint(hwnd);

                if (upHit == target) {
                    ActivateTarget(hwnd, target);
                }
            }
            return 0;
        }

        case WM_CAPTURECHANGED: {
            if (g_ui.pressed != HitTarget::None) {
                g_ui.pressed = HitTarget::None;
                Repaint(hwnd);
            }
            return 0;
        }

        case WM_SETCURSOR: {
            if (LOWORD(lParam) == HTCLIENT) {
                if (g_ui.busy) {
                    SetCursor(g_ui.curWait);
                } else if (g_ui.trackingMouse) {
                    SetCursor((g_ui.hover != HitTarget::None) ? g_ui.curHand : g_ui.curArrow);
                } else {
                    SyncHover(hwnd, true);
                }
                return TRUE;
            }
            break;
        }

        case WM_DESTROY:
            CloseIdentifyWindows(hwnd);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

static int RunApp(HINSTANCE hInstance) {
    if (!InitD2D()) {
        ShutdownD2D();
        MessageBoxW(nullptr, L"Direct2D / DirectWrite could not be initialized.", L"Scaling Switcher",
                    MB_OK | MB_ICONERROR);
        return 1;
    }

    const UINT dpi = GetDpiForSystem();
    SetDpiState(dpi);
    const int smIcon = ScaleDpi(16, dpi);

    g_ui.curArrow = LoadCursorW(nullptr, IDC_ARROW);
    g_ui.curHand  = LoadCursorW(nullptr, IDC_HAND);
    g_ui.curWait  = LoadCursorW(nullptr, IDC_WAIT);

    HICON hIconBig = static_cast<HICON>(LoadImageW(
        hInstance, MAKEINTRESOURCEW(IDI_APP_ICON), IMAGE_ICON,
        0, 0, LR_DEFAULTCOLOR | LR_DEFAULTSIZE | LR_SHARED));
    HICON hIconSmall = static_cast<HICON>(LoadImageW(
        hInstance, MAKEINTRESOURCEW(IDI_APP_ICON), IMAGE_ICON,
        smIcon, smIcon, LR_DEFAULTCOLOR | LR_SHARED));

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = g_ui.curArrow;
    wc.hIcon         = hIconBig;
    wc.hIconSm       = hIconSmall;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"ScalingSwitcherWndClass";

    if (!RegisterClassExW(&wc)) {
        ShutdownD2D();
        return 1;
    }

    WNDCLASSEXW idWc = {};
    idWc.cbSize        = sizeof(idWc);
    idWc.lpfnWndProc   = IdentifyWndProc;
    idWc.hInstance     = hInstance;
    idWc.lpszClassName = kIdentifyClass;
    RegisterClassExW(&idWc);

    int winW = 0, winH = 0;
    CalcWindowSize(dpi, winW, winH);

    int posX = CW_USEDEFAULT;
    int posY = CW_USEDEFAULT;
    CenterInWorkArea(winW, winH, posX, posY);

    HWND hwnd = CreateWindowExW(WS_EX_APPWINDOW, wc.lpszClassName, L"Scaling Switcher",
                                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                posX, posY, winW, winH,
                                nullptr, nullptr, hInstance, nullptr);
    if (!hwnd) {
        ShutdownD2D();
        return 1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        DispatchMessageW(&msg);
    }

    ShutdownD2D();
    NvApi::Shutdown();
    return static_cast<int>(msg.wParam);
}

// Process entry point (linked with -nostdlib -Wl,-e,EntryPoint).
extern "C" void WINAPI EntryPoint() {
    ExitProcess(static_cast<UINT>(RunApp(GetModuleHandleW(nullptr))));
}
