# roll
A simple Diceroller in C for RPG and CLI enthusiasts
by João Victor Guimarães

## USAGE
`roll [options] XdY[+A-B#Z] ...`
Where:
`X` is Number of Rolls
`Y` is the type of die
`A` and B are optional increments and decrements, respectively
`Z` is a Target Number. A roll will be successful if Result >= TN
It is possible to make several rolls and use several modifiers to each roll.

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
    [13 - Fail] [6 - Success] [88 - Fail]
    
    $: roll -p 3d6+2-3#3
    [2->1, 3->2, 4->3 - 1 Success]
    
    $: roll -p 3d6+2-3#2000
    [2->1, 3->2, 4->3 - 0 Success]

### `roll xdy -t z`   
Sets a default target number of Z
    
    $: roll -t 5 -p 3d6 2d6#1
    [2, 3, 4 - 0 Successes, 1,3,2 - 3 Successes]
