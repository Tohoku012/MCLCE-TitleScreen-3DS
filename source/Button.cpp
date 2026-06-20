#include "Button.h"
#include <3ds.h>
#include <cmath>
#include <3ds/services/cfgu.h>
#include <citro2d.h>


void Button::init()
{
    buttonSheet = C2D_SpriteSheetLoad("romfs:/gfx/Button.t3x");
    graphicSheet = C2D_SpriteSheetLoad("romfs:/gfx/Button_graphic.t3x");

    C2D_Sprite* textSprs[] = {&aSpr, &bSpr, &xSpr, &ySpr};
    C2D_Sprite* colorSprs[] = {&RedSpr, &BlueSpr, &YelloSpr, &GreenSpr};

    for(int i = 0; i < 4; i++) {
    C2D_SpriteFromSheet(textSprs[i], graphicSheet, i); 
    C2D_SpriteSetScale(textSprs[i], 0.325f, 0.325f);
    C2D_SpriteSetCenter(textSprs[i], 0.5f, 0.5f);
    C2D_SpriteSetPos(textSprs[i], 10, 230);
    }

    for(int i = 0; i < 4; i++) {
    C2D_SpriteFromSheet(colorSprs[i], buttonSheet, i); 
    C2D_SpriteSetScale(colorSprs[i], 1.0f, 1.0f);
    C2D_SpriteSetCenter(colorSprs[i], 0.5f, 0.5f);
    }

    finished = false;

    buttons[0] = { KEY_A, &aSpr, 0.0f, 0 };
    buttons[1] = { KEY_B, &bSpr, 0.0f, 0 };
    buttons[2] = { KEY_X, &xSpr, 0.0f, 0 };
    buttons[3] = { KEY_Y, &ySpr, 0.0f, 0 };
    
    dynamicBuf = C2D_TextBufNew(500);
    myMojangFont = C2D_FontLoad("romfs:/Media/font/Mojang_Font_7.bcfnt");
    myDFGFont = C2D_FontLoad("romfs:/Media/font/DFGMaruGothic-Md.bcfnt");

    cfguInit();
    u8 lang;
    CFGU_GetSystemLanguage(&lang);
    cfguExit();
    isJapanese = (lang == CFG_LANGUAGE_JP); 
    
    if (isJapanese) {
        // 丸ゴシックフォント
        C2D_TextFontParse(&Selecttext, myDFGFont, dynamicBuf, "選択");
    } else {
        // Mojangフォント
        C2D_TextFontParse(&Selecttext, myMojangFont, dynamicBuf, "Select");
    }


}

void setSpriteY(C2D_Sprite* spr, float y) {
    C2D_SpriteSetPos(spr, spr->params.pos.x, y);
}

void Button::update()
{

    u32 kDown = hidKeysDown();
    u64 currentTime = osGetTime();

for (int i = 0; i < 4; i++) {
        if (kDown & buttons[i].key) {

            if (buttons[i].pressedTime == 0)
            buttons[i].originalY = buttons[i].sprite->params.pos.y;
            setSpriteY(buttons[i].sprite, buttons[i].originalY + 1.0f); 
            buttons[i].pressedTime = currentTime; 
        }

        if (buttons[i].pressedTime > 0 && (currentTime - buttons[i].pressedTime) >= 500) {
            setSpriteY(buttons[i].sprite, buttons[i].originalY); 
            buttons[i].pressedTime = 0; 
        }
    }
}

void Button::draw(C3D_RenderTarget* top, int state)
{
    C2D_SceneBegin(top);
    float scale = isJapanese ? 0.4f : 0.5f;
    switch(state) {
        case 0: // AutoSave

            C2D_SpriteSetPos(&aSpr, 10.0f, aSpr.params.pos.y);
            C2D_DrawSprite(&aSpr); 

            C2D_DrawText(&Selecttext, C2D_AlignCenter | C2D_WithColor, 161.0f, 201.0f, 0.0f, scale, scale, C2D_Color32(0, 0, 0, 255));
            C2D_DrawText(&Selecttext, C2D_AlignCenter | C2D_WithColor, 160.0f, 200.0f, 0.0f, scale, scale, C2D_Color32(255, 255, 255, 255));
            break;
        case 1: // MainMenu

            C2D_SpriteSetPos(&aSpr, 10.0f, aSpr.params.pos.y);
            C2D_DrawSprite(&aSpr); 

            break;
    }

}



bool Button::isFinished() const
{
    return finished;
}

void Button::exit()
{
    C2D_SpriteSheetFree(buttonSheet);
    C2D_SpriteSheetFree(graphicSheet);
}