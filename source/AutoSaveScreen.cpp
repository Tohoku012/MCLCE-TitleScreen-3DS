#include "AutoSaveScreen.h"
#include <3ds.h>
#include <cmath>
#include "Sound.h"
#include <3ds/services/cfgu.h>
extern Sound sound;

void AutoSaveScreen::init()
{
    panelSheet = C2D_SpriteSheetLoad("romfs:/gfx/AutoSavePanel.t3x");
    C2D_SpriteFromSheet(&panelSpr, panelSheet, 0);

    C2D_SpriteSetScale(&panelSpr, 1.0f, 1.0f); 
    C2D_SpriteSetCenter(&panelSpr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&panelSpr, 160.0f, 120.0f);

    ButtonSheet = C2D_SpriteSheetLoad("romfs:/gfx/MainMenuButton.t3x");
    C2D_SpriteFromSheet(&ButtonSpr, ButtonSheet, 1);

    C2D_SpriteSetScale(&ButtonSpr, 0.8f, 0.8f); 
    C2D_SpriteSetCenter(&ButtonSpr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&ButtonSpr, 160.0f, 200.0f); 

    SaveArrowSheet = C2D_SpriteSheetLoad("romfs:/gfx/SaveArrow.t3x");
    C2D_SpriteFromSheet(&SaveArrowSpr, SaveArrowSheet, 0);

    C2D_SpriteSetScale(&SaveArrowSpr, 1.0f, 1.0f); 
    C2D_SpriteSetCenter(&SaveArrowSpr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&SaveArrowSpr, 160.0f, 30.0f);
    startY = 30.0f;

    SaveChestSheet = C2D_SpriteSheetLoad("romfs:/gfx/SaveChest.t3x");
    C2D_SpriteFromSheet(&SaveChestSpr, SaveChestSheet, 0);

    C2D_SpriteSetScale(&SaveChestSpr, 1.0f, 1.0f); 
    C2D_SpriteSetCenter(&SaveChestSpr, 0.5f, 0.5f);
    C2D_SpriteSetPos(&SaveChestSpr, 160.0f, 60.0f);


    dynamicBuf = C2D_TextBufNew(500);
    myMojangFont = C2D_FontLoad("romfs:/Media/font/Mojang_Font_7.bcfnt");
    myDFGFont = C2D_FontLoad("romfs:/Media/font/DFGMaruGothic-Md.bcfnt");


    cfguInit();
    u8 lang;
    CFGU_GetSystemLanguage(&lang);
    cfguExit();

    if (lang == CFG_LANGUAGE_JP) {
        // 丸ゴシックで日本語
        C2D_TextFontParse(&autosavetext, myDFGFont, dynamicBuf, 
            "このゲームにはオートセーブ機能があります。\nデータのセーブ中には上記のアイコンが\n表示されます。\n\nこのアイコンが表示されてる間は、\nNintendo 3DS本体の電源を切らないでください。");
    } else {
        // Mojangフォント
        C2D_TextFontParse(&autosavetext, myMojangFont, dynamicBuf, 
            "This game has a level autosave feature.\nWhen you see the icon above displayed,\nthe game is saving your data.\n\nPlease do not turn off your Nintendo 3DS\nconsole while this icon is on-screen.");
    }
    C2D_TextFontParse(&oktext, myMojangFont, dynamicBuf, "OK");

    finished = false;
}


void AutoSaveScreen::update()
{
    if (hidKeysDown() & KEY_A)
    {
        sound.playPress();
        finished = true;
    }

    const float speed = 0.375f; 
    const float range = 7.0f; 

    static float offset = 0.0f; 
    static int direction = 1; 

    offset += speed * direction;

    if (offset >= range) {
        offset = range;   
        direction = -1;   
    } else if (offset <= 0.0f) {
        offset = 0.0f;   
        direction = 1;  
    }
    C2D_SpriteSetPos(&SaveArrowSpr, 160.0f, startY + offset);
}

void AutoSaveScreen::draw(C3D_RenderTarget* bottom)
{
    C2D_SceneBegin(bottom);

    if (panelSheet)
{
    C2D_DrawSprite(&panelSpr);

}
    if (ButtonSheet)
{
    C2D_DrawSprite(&ButtonSpr);

}


    if (SaveChestSheet)
{
    C2D_DrawSprite(&SaveChestSpr);

}
    if (SaveArrowSheet)
{
    C2D_DrawSprite(&SaveArrowSpr);

}
    /* Deleted
    cfguInit();
    u8 lang;
    CFGU_GetSystemLanguage(&lang);
    cfguExit();

    float scale = (lang == CFG_LANGUAGE_JP) ? 0.4f : 0.5f; // 日本語少し小さく
    */
    

    C2D_DrawText(&autosavetext, C2D_AlignLeft | C2D_WithColor, 20.0f, 80.0f, 0.0f, 0.5f, 0.5f, C2D_Color32(51, 51, 51, 255));

    C2D_DrawText(&oktext, C2D_AlignCenter | C2D_WithColor, 161.0f, 190.0f, 0.0f, 0.7f, 0.7f, C2D_Color32(23, 24, 35, 255));
    C2D_DrawText(&oktext, C2D_AlignCenter | C2D_WithColor, 160.0f, 189.0f, 0.0f, 0.7f, 0.7f, C2D_Color32(255, 255, 0, 255));
    
}



bool AutoSaveScreen::isFinished() const
{
    return finished;
}

void AutoSaveScreen::exit()
{
    C2D_TextBufDelete(dynamicBuf);
    C2D_FontFree(myMojangFont);
    C2D_FontFree(myDFGFont);
    C2D_SpriteSheetFree(panelSheet);
    C2D_SpriteSheetFree(ButtonSheet);
    C2D_SpriteSheetFree(SaveChestSheet);
    C2D_SpriteSheetFree(SaveArrowSheet);
}