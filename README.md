spellchecker
============

Answer to twitch [spellchecker](http://www.twitch.tv/problems/spellcheck) problem

Compilation
===========

Just copy/paste this line:

```terminal
mkdir -p build && cd build && cmake .. && make && cd ..
```

Tested with `gcc (Ubuntu/Linaro 4.7.2-11precise2) 4.7.2` on Linux Mint Maya 64-bit, but should word everywhere.. I think.

Running
=======

This program takes up to one argument: `--autocomplete`.
Turning this flag on will change the behaviour of the program. Instead of correcting spelling mistakes,
it will output a list of possible completion to the word you entered.

Example:
```terminal
> job
jobs
joblessness's
joblessness
jobless
jobbing
jobbers
jobber's
jobber
jobbed
job's
job
```

Here is an other example with the correction of spelling mistakes:

```terminal
> jjjooobbb
job
> CUNsperrICY
conspiracy
```
