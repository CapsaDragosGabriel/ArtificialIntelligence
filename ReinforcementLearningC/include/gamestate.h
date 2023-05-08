#ifndef GAMESTATE_H
#define GAMESTATE_H


class gamestate
{
    public:
        gamestate();
        int teren[4][12];
        int rewards[4][12];
        virtual ~gamestate();
        printRewards();

};

#endif // GAMESTATE_H
