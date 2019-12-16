# Advent of Code 2019

I'm using these puzzles to learn C, since I almost always only use Python.

## Things I've learned about each day:

### [Day 1](01) - Fuel calculation

- Includes, standard library and relative files
- Declaring headers
- Reading files
- Printing output
- Basic C types (char, int) and syntax (functions, conditionals, loops)
- Using gcc

### [Day 2](02) - Intcode v0

- Working with arrays and pointers to arrays
- Biggest new thing here is using strtok, which I don't really
  entirely understand...

### [Day 3](03) - Crossing wires

- Working with strings - reading and comparing
- Converting chars to ints
- A bit more with pointers and addresses
- Coming from Python, using C makes it easier to justify brute force
  solutions to problems... it's still pretty fast, hah.

### [Day 4](04) - Number patterns

- Nothing too profound today

### [Day 5](05) - Intcode v1

- I like how this subtly built on the logic breaking down large
  numbers from Day 4, but again, nothing too profound

### [Day 6](06) - Orbit traversal

- Structs! (This was a lot to learn...)
- Pointer->notation

### [Day 7](07) - Intcode v2

- Keeping track of positions in multiple intcode instances and across
  multiple function calls was a challenge, and I didn't quite get it
  right today. Stored and passed around a bunch of indices rather than
  using pointers appropriately.
- Permutations were also unsatisfying; Google had to help with the algorithm
  (which I only 75% understand), and keeping track of the outputs was
  harder and messier than I suspect it needed to be...

### [Day 8](08) - Layered image processing

- Reading file character-by-character, and not relying on atoi
  and strtok (which is still as murky as it was on Day 2)

### [Day 9](09) - Intcode v3

- Using longs vs ints
- Convenient to have char-by-char file reading from yesterday,
  since atoi doesn't work anymore with longs

### [Day 10](10) - Asteroid detection

- Floats
- Nothing else too profound related to learning C, but worked through
  lots of math and logic.

### [Day 11](11) - Intcode v4

- Pointers! Although I used these a bit on previous days, I got a much
  better understanding of them today, since I refactored my intcode
  computer to use them to keep track of position over multiple
  function runs.

### [Day 12](12) - Lunar motion

- No profoundly new concepts
- Small things: typedef, do-while loop
- I was happy to implement Least Common Multiple algorithm without
  help, especially after Day 7's unsatisfying permutations.

### [Day 13](13) - Intcode v5

- Dynamic array allocation with malloc and realloc
- Null pointers
- Building Doxygen docs

### [Day 14](14) - Ore to fuel

- A lot more confidence with structs and pointers
- A lot less confidence with recursion... took me a long time with a lot
  of starting over to get to a solution that worked well.

### [Day 15](15) - Re-oxygenate

- Tiny C things like random numbers and sleeps, but feeling quite
  comfortable
- Mostly recursion - spent longer thinking on this one, so I was quicker
  to the solution once I started coding.
