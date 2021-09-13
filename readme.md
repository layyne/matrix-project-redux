# C matrix project - Redux
This is a for-fun revamp of an old project I did for a C programming class. It's a small library
for various simple matrix operations, with my attempt at error handling, error codes, and
helper/internal functions.

I used this to try implementing error reporting/handling in a language that doesn't inherently support
multiple return values, which in this case involved passing the proper "return value" address through
function parameters and simply returning an integer that corresponds to a certain error code.