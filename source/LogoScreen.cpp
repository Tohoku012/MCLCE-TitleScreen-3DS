#include "LogoScreen.h"
#include <3ds.h>
#include <cmath>

void LogoScreen::init()
{

    fadeAlpha = 255;

    timer = 0.0f;
    stage = 0;
    alpha = 0.0f;

    mojangSheet = C2D_SpriteSheetLoad("romfs:/gfx/MOJANGLOGO.t3x");
    fourJSheet  = C2D_SpriteSheetLoad("romfs:/gfx/4JLOGO.t3x");

    C2D_SpriteFromSheet(&mojangSpr, mojangSheet, 0);
    C2D_SpriteFromSheet(&fourJSpr, fourJSheet, 0);

C2D_SpriteSetScale(&mojangSpr, 0.6f, 0.6f);
C2D_SpriteSetScale(&fourJSpr, 0.6f, 0.6f);
    
C2D_SpriteSetCenter(&mojangSpr, 0.5f, 0.5f);
C2D_SpriteSetCenter(&fourJSpr, 0.5f, 0.5f);

C2D_SpriteSetPos(&mojangSpr, 200.0f, 120.0f);
C2D_SpriteSetPos(&fourJSpr, 200.0f, 120.0f);

finished = false;
}

void LogoScreen::update()
{
    if (hidKeysDown() & KEY_A)
    {
        finished = true;
    }

    timer += 0.016f;

    const float fadeTime = 0.4f;
    const float logoTime = 3.0f;

    // alpha計算
    if (timer < fadeTime)
    {
        alpha = timer / fadeTime;
    }
    else if (timer > logoTime - fadeTime)
    {
        alpha = (logoTime - timer) / fadeTime;
    }
    else
    {
        alpha = 1.0f;
    }

    if (alpha < 0.0f) alpha = 0.0f;
    if (alpha > 1.0f) alpha = 1.0f;

    if (timer >= logoTime)
    {
        timer = 0.0f;
        stage++;

        if (stage > 2)
            stage = 2;
    }


}

void LogoScreen::draw(C3D_RenderTarget* top)
{
    C2D_TargetClear(top, C2D_Color32(255,255,255,255));
    C2D_SceneBegin(top);

    C2D_Sprite* spr = nullptr;

    if (stage == 0)
        spr = &mojangSpr;
    else if (stage == 1)
        spr = &fourJSpr;

if (spr)
{
    C2D_DrawSprite(spr);

    u8 fadeAlpha = (u8)((1.0f - alpha) * 255.0f);

    C2D_DrawRectSolid(
        0.0f,
        0.0f,
        0.5f,
        400.0f,
        240.0f,
        C2D_Color32(
            255,
            255,
            255,
            fadeAlpha
        )
    );
}
}

bool LogoScreen::isFinished() const
{
   return (stage >= 2) || finished;
}

void LogoScreen::exit()
{

    C2D_SpriteSheetFree(mojangSheet);
    C2D_SpriteSheetFree(fourJSheet);

}