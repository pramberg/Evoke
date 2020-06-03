#pragma once
#include "Core/Window.h"
#include <SDL.h>

namespace Evoke
{
    class SDLWindow : public Window
    {
    public:
        SDLWindow(const WindowProperties &inProperties);
        virtual ~SDLWindow();

        virtual void Update() override;

        virtual u32 GetWidth() const override { return mData.Width; }
        virtual u32 GetHeight() const override { return mData.Height; }

        virtual void SetVSync(bool inEnabled) override;
        virtual bool GetVSyncEnabled() const override { return mData.VSyncEnabled; }

    private:
        virtual void Init(const WindowProperties &inProperties);
        virtual void Close();
        static i32 HandleEvents(void *inUserData, const SDL_Event *inEvent);

    private:
        SDL_Window *mWindow;
        SDL_Renderer *mRenderer;

        struct WindowData
        {
            string Title;
            u32 Width;
            u32 Height;
            b8 VSyncEnabled;
        } mData;
    };
} // namespace Evoke