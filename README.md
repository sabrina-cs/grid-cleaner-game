# grid_cleaner_game
Move a robot across a 10x10 grid, clear away any dirt, stay clear of walls, and recharge on chargers in this tiny terminal game.
The twist is that you can create your own level in setup mode before you start playing!
Win condition: all dirt tiles are cleaned.

---

## Setup Mode (Level Editor)
You design the board in setup mode when you first launch the game.
After entering commands and coordinates, hit Enter!
	•	w r c         → place Wall at (row, col)
	•	d r c         → place Dirt at (row, col)
	•	h r c         → place Charger at (row, col)
	•	L r1 c1 r2 c2 → draw a straight line of walls (must be horizontal or vertical)
	•	q             → finish setup and start the game

---

## Play Mode
Controls during play:
	•	w a s d → move the robot (wraps around edges, can’t pass walls)
	•	b       → show battery percentage
	•	r       → recharge battery (must stand on a charger)
	•	c       → show moves made so far

Symbols:
	•	@ → the robot (player)
	•	* → dirt (needs to be cleaned)
	•	### → wall
	•	+ → charger (recharges battery)
	•	empty space → clean floor

---

## 📚 Concepts Practiced
-	Arrays and indexing
-	Loops and conditionals
-	Functions and modular programming
-	Handling user input in C

---

## ⚙️ How to Compile & Run
Make sure you have **GCC** installed

```bash
# Compile
gcc grid_cleaner.c -o grid_cleaner

# Run
./grid_cleaner
