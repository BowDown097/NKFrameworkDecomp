# NKFrameworkDecomp
A decompilation of NinjaKiwi's C++ game framework. <h6>sadly with seemingly no official title other than "NewFramework" (boring!)</h6>

## Sources
Most of the code is taken from macOS builds of SAS: Zombie Assault 4, which include most of the game's symbols. Otherwise, the Windows builds are used.

There are no plans to do anything with Android and iOS builds as working with them is much more difficult for not much return, though be my guest if you want to do that and hopefully contribute back 😃

## Philosophy
The primary philosophy of this decompilation is to take as few liberties with the code and file structure as possible, even when something is far from optimal.

Some ground rules:
- Files whose locations are unknown are put into the "Uncategorized" folder unless if a very probable guess to their locations can be made.
- The [style guide](STYLE_GUIDE.md) should always be followed.
