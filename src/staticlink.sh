#!/bin/sh
g++-3.3 -g -Wall -o glob2 EndGameScreen.o AINumbi.o AI.o Building.o BuildingType.o Engine.o EntityType.o Fatal.o GUIMapPreview.o Game.o GameGUI.o GameGUIDialog.o GameGUILoadSave.o Glob2.o GlobalContainer.o Bullet.o Sector.o Map.o MapGenerator.o MapEdit.o NetGame.o Order.o Player.o SessionConnection.o Race.o Session.o SGSL.o Team.o TeamStat.o Unit.o UnitType.o Utilities.o CustomGameScreen.o LoadGameScreen.o MainMenuScreen.o MultiplayersOfferScreen.o YOGScreen.o Settings.o SettingsScreen.o MultiplayersHostScreen.o MultiplayersHost.o MultiplayersJoinScreen.o MultiplayersJoin.o MultiplayersConnectedScreen.o MultiplayersChooseMapScreen.o MultiplayersCrossConnectable.o NewMapScreen.o YOG.o MapGenerationDescriptor.o LogFileManager.o YOGPreScreen.o ScriptEditorScreen.o GUIGlob2FileList.o  -lSDL_image -lfreetype -lz -L/usr/lib -lSDL -lpthread ../libgag/src/libgag.a /usr/lib/libSDL.a /usr/lib/libSDL_image.a /usr/lib/libSDL_net.a /usr/lib/libSDL.a /usr/X11R6/lib/libX11.a /usr/lib/libpng*.a /usr/lib/libtiff.a /usr/X11R6/lib/libXext.a /usr/lib/libfreetype.a /usr/lib/libdl.a /usr/lib/libjpeg.a /usr/lib/libz.a -static
