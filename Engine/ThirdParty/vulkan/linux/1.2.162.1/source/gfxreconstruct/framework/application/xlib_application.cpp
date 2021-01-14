/*
** Copyright (c) 2020 LunarG, Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and associated documentation files (the "Software"),
** to deal in the Software without restriction, including without limitation
** the rights to use, copy, modify, merge, publish, distribute, sublicense,
** and/or sell copies of the Software, and to permit persons to whom the
** Software is furnished to do so, subject to the following conditions:
**
** The above copyright notice and this permission notice shall be included in
** all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
** FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
** DEALINGS IN THE SOFTWARE.
*/

#include "application/xlib_application.h"

#include "application/xlib_window.h"
#include "util/logging.h"

#include <cstdlib>

GFXRECON_BEGIN_NAMESPACE(gfxrecon)
GFXRECON_BEGIN_NAMESPACE(application)

XlibApplication::XlibApplication(const std::string& name) : Application(name) {}

XlibApplication::~XlibApplication()
{
    if (display_ != nullptr)
    {
        const auto xlib = xlib_loader_.GetFunctionTable();
        xlib.CloseDisplay(display_);
    }
}

static int ErrorHandler(Display* display, XErrorEvent* error_event)
{
    GFXRECON_LOG_ERROR("Xlib error: %d", error_event->error_code);
    return 0;
}

// A reference-counting interface to to XOpenDisplay/XCloseDisplay which shares
// a single display connection among multiple windows, and closes it when the
// last window is destroyed. This is a workaround for an issue with the NVIDIA
// driver which registers a callback for XCloseDisplay that needs to happen
// before the ICD is unloaded at vkDestroyInstance time.
// https://github.com/KhronosGroup/Vulkan-LoaderAndValidationLayers/issues/1894
Display* XlibApplication::OpenDisplay()
{
    if (display_ == nullptr)
    {
        auto xlib = xlib_loader_.GetFunctionTable();
        display_  = xlib.OpenDisplay(nullptr);
    }
    ++display_open_count_;
    return display_;
}

void XlibApplication::CloseDisplay(Display* display)
{
    assert(display == display_);

    if ((--display_open_count_ == 0) && (display_ != nullptr))
    {
        auto xlib = xlib_loader_.GetFunctionTable();
        xlib.CloseDisplay(display_);
        display_ = nullptr;
    }
}

bool XlibApplication::Initialize(decode::FileProcessor* file_processor)
{
    if (!xlib_loader_.Initialize())
    {
        return false;
    }

    const auto xlib = xlib_loader_.GetFunctionTable();
    xlib.SetErrorHandler(ErrorHandler);

    display_ = xlib.OpenDisplay(nullptr);
    if (!display_)
    {
        return false;
    }

    SetFileProcessor(file_processor);

    return true;
}

bool XlibApplication::RegisterXlibWindow(XlibWindow* window)
{
    return Application::RegisterWindow(window);
}

bool XlibApplication::UnregisterXlibWindow(XlibWindow* window)
{
    return Application::UnregisterWindow(window);
}

void XlibApplication::ProcessEvents(bool wait_for_input)
{
    const auto xlib = xlib_loader_.GetFunctionTable();
    while (IsRunning() && (wait_for_input || (xlib.Pending(display_) > 0)))
    {
        wait_for_input = false;

        XEvent event;
        xlib.NextEvent(display_, &event);

        switch (event.type)
        {
            case KeyRelease:
                switch (event.xkey.keycode)
                {
                    case 0x9: // Escape
                        StopRunning();
                        break;
                    case 0x21: // p
                    case 0x41: // Space
                        SetPaused(!GetPaused());
                        break;
                    default:
                        break;
                }
                break;

            case KeyPress:
                switch (event.xkey.keycode)
                {
                    // Using XCB_KEY_PRESS for repeat when key is held down.
                    case 0x72: // Right arrow
                    case 0x39: // n
                        if (GetPaused())
                        {
                            PlaySingleFrame();
                        }
                        break;
                    default:
                        break;
                }

                break;
        }
    }
}

GFXRECON_END_NAMESPACE(application)
GFXRECON_END_NAMESPACE(gfxrecon)
