# roll
A simple Diceroller in C for RPG and CLI enthusiasts
by João Victor Guimarães

## (UN)INSTALLING
* Clone the repo / download
* Run the install/uninstall scripts (Asuming `gcc` as compiler in the makefile)
    * You might need to add execution permissions: `chmod +x install.sh`
* The binary will be placed in /usr/local/bin/ as `roll`
* TODO: Actually make a neat release package... the scripts will do for now.

## USAGE
`roll [options] XdY[+A-B#Z] ...`
Where:

`X` is Number of Rolls

`Y` is the type of die

`A` and B are optional increments and decrements, respectively

`Z` is a Target Number. A roll will be successful if Result >= TN
It is possible to make several rolls and use several modifiers to each roll.

Please note that roll fails quietly - Incorrect input will simply be ignored. 

## OPTIONS
`-p`		Dice Pool mode: Each die of a roll is independent of the others

`-v`		Verbose mode

`-t<NUM>`	Default Target Number: All rolls will be compared against it.
		An explicit target number declaration will be used instead for that roll.
		
`-V`		Prints out the program's version\n"


## Examples and Features
### `roll xdy`
rolls a dy x times and sums their results
    
    $: roll 2d10 3d4 1d123
    [13] [6] [88]
    
### `roll -p xdy`     
rolls xdy as a dice pool, gives individual results for each die
    
    $: roll -p 2d10 3d4 1d123
    [5,8] [4,2] [88]
    
### `roll xdy+a-b`    
rolls a dy x times and sums their results, modifies final sum with modifiers    
    
    $: roll 1d20+4-6 3d6-20+3
    [12 -> 10] [9 -> -8]
    
    $: roll -p 3d6+2-3
    [2->1, 3->2, 4->3]
    
    
### `roll xdy#z`   
rolls a dy x times and returning success if results are Greater or Equal than a Target Number
    
    $: roll 2d10#15 3d4#3 1d123#1409
    [13 | Fail] [6 | Success] [88 | Fail]
    
    $: roll -p 3d6+2-3#3
    [2->1, 3->2, 4->3 | 1 Successes]
    
    $: roll -p 3d6+2-3#2000
    [2->1, 3->2, 4->3 | 0 Successes]

### `roll xdy -t z`   
Sets a default target number of Z
    
    $: roll -t 5 -p 3d6 2d6#1
    [2, 3, 4 | 0 Successes, 1,3,2 | 3 Successes]

## Regarding the Randomness;
As suggested in a [post by Yale Professor James
Aspnes](http://www.cs.yale.edu/homes/aspnes/pinewiki/C(2f)Randomization.html)  `roll` uses the time from epoch in
seconds as a seed for the standard `rand()` C function. Although the argument can be made that this method is not
truly random or cryptographically insecure, it (as far as my research went) proved to be the most portable, using
native C libraries instead of any external depencies, as well as being sufficiently random for the use case of tabletop
gaming. 

That being said, I am quite aware that players might take issue in using a non-truly random, not-cryptographically
secure, non "atmosphere-noise or nuclear decay" based algorithm instead of cheap and probably unbalanced plastic dice.
So here is the official warning: __running `roll` several times in quick succesion will result in the same results__.

Have fun!

## Authors
* **João Victor Guimarães** - [jvlsg](https://github.com/jvlsg/)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
