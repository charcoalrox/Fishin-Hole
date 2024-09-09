c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -c -o main.o main.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbm_bythesea.o bgmdat\gbm_bythesea.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o bgm_bubblemachine.o bgmdat\bgm_bubblemachine.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf-ba0 -Wl-y -Wl-w -c -o savestate.o LocLibs\savestate.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player.o bgmdat\gbt_player.s
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o gbt_player_bank1.o bgmdat\gbt_player_bank1.s
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o MapTileSet.o ImgCode\MapTileSet.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o Background.o ImgCode\BackgroundData\Background.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o BG_StartMenu.o ImgCode\BackgroundData\BG_StartMenu.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o BG_HighScores.o ImgCode\BackgroundData\BG_HighScores.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wf--debug -Wl-y -Wl-w -c -o BG_NewHighScore.o ImgCode\BackgroundData\BG_NewHighScore.c
c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -c -o GameSprites.o ImgCode\GameSprites.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o sfx.o LocLibs\sfx.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o ControlFuncs.o LocLibs\ControlFuncs.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o SystemCommands.o LocLibs\SystemCommands.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o HudElements.o LocLibs\HudElements.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o PlayerControlFuncs.o LocLibs\PlayerControlFuncs.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o LevelSetupFuncs.o LocLibs\LevelSetupFuncs.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o HighScoreScreen.o LocLibs\HighScoreScreen.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o MusicFunc.o LocLibs\MusicFunc.c
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o basketball.o Levels\basketball.c

c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wl-yp0x143=0x80 -Wl-yt0x1B -Wl-yo4 -Wl-ya4 -o FishinHole.gb ^
main.o gbm_bythesea.o bgm_bubblemachine.o savestate.o gbt_player.o gbt_player_bank1.o MapTileSet.o Background.o BG_StartMenu.o BG_HighScores.o BG_NewHighScore.o GameSprites.o ^
sfx.o ControlFuncs.o SystemCommands.o HudElements.o PlayerControlFuncs.o MusicFunc.o basketball.o LevelSetupFuncs.o HighScoreScreen.o 


c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -c -o mainClean.o mainClean.c

c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -Wl-yp0x143=0x80 -Wl-yt0x1B -Wl-yo4 -Wl-ya4 -o FishinHole(NoBlueFlashes).gb ^
mainClean.o gbm_bythesea.o bgm_bubblemachine.o savestate.o gbt_player.o gbt_player_bank1.o MapTileSet.o Background.o BG_StartMenu.o BG_HighScores.o BG_NewHighScore.o GameSprites.o ^
sfx.o ControlFuncs.o SystemCommands.o HudElements.o PlayerControlFuncs.o MusicFunc.o basketball.o LevelSetupFuncs.o HighScoreScreen.o 


del *.ihx *.noi *.asm *.lst *.o *.sym *.cdb *.adb