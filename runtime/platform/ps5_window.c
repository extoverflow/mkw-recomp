// PS5 build only: the window back end (gfx/nvn/host/gfx_wsys.h) on the console's one display, through RADV's
// VK_KHR_display (PS5_Vulkan: the swapchain goes to VideoOut). It takes gfx_window_sdl.c's place: no window, no event
// thread and no window events. The display mode is the one MKW_WINDOW_SIZE names when the display has it, else the
// display's first (its current) mode; the swapchain is that size and gfx_present.c scales the game's picture into it.
//
// The display-surface steps (displays, their modes, a plane that can show the display, the plane's alpha) follow the
// Vulkan samples' VulkanSwapChain::createDirect2DisplaySurface (PS5_VulkanTemplate base/VulkanSwapChain.cpp:569-701,
// MIT, Copyright (C) 2016 Sascha Willems), written again here in C.
#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "gfx_wsys.h"
#include "vk.h"
#include "platform/platform.h"

#define MAX_N 16

static int g_w = 0, g_h = 0;   // the display mode chosen at the first surface; 0 before

int gfx_window_vk_extensions(const char **out, int max) {
    static const char *const ext[] = {VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_DISPLAY_EXTENSION_NAME};
    int k = 0;
    for (int i = 0; i < 2 && k < max; i++) out[k++] = ext[i];
    return k;
}

// No query for a display before the surface exists: a family that draws can present (PS5_Vulkan's swapchain submits
// its flips from the queue that presents).
int gfx_window_vk_presentable(VkInstance inst, VkPhysicalDevice pd, uint32_t family) {
    (void)inst;
    VkQueueFamilyProperties q[MAX_N];
    uint32_t n = MAX_N;
    vkGetPhysicalDeviceQueueFamilyProperties(pd, &n, q);
    return family < n && (q[family].queueFlags & VK_QUEUE_GRAPHICS_BIT);
}

int wsys_open(void) { return 1; }

// The display, a mode of it and a plane that can show it; logs what the display offers, once.
static VkSurfaceKHR display_surface(VkInstance inst) {
    VkPhysicalDevice pds[MAX_N];
    uint32_t npd = MAX_N;
    if (vkEnumeratePhysicalDevices(inst, &npd, pds) < 0 || !npd) {
        rt_log("gfx: ps5: no physical device for the display");
        return VK_NULL_HANDLE;
    }
    VkPhysicalDevice pd = pds[0];
    VkDisplayPropertiesKHR disp[MAX_N];
    uint32_t nd = MAX_N;
    if (vkGetPhysicalDeviceDisplayPropertiesKHR(pd, &nd, disp) < 0 || !nd) {
        rt_log("gfx: ps5: no display (vkGetPhysicalDeviceDisplayPropertiesKHR)");
        return VK_NULL_HANDLE;
    }
    VkDisplayKHR display = disp[0].display;
    VkDisplayModePropertiesKHR modes[64];
    uint32_t nm = 64;
    if (vkGetDisplayModePropertiesKHR(pd, display, &nm, modes) < 0 || !nm) {
        rt_log("gfx: ps5: display %s has no modes", disp[0].displayName ? disp[0].displayName : "?");
        return VK_NULL_HANDLE;
    }
    int want_w, want_h, pick = 0;
    gfx_cfg_window_size(&want_w, &want_h);
    for (uint32_t i = 0; i < nm; i++) {
        const VkDisplayModeParametersKHR *m = &modes[i].parameters;
        rt_log("gfx: ps5: display %s mode %u: %ux%u at %.3f Hz", disp[0].displayName ? disp[0].displayName : "?", i,
               m->visibleRegion.width, m->visibleRegion.height, m->refreshRate / 1000.0);
        if ((int)m->visibleRegion.width == want_w && (int)m->visibleRegion.height == want_h && !pick) pick = (int)i + 1;
    }
    const VkDisplayModePropertiesKHR *mode = &modes[pick ? pick - 1 : 0];
    VkDisplayPlanePropertiesKHR planes[MAX_N];
    uint32_t np = MAX_N;
    if (vkGetPhysicalDeviceDisplayPlanePropertiesKHR(pd, &np, planes) < 0) np = 0;
    uint32_t plane = UINT32_MAX;
    for (uint32_t p = 0; p < np && plane == UINT32_MAX; p++) {
        if (planes[p].currentDisplay != VK_NULL_HANDLE && planes[p].currentDisplay != display) continue;
        VkDisplayKHR on[MAX_N];
        uint32_t non = MAX_N;
        if (vkGetDisplayPlaneSupportedDisplaysKHR(pd, p, &non, on) < 0) continue;
        for (uint32_t j = 0; j < non; j++)
            if (on[j] == display) plane = p;
    }
    if (plane == UINT32_MAX) {
        rt_log("gfx: ps5: no plane shows the display (%u planes)", np);
        return VK_NULL_HANDLE;
    }
    VkDisplayPlaneCapabilitiesKHR cap;
    vkGetDisplayPlaneCapabilitiesKHR(pd, mode->displayMode, plane, &cap);
    VkDisplayPlaneAlphaFlagBitsKHR alpha = VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR;
    static const VkDisplayPlaneAlphaFlagBitsKHR order[] = {
        VK_DISPLAY_PLANE_ALPHA_OPAQUE_BIT_KHR, VK_DISPLAY_PLANE_ALPHA_GLOBAL_BIT_KHR,
        VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_BIT_KHR, VK_DISPLAY_PLANE_ALPHA_PER_PIXEL_PREMULTIPLIED_BIT_KHR};
    for (int i = 3; i >= 0; i--)
        if (cap.supportedAlpha & order[i]) alpha = order[i];   // opaque first (the samples prefer premultiplied):
                                                               // nothing is behind the game's picture to blend with
    VkDisplaySurfaceCreateInfoKHR ci = {
        .sType = VK_STRUCTURE_TYPE_DISPLAY_SURFACE_CREATE_INFO_KHR,
        .displayMode = mode->displayMode,
        .planeIndex = plane,
        .planeStackIndex = planes[plane].currentStackIndex,
        .transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .globalAlpha = 1.0f,
        .alphaMode = alpha,
        .imageExtent = mode->parameters.visibleRegion,
    };
    VkSurfaceKHR s = VK_NULL_HANDLE;
    VkResult r = vkCreateDisplayPlaneSurfaceKHR(inst, &ci, NULL, &s);
    if (r < 0) {
        rt_log("gfx: ps5: vkCreateDisplayPlaneSurfaceKHR failed (%d)", r);
        return VK_NULL_HANDLE;
    }
    g_w = (int)mode->parameters.visibleRegion.width, g_h = (int)mode->parameters.visibleRegion.height;
    gfx_window_set_refresh(mode->parameters.refreshRate / 1000.0);
    rt_log("gfx: ps5: display surface %dx%d at %.3f Hz on plane %u%s", g_w, g_h, mode->parameters.refreshRate / 1000.0,
           plane, pick ? "" : " (MKW_WINDOW_SIZE is not a mode of the display: its first mode)");
    return s;
}

VkSurfaceKHR wsys_surface(VkInstance inst) { return display_surface(inst); }

void wsys_client_size(int *w, int *h) {
    if (g_w) *w = g_w, *h = g_h;
    else gfx_cfg_window_size(w, h);
}

int wsys_hdr(float *sdr_white, float *headroom) {   // SDR until the console's HDR setting is read (Step 5)
    *sdr_white = *headroom = 0;
    return 0;
}

int wsys_minimized(void) { return 0; }

void wsys_resize(int w, int h) {   // the display keeps its mode; the swapchain stays its size
    rt_log("gfx: ps5: resize to %dx%d ignored: the display keeps its mode", w, h);
}

// --probe-window: there is no window thread, so the focus change is delivered here, at once (gfx_wsys.h: "handled ...
// before these return"); no keyboard and no mouse, so no Esc and no clicks (0: the probe skips the click checks).
void wsys_probe_focus(int in) { gfx_window_focus_changed(in); }
void wsys_probe_escape(void) {}
int wsys_probe_click(int where) {
    (void)where;
    return 0;
}

void gfx_window_apply_settings(void) {}   // size and fullscreen: the display's mode is fixed once the surface exists
void gfx_window_request_close(void) { gfx_window_closed(); }

int gfx_window_monitor_size(int *w, int *h) {
    wsys_client_size(w, h);
    return 1;
}

int gfx_window_shape(int *w, int *h) {
    wsys_client_size(w, h);
    return *h > 0;
}

int gfx_window_lines(void) {
    int w, h;
    return gfx_window_shape(&w, &h) ? h : 0;
}

// Why no window can open (vk.h; gfx_window_sdl.c keeps SDL's reason): the PS5's display plane surface is made at
// vk_init and logs its own failure (vkCreateDisplayPlaneSurfaceKHR above), so there is no earlier reason to give.
const char *gfx_window_failed(void) { return NULL; }
