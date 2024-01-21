# NKFrameworkDecomp
A decompilation of NinjaKiwi's C++ game framework. <h6>sadly with seemingly no official title other than "NewFramework" (boring!)</h6>

## Sources
Most of the code is taken from macOS builds of SAS: Zombie Assault 4, which include most of the game's symbols. Otherwise, the Windows builds are used.

There are no plans to do anything with Android and iOS builds as working with them is much more difficult for not much return, though be my guest if you want to do that and hopefully contribute back 😃

## Philosophy
The primary philosophy of this decompilation is to take as few liberties with the code and file structure as possible, even when something is far from optimal.

Some ground rules:
- Files whose locations are unknown are put into the "Uncategorized" folder unless if a very probable guess to their locations can be made.
- Liberties can be taken within reason for operations with C++ data types that disassemblers do not work well with (e.g. std::map) and data types that are mostly inlined by the compiler (e.g. most interfaces).
