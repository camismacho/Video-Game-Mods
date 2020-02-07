----GTA V Pure Black Window Tint----
Written by camismacho

PLEASE BACK UP YOUR FILES BEFORE INSTALLING THIS!!!
PLEASE READ ALL INSTRUCTIONs FOR MOD COMPATIBILITY!!!


***EASY INSTALLATION*** (Use if your carcols.ymt is unmodified)

-I have included 4 ways to install this mod. You can choose to replace whichever tint in LS Customs you use the least, or choose the 4th option.
	
	1: Light Smoke Replace - Replaces light smoke tint with pure black
	2: Dark Smoke Replace - Replaces dark smoke tint with pure black
	3: Limo Replace - Replaces limo tint with pure black
	4: Trainer - You can only access this option with a trainer (I use Menyoo, haven't tested other trainers. For Menyoo, it's the "Stock" tint option in Menyoo Customs.) This means you'll keep light, dark, and limo tints in LSC, and can only apply pure black through the use of a trainer.

Choose whichever option suits you best and drop carcols.ymt using OpenIV at:
	mods\update\update.rpf\x64\data

Enjoy the return of Pure Black Tint!



***MANUAL/MOD INSTALLATION***

-Some mods already provide you with a modified carcols.ymt file. The files I provided are for an unmodified game.
 If this is your case, you can still have Black Tint, you just have to modify the already modified carcols.ymt yourself.

1: Using OpenIV, navigate to mods\update\update.rpf\x64\data.
2: Select carcols.ymt and extract it. (If the file is still encypted, right-click the file and select Edit. Then select Export and export it as a txt file. Use Notepad to open the file, the Save As all files and add the .ymt extension.)
3: Open using your Notepad or your favorite text editor (you can also do this in OpenIV if it lets you, my file doesn't let me)
4: Ctrl+F to search.
5: Type "Window Colors" and search.
6: Scroll down and you will see 5 window options, you should edit any except "standard windows"
7: Choose the corresponding color you wish to replace, and paste this line:
	
<color value="D6000000:/>

   ***For example, if you want to replace Limo, then your file should look like:
	<Item>
          <color value="0xD6000000"/>
          <name>Limo Black</name>
        </Item>***

(Note: the <name> 4 </name> option is the trainer option I described above. Use this one if you want to keep all other tints.)

8: Save the file and drop it back at mods\update\update.rpf\x64\data.

That's it! You keep your already modded carcols.ymt, and you have black tint!


***EXTRA INFO***
1: This will not adversely affect cars that are already tinted in-game (i.e. Stretch, Rancher)
2: You can use the Pure Black value "D6000000" in vehicles.meta if you'd like cars to spawn in-game with Black Tint, or any tint for that matter.
3: This will fully black out the windows of modded cars if they're not mapped right, so you may get a solid black windshield and lights.

If someone knows how to add an extra tint option in LS Customs so we can truly have Green and Pure Black tint, feel free to contact me.








