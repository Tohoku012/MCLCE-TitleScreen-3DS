#include <cmath>
#include <3ds.h>
#include "Sound.h"
#include <3ds/services/cfgu.h>
#include "MainMenuScreen.h"
#include <citro2d.h>
extern Sound sound;

void MainMenuScreen::init()
{

    ButtonSheet = C2D_SpriteSheetLoad("romfs:/gfx/MainMenuButton.t3x");

    C2D_SpriteFromSheet(&ButtonSpr, ButtonSheet, 0);
    C2D_SpriteSetScale(&ButtonSpr, 0.8f, 0.8f); 
    C2D_SpriteSetCenter(&ButtonSpr, 0.5f, 0.5f);

    C2D_SpriteFromSheet(&ButtonOverSpr, ButtonSheet, 1);
    C2D_SpriteSetScale(&ButtonOverSpr, 0.8f, 0.8f); 
    C2D_SpriteSetCenter(&ButtonOverSpr, 0.5f, 0.5f);


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
        //C2D_TextFontParse(&autosavetext, myDFGFont, dynamicBuf, "");
        C2D_TextFontParse(&PlayGametext, myDFGFont, dynamicBuf, "プレイする");
        C2D_TextFontParse(&Achievementstext, myDFGFont, dynamicBuf, "実績");
        C2D_TextFontParse(&HelpOptionstext, myDFGFont, dynamicBuf, "遊び方 & オプション");
        C2D_TextFontParse(&Storetext, myDFGFont, dynamicBuf, "Minecraft ストア");
    } else {
        // Mojangフォント
        C2D_TextFontParse(&PlayGametext, myMojangFont, dynamicBuf, "Play Game");
        C2D_TextFontParse(&Achievementstext, myMojangFont, dynamicBuf, "Achievements");
        C2D_TextFontParse(&HelpOptionstext, myMojangFont, dynamicBuf, "Help & Options");
        C2D_TextFontParse(&Storetext, myMojangFont, dynamicBuf, "Minecraft Store");
    }
    
    C2D_TextFontParse(&splashtext, myMojangFont, dynamicBuf, "Minecraft!");

    finished = false;
}


void MainMenuScreen::update()
{
    float time = (float)(osGetTime() % 1000) / 1000.0f;
    
    float sss = 1.8f - fabsf(sinf(time * 3.14159f * 2.0f) * 0.1f);
    
    textScale = sss * 0.5f;


static u64 lastPressTime = 0;   
    static bool isFirstHold = true; 
    
    u32 kHeld = hidKeysHeld();
    u32 kDown = hidKeysDown();

    if (!(kHeld & (KEY_UP | KEY_DOWN))) {
        lastPressTime = 0;
        isFirstHold = true;
        return;
    }

    u64 currentTime = osGetTime();
    bool canMove = false;

    if (kDown & (KEY_UP | KEY_DOWN)) {
        canMove = true;
        lastPressTime = currentTime;
        isFirstHold = true;
    } else if (lastPressTime != 0) {
        u64 threshold = isFirstHold ? 300 : 100; 
        if (currentTime - lastPressTime >= threshold) {
            canMove = true;
            lastPressTime = currentTime;
            isFirstHold = false; 
        }
    }

    if (canMove) {
        if (kHeld & KEY_UP) {
            selectedIndex--;
            if (selectedIndex < 0) selectedIndex = 3;
            //sound.playFocus();
        } else if (kHeld & KEY_DOWN) {
            selectedIndex++;
            if (selectedIndex > 3) selectedIndex = 0;
            //sound.playFocus();
        }
    }

}

void MainMenuScreen::draw(C3D_RenderTarget* top, C3D_RenderTarget* bottom)
{
    C2D_SceneBegin(top);
    
    float outW, outH;
    C2D_TextGetDimensions(&splashtext, textScale, textScale, &outW, &outH);

    float baseY = 180.0f;
    
    float drawY = baseY - (outH / 2.0f);

    // baseY - outH

    C2D_DrawText(&splashtext, C2D_AlignCenter | C2D_WithColor, 202.0f, drawY + 2.0f, 0.0f, textScale, textScale, C2D_Color32(72, 72, 0, 255));
    C2D_DrawText(&splashtext, C2D_AlignCenter | C2D_WithColor, 200.0f, drawY, 0.0f, textScale, textScale, C2D_Color32(255, 255, 0, 255));




    C2D_SceneBegin(bottom);
    
    // 描画ループ
    float yPos[] = {30.0f, 66.0f, 102.0f, 138.0f};
    C2D_Text* texts[] = {&PlayGametext, &Achievementstext, &HelpOptionstext, &Storetext};

    for(int i = 0; i < 4; i++) {
  
    C2D_Sprite* currentSpr = (i == selectedIndex) ? &ButtonOverSpr : &ButtonSpr;
    
    C2D_SpriteSetPos(currentSpr, 160.0f, yPos[i]); 
    C2D_DrawSprite(currentSpr);
        
        float scale = isJapanese ? 0.6f : 0.7f;
        float texty = isJapanese ? 1.5f : 0.0f;

        u32 textColor = (i == selectedIndex) ? C2D_Color32(255, 255, 0, 255) : C2D_Color32(255, 255, 255, 255);
        u32 textsdColor = (i == selectedIndex) ? C2D_Color32(23, 24, 35, 255) : C2D_Color32(0, 0, 0, 255);
        
        C2D_DrawText(texts[i], C2D_AlignCenter | C2D_WithColor, 161.0f, yPos[i] - 10.0f + texty, 0.0f, scale, scale, textsdColor);
        C2D_DrawText(texts[i], C2D_AlignCenter | C2D_WithColor, 160.0f, yPos[i] - 11.0f + texty, 0.0f, scale, scale, textColor);
    }


}

bool MainMenuScreen::isFinished() const
{
    return finished;
}

void MainMenuScreen::exit() {
    C2D_TextBufDelete(dynamicBuf);
    C2D_FontFree(myMojangFont);
    C2D_FontFree(myDFGFont);
    C2D_SpriteSheetFree(ButtonSheet);
}