#include <3ds.h>
#include <citro2d.h>
#include "TitleScreen.h"
#include "LogoScreen.h"
#include "AutoSaveScreen.h"
#include "MainMenuScreen.h"
#include "Button.h"
#include "sound.h"  

#define MAX_SPRITES   14
#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

//static object_2d_info_t background_object;
//static C2D_Sprite background_sprite[MAX_SPRITES];
//static sprite_position_t background_position = {0.0f, 0.0f};
Sound sound;

int main()
{
    romfsInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    //consoleInit(GFX_BOTTOM, NULL);
    
    // Create screens
    C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    C2D_SpriteSheet sheet = NULL;
    //C2D_Sprite bg;


    sound.init();
    sound.playBGMRandom(); 


LogoScreen logo;
TitleScreen title;
AutoSaveScreen autosave;
MainMenuScreen mainmenu;
Button button;

logo.init();
title.init();
autosave.init();
mainmenu.init();
button.init();

enum State {
    LOGO,
    AUTOSAVE,
    TITLE
};

State state = LOGO;


while (aptMainLoop())
{
    hidScanInput();

    if (hidKeysDown() & KEY_START)
        break;

    sound.update();

    if (state == LOGO)
    {
        logo.update();

        if (logo.isFinished())
        {
            logo.exit();
            state = AUTOSAVE;
        }
    }
    else if (state == AUTOSAVE)
    {
        title.update();
        autosave.update();
        button.update();
        if (autosave.isFinished())
        {
            autosave.exit();
            state = TITLE; 
        }
    }
    else if (state == TITLE)
    {
        title.update();
        mainmenu.update();
        button.update();
    }

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    if (state == LOGO)
    {
        logo.draw(top);
    }
    else if (state == AUTOSAVE)
    {
        title.draw(top, bottom);
        autosave.draw(bottom);
        button.draw(top, 0);
    }
    else if (state == TITLE)
    {
        title.draw(top, bottom);
        mainmenu.draw(top, bottom);
        button.draw(top, 1);
    }

    C3D_FrameEnd(0);
}

    if (sheet)
        C2D_SpriteSheetFree(sheet);

    C2D_Fini();
    C3D_Fini();
    gfxExit();
    sound.exit();
    mainmenu.exit();
    button.exit();
    romfsExit();

    return 0;
}