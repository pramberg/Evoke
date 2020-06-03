#include "PCH.h"
#include "SDLWindow.h"

namespace Evoke
{
    static bool sSDLInitialized = false;

    Window *Window::Create(const WindowProperties &inProperties /* = WindowProperties() */)
    {
        return new SDLWindow(inProperties);
    }

    SDLWindow::SDLWindow(const WindowProperties &inProperties) : mWindow(nullptr), mRenderer(nullptr)
    {
        Init(inProperties);
    }

    SDLWindow::~SDLWindow()
    {
        Close();
    }

    void SDLWindow::Update()
    {
        SDL_PollEvent(nullptr);
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 255);
        SDL_RenderClear(mRenderer);
        SDL_RenderPresent(mRenderer);
    }

    void SDLWindow::SetVSync(bool inEnabled)
    {
        if (inEnabled == GetVSyncEnabled())
            return;

        mData.VSyncEnabled = inEnabled;
        SDL_DestroyRenderer(mRenderer);

        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | (mData.VSyncEnabled ? SDL_RENDERER_PRESENTVSYNC : 0));
    }

    void SDLWindow::Init(const WindowProperties &inProperties)
    {
        mData.Title = inProperties.Title;
        mData.Width = inProperties.Width;
        mData.Height = inProperties.Height;

        if (!sSDLInitialized)
        {
            i32 success = SDL_Init(SDL_INIT_VIDEO);
            EV_CORE_ASSERT(success == 0, "Failed to initialize SDL.");

            sSDLInitialized = true;
        }

        mWindow = SDL_CreateWindow(
            mData.Title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            static_cast<i32>(mData.Width), static_cast<i32>(mData.Height),
            SDL_WINDOW_RESIZABLE);

        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        SDL_RaiseWindow(mWindow);
        SDL_AddEventWatch(reinterpret_cast<SDL_EventFilter>(&SDLWindow::HandleEvents), this);
    }

    void SDLWindow::Close()
    {
        SDL_DestroyWindow(mWindow);
        SDL_DestroyRenderer(mRenderer);
    }

    i32 SDLWindow::HandleEvents(void *inUserData, const SDL_Event *inEvent)
    {
        auto window = reinterpret_cast<SDLWindow *>(inUserData);

        // Window events
        if (inEvent->type == SDL_WINDOWEVENT)
        {
            switch (inEvent->window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                window->mData.Width = inEvent->window.data1;
                window->mData.Height = inEvent->window.data2;
                window->OnWindowResized.Broadcast(window->mData.Width, window->mData.Height);
                SDL_SetRenderDrawColor(window->mRenderer, 255, 0, 0, 255);
                SDL_RenderClear(window->mRenderer);
                SDL_RenderPresent(window->mRenderer);
                return 0;
            case SDL_WINDOWEVENT_CLOSE:
                window->OnWindowClosed.Broadcast();
                return 0;
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_SHOWN:
            default:
                break;
            }
        }

        // Key events
        if (inEvent->type == SDL_KEYDOWN)
        {
            window->OnKeyPressed.Broadcast(inEvent->key.keysym.sym, inEvent->key.repeat);
            return 0;
        }

        if (inEvent->type == SDL_KEYUP)
        {
            window->OnKeyReleased.Broadcast(inEvent->key.keysym.sym);
            return 0;
        }

        // Mouse events
        if (inEvent->type == SDL_MOUSEMOTION)
        {
            window->OnMouseMoved.Broadcast(inEvent->motion.x, inEvent->motion.y);
            return 0;
        }

        if (inEvent->type == SDL_MOUSEBUTTONDOWN)
        {
            window->OnMouseButtonPressed.Broadcast(inEvent->button.button, inEvent->button.clicks);
            return 0;
        }

        if (inEvent->type == SDL_MOUSEBUTTONUP)
        {
            window->OnMouseButtonReleased.Broadcast(inEvent->button.button);
            return 0;
        }

        if (inEvent->type == SDL_MOUSEWHEEL)
        {
            window->OnMouseScrolled.Broadcast(inEvent->wheel.x, inEvent->wheel.y);
            return 0;
        }

        return 1;
    }

} // namespace Evoke
