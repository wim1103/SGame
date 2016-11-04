## Practical tutorial for making a UE4 2D Game
Tencent is pleased to support the open source community by making this tutorial available. 

Copyright (C) 2015 THL A29 Limited, a Tencent company, and Reiwang. All rights reserved.

## Quick introduction
It is a UE4 made [Dungeon Raid](https://play.google.com/store/apps/details?id=com.fireflame.dungeonraid&hl=zh_CN) like game. The following picture may give you a first impression.

![Gameplay Image](/ArtSource/Reference/Gameplay.png)

If you want to further know about the game, you can see the gameplay video from the youtube [link.](https://youtu.be/VPCGhkHxyRQ)

## Still under development
The game is still under development, that is to say, serveral part of the game, including

1. Levelup part (Coin shop, XP full upgrade, Armor full upgrade) is not finished yet.
2. Majority kinds of the skills and the monsters is still under developement, because of the lacking of art.

## Codebase feature
1. Use UE4 [Unreal Message Bus](https://docs.unrealengine.com/latest/INT/API/Runtime/Messaging/IMessageBus/index.html) to process the game messages.
2. Use [iTween](https://wiki.unrealengine.com/Itween) for UE4 to animate the sprites.
3. Very well commented code.

## Analyze - Design - Implement
Apart from this codebase, there is a very detailed series video tutorial on how to analyze, design and implement the game. But it is in Chinese, because originally this tutorial is conducted by [Tencent GAD program](http://gad.qq.com/content/coursedetail/7168318). Following is some analyze and design pictures from the video tutorial:
![Class Diagram 1](/ArtSource/Reference/ClassD_1.png)
![Sequence Diagram 1](/ArtSource/Reference/SequenceD_1.png)