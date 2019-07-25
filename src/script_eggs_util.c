#include "global.h"
#include "item.h"
#include "script.h"
#include "strings.h"
#include "menu.h"
#include "menu_helpers.h"
#include "sound.h"
#include "constants/songs.h"
#include "main.h"
#include "constants/items.h"
#include "item_menu.h"
#include "event_data.h"
#include "string_util.h"

extern void (*gFieldCallback)(void);

void HasEggs(void)
{
    gSpecialVar_Result = 0;

    if(CheckBagHasEggItem()) {
        gSpecialVar_Result = 1;
    }
}

void EggCost(void)
{
    u8 type = gSpecialVar_0x8001 % 4;

    switch(type) {
        case 0: gSpecialVar_Result = 50; break;
        case 1: gSpecialVar_Result = 200; break;
        case 2: gSpecialVar_Result = 800; break;
        case 3: gSpecialVar_Result = 3200; break;
        default: gSpecialVar_Result = 10000; break;
    }

    ConvertIntToDecimalString(gStringVar1, gSpecialVar_Result);
    //ConvertIntToDecimalString(gStringVar1, gSpecialVar_0x8001);
    StringCopy10(gStringVar2, ItemId_GetName(ITEM_EGG_START + gSpecialVar_0x8001));
}

#define EGG_MENU_SIZE 5

EWRAM_DATA u8 numItems = 0;
EWRAM_DATA u8 cursorPos = 0;
EWRAM_DATA u8 flags = 0;

EWRAM_DATA u8 const* eggMenu[17*4 + 1];   //array of strings
EWRAM_DATA u8 eggMenuType[17*4]; //array of types

void destroyMenu(void)
{
    if ((flags & 1) != 0)
    {
        DestroyVerticalScrollIndicator(BOTTOM_ARROW);
    }
    if ((flags >> 1) == 1)
    {
        DestroyVerticalScrollIndicator(TOP_ARROW);
    }
    BuyMenuFreeMemory();
}

void cleanupTask(u8 taskId)
{
    DestroyTask(taskId);
    EnableBothScriptContexts();
}

void taskHandleMenu(u8);
bool8 handleUpDown(u8, u8);
void eggMenuUpdateScrollIndicators(u8);

void ShowEggMenu(void)
{
    u8 i;
    ScriptContext2_Enable();

    numItems = 0;

    for(i = 0; i < NUM_LOOT_EGGS; i++)
    {
        if(CheckBagHasItem(ITEM_EGG_START + i, 1))
        {
            eggMenuType[numItems] = i;
            eggMenu[numItems] = ItemId_GetName(ITEM_EGG_START + i);
            numItems ++;
        }
    }

    eggMenu[numItems] = gOtherText_CancelNoTerminator;
    numItems ++;

    Menu_DrawStdWindowFrame(0, 0, 10, 11);
    InitMenu(0, 1, 1, EGG_MENU_SIZE, 0, 9);
    flags = 0;
    ClearVerticalScrollIndicatorPalettes();
    LoadScrollIndicatorPalette();
    eggMenuUpdateScrollIndicators(0);

    for (i=0; i<(numItems < EGG_MENU_SIZE ? numItems : EGG_MENU_SIZE); i++)
    {
        Menu_PrintText(eggMenu[i], 1, 2 * i + 1);
    }
    cursorPos = 0;
    CreateTask(taskHandleMenu, 8);
}

void taskHandleMenu(u8 taskId)
{
    u8 prevCursorPos;
    if (gMain.newKeys == DPAD_UP && cursorPos != 0)
    {
        cursorPos--;
        prevCursorPos = Menu_GetCursorPos();
        Menu_MoveCursorNoWrap(-1);
        handleUpDown(prevCursorPos, DPAD_UP);
    }
    if (gMain.newKeys == DPAD_DOWN && cursorPos != numItems - 1)
    {
        cursorPos++;
        prevCursorPos = Menu_GetCursorPos();
        Menu_MoveCursorNoWrap(1);
        handleUpDown(prevCursorPos, DPAD_DOWN);
    }
    if (gMain.newKeys & A_BUTTON)
    {
        Menu_DestroyCursor();
        if(cursorPos == numItems - 1) {
            gSpecialVar_0x8001 = 0x7f;
        } else {
            gSpecialVar_0x8001 = eggMenuType[cursorPos];
        }
        PlaySE(SE_SELECT);
        destroyMenu();
        Menu_EraseWindowRect(0, 0, 29, 12);
        cleanupTask(taskId);
    }
    if (gMain.newKeys & B_BUTTON)
    {
        Menu_DestroyCursor();
        gSpecialVar_0x8001 = 0x7f;
        PlaySE(SE_SELECT);
        destroyMenu();
        Menu_EraseWindowRect(0, 0, 29, 12);
        cleanupTask(taskId);
    }
}

/* Removing the NONMATCHING block will swap the roles of r4 and r5 throughout.
Could possibly be fixed by writing code which increases the amount of references to newPos,
or decreasing the amount of references to i.*/
bool8 handleUpDown(u8 prevCursorPos, u8 dpadInput)
{
    u8 i;
    u8 flag = 0;
    u8 newPos = 0;
    if (numItems < EGG_MENU_SIZE)
    {
        return FALSE;
    }
    if (dpadInput == DPAD_UP)
    {
        if (prevCursorPos == 0)
        {
            newPos = cursorPos;
            flag = TRUE;
        }
    }
    else if (dpadInput == DPAD_DOWN)
    {
        if (prevCursorPos == 4)
        {
            newPos = cursorPos - 4;
            flag = TRUE;
        }
    }
    if (flag)
    {
        eggMenuUpdateScrollIndicators(newPos);
        Menu_BlankWindowRect(2, 1, 9, 10);
        for (i=0; i<EGG_MENU_SIZE; newPos++, i++)
        {
            Menu_PrintText(eggMenu[newPos], 1, 2 * i + 1);
        }
    }
    return flag;
}

void eggMenuUpdateScrollIndicators(u8 newPos)
{
    if (newPos == 0 && (flags & 0x02) != 0)
    {
        flags ^= 0x02;
        DestroyVerticalScrollIndicator(TOP_ARROW);
    }
    else if(newPos > 0 && (flags & 0x02) == 0)
    {
        flags |= 0x02;
        CreateVerticalScrollIndicators(TOP_ARROW, 0x2c, 0x08);
    }

    if (newPos + EGG_MENU_SIZE >= numItems && (flags & 0x01) != 0)
    {
        flags ^= 0x01;
        DestroyVerticalScrollIndicator(BOTTOM_ARROW);
    }
    else if (newPos + EGG_MENU_SIZE < numItems && (flags & 0x01) == 0)
    {
        flags |= 0x01;
        CreateVerticalScrollIndicators(BOTTOM_ARROW, 0x2c, 0x58);
    }
    
}
