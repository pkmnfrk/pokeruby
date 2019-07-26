
global_EventScript_EggGuy::
	lock
	faceplayer
	message gText_EggGuy_Intro
    waitmessage
	special HasEggs
	compare RESULT, 0
	goto_if_eq egg_none
    message gText_EggGuy_HasEggs
	waitmessage
	special ShowEggMenu
	waitstate
	compare VAR_SPECIAL_1, 0x7f
	goto_if_eq egg_cancel

	special EggCost @ also loads into STR_VAR_1
	msgbox gText_EggGuy_Cost, MSGBOX_YESNO
	compare RESULT, NO
	goto_if_eq egg_cancel

	@@ Requiring a save is a neat idea in theory
	@@ however, for it to mean anything, it would have to
	@@ happen before the players sees what they get. But,
	@@ I don't have an easy way of resuming this process
	@@ if it is interrupted after a save. So, let's just
	@@ leave it alone for now.
	@call_if_unset FLAG_TEMP_1, egg_mustSave
	@clearflag FLAG_TEMP_1
	@call S_DoSaveDialog
	@setflag FLAG_TEMP_1
	@compare RESULT, NO
	@goto_if_eq egg_cancel

	special SampleRoll
	compare VAR_SPECIAL_1, 1
	call_if_eq egg_getItem
	
    return

egg_none::
	msgbox gText_EggGuy_NoEggs, 4
	releaseall
	end

egg_cancel::
	msgbox gText_EggGuy_Cancel, 4
	releaseall
	end

egg_mustSave::
	msgbox gText_EggGuy_Save, 4
	setflag FLAG_TEMP_1
	return

egg_getItem::
	msgbox gText_EggGuy_GotItem, 4
	call Std_ObtainItem
	return

gText_EggGuy_Intro::
	.string "Hi there! I'm the EGG guy!\p"
	.string "If you have any POKéMON eggs, I can\n"
	.string "help you hatch them, for a nominal fee.\p$"

gText_EggGuy_NoEggs::
	.string "Come back with some eggs!$"

gText_EggGuy_Cancel::
	.string "Come back to hatch some eggs!$"

gText_EggGuy_HasEggs::
	.string "Which egg would you like to hatch?$"

gText_EggGuy_Cost::
	.string "You want to hatch this {STR_VAR_2}\n"
	.string "egg? That will cost ¥{STR_VAR_1}.$"

gText_EggGuy_Sample::
	.string "Let's see... Rolled a {STR_VAR_1}!\n"
	.string "That means you get a {STR_VAR_2}!$"

gText_EggGuy_Save::
	.string "Oh, by the way, every time you hatch\n"
	.string "an egg, I have to save the game.\p"
	.string "Just so you know!$"

gText_EggGuy_Error::
	.string "Whoops, it seems like there was an\n"
	.string "error of some kind. Sorry!$"

gText_EggGuy_GotItem::
	.string "Oh look, there was an item inside!$"

