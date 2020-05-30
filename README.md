# Chess app rough cut

## Hosted live
playchessnstuff.com

## Technical/motivation overview
This started with me wanting to implement a more classical type of machine learning after implementing a convolutional neural network in my previous project. The potential for a UI with a chess AI was ideal. I started writing chess logic in C++ to power the AI without much thought since that was what I had been writing in. I figured I would make a native interface. As I was finishing up the chess logic, it occurred to me that I could make a browser based interface for a C++ game/AI without much trouble (ha). React seemed like a good choice for the UI based on my limited experience with it. As I started to style it and make sure it worked on a few different browsers so I could show people, it occurred to me that I had a browser based two player game that could only be played against an AI or locally. In 2020. So I set out to add multiplayer. Of the few backend technologies I had worked with, Django was my favorite and it seemed intuitive for this purpose. I also knew that wrapping python around C++ code was a well worn path and could be used to validate the moves on the back end (that wrapping actually did turn out pretty painless). This is a rough, but functional cut.

*the readme will get better and more organized in each update, hopefully including instructions to build locally eventually