# NFS Heat Hacks

In this directory you will find my trainer project for Need for Speed Heat. It's very WIP.
Cheat Engine is required to run this for now. Use CE to open "NFS Heat Trainer.CT" and hook it to Heat.
Also contains various resources, and a batch file to run Heat offline in one step.

----------------------------
++++++POI/Descriptions++++++
----------------------------

1: NFS Heat Trainer.CT

   This is the cheat table for the trainer. It has somewhat limited functionality.
   If you view the file as text, you can see the auto-assemble and LUA scripts within.
   
   What it does for now: Infinite health - locks health address value at 100
                         Disable Busting - will enable built in "Disable Busting" feature
                         Unlock All Cars - enables built in "Unlock All Cars" feature
                         Infinite NOS (WIP) - has a mess of pointers and auto assemble scripts
                                              to give infinite boost, 50% working.
   
   To do: Create scripts for part swaps - you can swap parts in the game for some hidden parts,
                                          as well as swap to parts that aren't allowed. I aim to 
                                          create some scripts to automate certain swaps. Holding off
                                          until Frosty editor is released though.
                                          
2: Disable Busting.LUA
   
   This small script will perform an array of bytes search within Cheat Engine, and perform a swap. 
   You can edit the file to add any AOB you wish to swap. 
   
   In this example, we are trying to change
   the memory containing "Gameplay/Tweakables/Chase_DisableBusting". We wish to change this from "false"
   to "true". So you can simply input the bytes for the false statement for 'pattern', and then put
   the true statement bytes into 'replace'. It will save the original bytes, and replace them with
   the new ones. It also provides enable/disable functionaltiy for CE, which sometimes doesn't work :(
   
3: Origin Offline.bat

   This is a simple Batch file I wrote to play NFS Heat in offline mode in one easy step. EA doesn't
   allow online modding, and will reset your money to 0 and ruin your inventory if you try to go online
   with any mods. 
   
   Luckily Origin has a built-in offline mode. This file will temporarily disable your Internet adapter
   and subsequently runs NFS Heat, which forces Origin into offline mode. It then waits 30 secs, and prompts
   you to press any key to enable the adapter. This way if your game crashes you can easily restart, and in general
   allows you to play offline in one click. Easy!
   
4: Resources

   This directory is full of ID dumps for NFS Heat. I compiled my most-used ID into "Favorite Heat Things". This includes
   Suspenion IDs, Engine IDs, all Performance IDs, and various other tweaks. This list was compiled from the other dumps
   (not my dumps, credits to respective authors) found in this folder.

5: NFS Heat Save Backup
   
   This contains my savegame backups. Legit contains my original un-modded backup for online play. Modded contains
   my modded save with $1.5 billion, plus many hacked cars with modded engine/suspension swaps and black parts/cop NOS.
   
6: Pointer Scans

   These are pointer scan files that I use with Cheat Engine. These basically allow you to track down addresses within a
   game. For example, once you find the address that holds the Health value, you can pointer scan for all addresses containing
   the value. You then repeat this process to narrow down the results. After many scans, you will find a few pointers that will
   always point to the same address even across game restarts. That way, you don't always have to search for a value, you can
   just lock the pointer.
