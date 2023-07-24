# Windows-Game-Engine
It is a simple game engine for simple graphical games. You can create your own game using this engine.
How to use:
1) Download project;
2) Open file WinGame.cpp;
3) You need to add your game information, using 4 methods:

void Init() - init game data;
void Act(float dt) -update game data, dt - time elapsed since the previous update (seconds);
void Draw() - filling buffer(screen of your game);
void Finish() - game results, free game data.
