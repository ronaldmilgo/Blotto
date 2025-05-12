# Blotto

This project implements a simulation of the **Blotto game**, where multiple players distribute fixed units across several battlefields. The program uses a custom map ADT backed by a hash table to store and compare player strategies efficiently.

## ⚔️ Overview

Blotto is a two-player game where each player allocates the same number of units across multiple battlefields. Each battlefield has a point value, and the player who allocates more units to a battlefield earns its points. In case of a tie, the points are split. The player with the highest total wins the matchup.

## 🎯 Objectives

- Implement a hash table–based map ADT (gmap)  
- Use the gmap to store and compare player distributions  
- Efficiently simulate head-to-head matchups between players  

## 🧾 Input Format

The program is executed as:

```bash
./Blotto <value1> <value2> ... <valueN> < input.txt
```

Where:
- Each `<value>` is the point value of a battlefield
- `input.txt` contains two sections: player distributions and matchups

### 📌 Player Section

Each line:
```
ID,units1,units2,...,unitsN
```

- `ID`: A unique identifier (≤ 31 characters, no commas or whitespace)
- `unitsX`: A non-negative integer allocation for each battlefield

### 📌 Matchup Section

After a blank line, each line:
```
ID1 ID2
```
Represents a matchup between two previously defined players.

## 💻 Example

**Command:**
```bash
./Blotto 1 2 3 4 < blotto.in
```

**Input (blotto.in):**
```
P1,2,2,2,3
P2,3,3,2,1
P3,2,2,5,0

P1 P2
P1 P3
P2 P3
```

**Output:**
```
P1 5.5 - P2 4.5
P1 5.5 - P3 4.5
P2 7.0 - P3 3.0
```

## 🧠 gmap ADT

The program relies on a generic map ADT that maps keys to values, using client-provided hash, copy, compare, and free functions. The map is implemented as a **chained hash table**.

### Supported Functions

- `gmap_create`  
- `gmap_put`, `gmap_get`, `gmap_remove`  
- `gmap_contains_key`, `gmap_size`, `gmap_keys`  
- `gmap_for_each`, `gmap_destroy`

### Performance Goals

| Function            | Expected Time | Worst-Case Time |
|---------------------|---------------|------------------|
| `gmap_get`          | O(1)          | O(n)             |
| `gmap_put`          | O(1)          | O(n)             |
| `gmap_remove`       | O(1)          | O(n)             |
| `gmap_contains_key` | O(1)          | O(n)             |
| `gmap_for_each`     | O(n)          | O(n)             |

(`n` = number of key/value pairs)

## 📊 Output Format

Each matchup result is printed as:

```
WINNER_ID score - LOSER_ID score
```

- Ties go to the first listed player
- Scores are formatted with **one decimal point**
- Output order matches input matchup order
- No additional output is printed

---

This project demonstrates the use of abstract data types, hash table design, and simulation of competitive strategy games using efficient in-memory data structures.
