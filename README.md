# grid-cleaner-game
The objective of this straightforward C console game is to clean every tile by moving around a grid.
The purpose of this project was to practice using arrays, loops, functions, and fundamental game logic in C.

---

## Game overview
- The grid starts filled with uncleaned tiles (`-`)
- The player (`P`) moves using **W A S D** keys:
  - `W` → move up  
  - `A` → move left  
  - `S` → move down  
  - `D` → move right  
- When the player steps on a tile, it becomes **cleaned** (`.`)
- The game ends when **all tiles are cleaned**.  

---

## ⚙️ How to Compile & Run
Make sure you have **GCC** installed

```bash
# Compile
gcc grid_cleaner.c -o grid_cleaner

# Run
./grid_cleaner

---

## 📚 Concepts Practiced
	-	Arrays and indexing
	-	Loops and conditionals
	-	Functions and modular programming
	-	Handling user input in C
