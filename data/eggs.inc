
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
	special SampleRoll
	msgbox gText_EggGuy_Sample, 4
    return

egg_none::
	msgbox gText_EggGuy_NoEggs, 4
	releaseall
	end

egg_cancel::
	msgbox gText_EggGuy_Cancel, 4
	releaseall
	end

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
