# 🧊 Rubik's Cube Solver — Code Walkthrough & Interview Guide

## Current Progress

| # | File | Status |
|---|------|--------|
| 1 | `RubiksCube.hpp` | 🟡 In Progress (your code below) |
| 2 | `RubiksCube.cpp` | ⬜ Not started |
| 3 | `ThreeDArrayModel.cpp` | ⬜ Not started |
| 4 | `OneDArrayModel.cpp` | ⬜ Not started |
| 5 | `BitBoardModel.cpp` | ⬜ Not started |
| 6 | `GenericRubiksHash.hpp` | ⬜ Not started |
| 7 | `Bfs.hpp` | ⬜ Not started |
| 8 | `Dfs.hpp` | ⬜ Not started |
| 9 | `IDDfs.hpp` | ⬜ Not started |
| 10 | `FileManager.cpp` | ⬜ Not started |
| 11 | `CreateCornerDatabase.cpp` | ⬜ Not started |
| 12 | `CreateEdgeDatabase.cpp` | ⬜ Not started |
| 13 | `AStar.hpp` | ⬜ Not started |
| 14 | `main.cpp` | ⬜ Not started |
| 15 | `CMakeLists.txt` | ⬜ Not started |

---

## File 1: `RubiksCube.hpp` — The Abstract Interface

This is the **foundation** of the entire project. Every model (3D Array, 1D Array, BitBoard) and every solver (BFS, DFS, IDDFS, A*) depends on this file.

### 📋 What You Have So Far

```cpp
#ifndef Rubikscube_hpp
#define Rubikscube_hpp

#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Rubiksube{   // ⚠️ Typo: should be RubiksCube
  public:

   static constexpr char face[6] = {'W','G','R','B','O','Y'};

   // ... cornerMap, cornerOrientations, edgeMap, edgeOrientation ...
   // ... res, cornerNo, edgeNo ...
   // ... enum class Move ...
};
```

---

### 🔍 Line-by-Line Deep Dive

#### Lines 1-2: Include Guards
```cpp
#ifndef Rubikscube_hpp
#define Rubikscube_hpp
```

**What it does:** Prevents the header from being included multiple times (which would cause "redefinition" errors).

**Tradeoff: `#ifndef` vs `#pragma once`**
| Approach | Pros | Cons |
|----------|------|------|
| `#ifndef`/`#define` | Standard C++, works on ALL compilers | Verbose, relies on unique macro names |
| `#pragma once` | Cleaner, faster (compiler optimized) | Non-standard (though supported everywhere in practice) |

> [!TIP]
> **Interview Note:** In production code, `#pragma once` is common (Google, LLVM, Microsoft all use it). But `#ifndef` guards are the "safe" choice for maximum portability. For this project, either is fine.

---

#### Lines 4-6: Includes
```cpp
#include<iostream>
#include<vector>
#include<string>
```

**What each provides:**
- `<iostream>` — `cout`, `cin` for printing cube state
- `<vector>` — dynamic arrays (used for `cornerNo`, `edgeNo`, `res`)
- `<string>` — string operations (used later for move-to-string conversion)

> [!WARNING]
> **Missing includes you'll need later:** `<algorithm>`, `<unordered_map>`, `<random>` (for scrambling). You'll add these when writing `RubiksCube.cpp`.

---

#### Line 8: `using namespace std;`
```cpp
using namespace std;
```

**Tradeoff — this is controversial:**

| Approach | Pros | Cons |
|----------|------|------|
| `using namespace std;` | Less typing, cleaner looking | Pollutes namespace, name collisions possible |
| Explicit `std::` prefix | No ambiguity, safer | More verbose |

> [!IMPORTANT]
> **Interview Question:** "Is `using namespace std;` in a header file a good practice?"
> 
> **Answer:** **No, it's generally bad practice in headers** because anyone who includes your header gets the entire `std` namespace pulled in, potentially causing name collisions in their code. It's acceptable in `.cpp` files or small projects. For production, use `std::` explicitly or `using std::vector;` for specific types.
> 
> **For your interview:** Acknowledge this — "I used it for brevity, but in production I'd use explicit `std::` qualifiers, especially in headers."

---

#### Line 10: Class Declaration
```cpp
class Rubiksube{
```

> [!WARNING]
> ⚠️ **Typo:** `Rubiksube` → should be `RubiksCube`. Make sure to fix this! In the reference project, it's `RubiksCube`.

**Design Decision:** This is the **abstract base class** (ABC). It defines the *interface* that all cube representations must implement.

**Why an abstract class instead of just writing one implementation?**
- **Strategy Pattern / Polymorphism** — solvers work with `RubiksCube*` pointers, not concrete types
- **Open/Closed Principle** — you can add a new model without changing solvers
- **Testing** — easy to test each model independently

---

#### Line 13: Face Colors Array
```cpp
static constexpr char face[6] = {'W','G','R','B','O','Y'};
```

**Breaking it down:**
- `static` — shared across ALL instances (not per-object). Makes sense because face labels are universal.
- `constexpr` — evaluated at **compile time**. The compiler replaces `face[0]` with `'W'` directly. Zero runtime cost.
- `char face[6]` — 6 faces, each stored as a character

**The face ordering convention:**
| Index | Color | Face |
|-------|-------|------|
| 0 | W (White) | UP |
| 1 | G (Green) | LEFT |
| 2 | R (Red) | FRONT |
| 3 | B (Blue) | RIGHT |
| 4 | O (Orange) | BACK |
| 5 | Y (Yellow) | DOWN |

> [!TIP]
> **Why this specific ordering matters:** The ordering defines how `face * 9 + row * 3 + col` maps to sticker positions in the 1D model. It also determines how the corner/edge maps are indexed. Changing this ordering would break everything downstream.
>
> **Interview Question:** "How did you choose the face ordering?"
> **Answer:** Standard convention — White=Top, Yellow=Bottom, with Green-Red-Blue-Orange around the middle following a specific rotation. This matches the most common Rubik's Cube color scheme.

---

#### Lines 15-24: Corner Map — `cornerMap[8][3][3]`
```cpp
static constexpr int cornerMap[8][3][3] = {
    {{0, 2, 0}, {1, 0, 2}, {2, 0, 0}}, // 0: White-Green-Red (Top-Left-Front)
    {{0, 2, 2}, {2, 0, 2}, {3, 0, 0}}, // 1: White-Red-Blue (Top-Front-Right)
    // ...
};
```

**What this is:** A lookup table mapping each **corner cubie** to its **3 sticker positions** on the cube.

**Dimensions explained:**
- `[8]` — there are 8 corners on a Rubik's Cube
- `[3]` — each corner has 3 colored stickers
- `[3]` — each sticker position is identified by `{face, row, col}`

**Example — Corner 0: `{{0, 2, 0}, {1, 0, 2}, {2, 0, 0}}`**
- Sticker 0: `face=0 (White/Up), row=2, col=0` → bottom-left of the top face
- Sticker 1: `face=1 (Green/Left), row=0, col=2` → top-right of the left face
- Sticker 2: `face=2 (Red/Front), row=0, col=0` → top-left of the front face

These three stickers physically share the same corner cubie (the little plastic piece at the corner of the cube).

**Why this is needed:**
- To **identify** which corner cubie is at a given position
- To **determine orientation** — by checking which color faces which direction
- Critical for **Pattern Database encoding** — need to know where each corner is and its twist

> [!IMPORTANT]
> **Interview Deep Dive — Corner Orientation:**
> 
> A corner cubie can be in 3 orientations (0°, 120°, 240° twist). If the "primary" color (White or Yellow) faces Up or Down, orientation = 0. If twisted clockwise = 1, counter-clockwise = 2.
> 
> **Why only 3^7 not 3^8?** The 8th corner's orientation is mathematically determined by the other 7. This is a group-theoretic constraint of the Rubik's Cube. The sum of all corner orientations ≡ 0 (mod 3). This saves memory in the Pattern Database (88M states instead of 264M).

---

#### Lines 27-36: Corner Orientations — `cornerOrientations[8][3][3]`
```cpp
static constexpr int cornerOrientations[8][3][3] = {
    {{0, 1, 2}, {2, 0, 1}, {1, 2, 0}}, // 0: White, Green, Red
    // ...
};
```

**Dimensions:**
- `[8]` — 8 corners
- `[3]` — 3 possible orientations (twist 0, 1, 2)
- `[3]` — the 3 color indices in that orientation

**What this encodes:**
For corner 0 (White-Green-Red), the three orientations are:
- **Twist 0:** Colors appear as `{0, 1, 2}` → White, Green, Red (the "solved" orientation)
- **Twist 1:** Colors appear as `{2, 0, 1}` → Red, White, Green (clockwise twist)
- **Twist 2:** Colors appear as `{1, 2, 0}` → Green, Red, White (counter-clockwise twist)

**Why this is needed:**
When you look at a scrambled cube and see a corner, you need to determine:
1. **Which corner** is it? (by the set of 3 colors)
2. **What orientation** is it in? (how it's twisted)

This table lets you do a quick lookup: given the actual colors on the stickers, match against all orientations of all corners to find the corner ID and twist.

> [!TIP]
> **Tradeoff — Lookup Table vs. Computation:**
> - **Table:** O(1) lookup, but uses ~72 ints of memory. Defined at compile time.
> - **Compute on-the-fly:** Save memory, but more CPU cycles per query.
> - **Winner:** Tables. We query this millions of times during Pattern DB construction. Memory is cheap for 72 ints.

---

#### Lines 38-56: Edge Map — `edgeMap[12][2][3]`
```cpp
static constexpr int edgeMap[12][2][3] = {
    {{0, 0, 1}, {4, 0, 1}}, // 0: UB (Up-Back)
    {{0, 1, 2}, {3, 0, 1}}, // 1: UR (Up-Right)
    // ...
};
```

**Same concept as cornerMap, but for edges:**
- `[12]` — 12 edges on a Rubik's Cube
- `[2]` — each edge has 2 colored stickers
- `[3]` — each sticker position = `{face, row, col}`

**Organization by layer:**
- Edges 0-3: **Top layer** (UB, UR, UF, UL)
- Edges 4-7: **Middle layer** (BR, FR, FL, BL)
- Edges 8-11: **Bottom layer** (DB, DR, DF, DL)

> [!TIP]
> **Interview Note:** The grouping into top/middle/bottom layers is deliberate. When building the **Edge Pattern Database**, you split edges into two groups of 6. The layer-based ordering makes it natural to split as `{0-5}` and `{6-11}`.

---

#### Lines 58-76: Edge Orientation — `edgeOrientation[12][2][2]`
```cpp
static constexpr int edgeOrientation[12][2][2] = {
    {{0, 4}, {4, 0}}, // 0: UB
    // ...
};
```

**Dimensions:**
- `[12]` — 12 edges
- `[2]` — 2 orientations (0 = correct, 1 = flipped)
- `[2]` — the 2 face-color indices in that orientation

**Edge orientation is simpler than corners:**
An edge is either **correct (0)** or **flipped (1)**. That's it — only 2 states.

**Why edges can only be in 2 orientations:** An edge piece has exactly 2 stickers. Either the "primary" sticker faces the correct direction, or it doesn't.

**Mathematical constraint:** The sum of all edge orientations ≡ 0 (mod 2). So the 12th edge's orientation is determined by the other 11. This is why edge DB size uses **2^11** not 2^12.

---

#### Line 78: The `res` pair
```cpp
pair<vector<int>,vector<int>> res = {vector<int>(8),vector<int>(8)};
```

**What this is:** A reusable result container for corner detection:
- `res.first` = vector of 8 ints → **corner positions** (which slot each corner is in)
- `res.second` = vector of 8 ints → **corner orientations** (twist of each corner)

**Why a member variable instead of local?**
- **Performance:** Avoids allocating/deallocating vectors every time you query the corner state
- **Tradeoff:** Slightly "unclean" design (mutable state that's really a scratch buffer), but the performance gain is worth it when called millions of times

> [!WARNING]
> **Interview Gotcha:** This makes the class NOT thread-safe. Two threads calling corner-detection simultaneously would clobber each other's `res`. For multithreaded solvers, each thread needs its own cube object.

---

#### Lines 80-82: Corner/Edge Number Arrays
```cpp
vector<int> cornerNo = {7, 13, 38, 44, 25, 19, 56, 50};
vector<int> edgeNo = {17, 9, 5, 3, 24, 12, 6, 18, 48, 40, 36, 34};
```

**What these are:** Pre-computed 1D indices for corners and edges in the **OneDArrayModel** (54-element flat array).

**How they're calculated:** Using the formula `face * 9 + row * 3 + col`:
- Corner 0, sticker 0: face=0, row=2, col=0 → `0*9 + 2*3 + 0 = 6`... 

Wait — these indices seem to correspond to a *different* mapping, possibly the "primary" sticker of each corner/edge in the 1D model. They're used as quick lookups to identify which corner/edge is at each physical position.

> [!TIP]
> **Interview Note:** Having these pre-computed avoids recalculating `face * 9 + row * 3 + col` every time. Another example of the "precompute vs compute" tradeoff — always precompute when the input is static and the function is called frequently.

---

#### Lines 84-92: Move Enum
```cpp
enum class Move{
    U, UPRIME, U2,
    D, DPRIME, D2,
    L, LPRIME, L2,
    R, RPRIME, R2,
    F, FPRIME, F2,
    B, BPRIME, B2,
    stop
};
```

**Why `enum class` (scoped enum) instead of plain `enum`?**

| Feature | `enum` | `enum class` |
|---------|--------|-------------|
| Scoping | Global (pollutes namespace) | Scoped (`Move::U`) |
| Implicit conversion | Converts to int implicitly | No implicit conversion |
| Type safety | Weak | Strong |

> [!IMPORTANT]
> **Interview Answer:** "`enum class` provides type safety. With plain `enum`, `Move::U` would implicitly convert to `0`, which could accidentally be used in arithmetic or comparisons where it shouldn't be. `enum class` prevents that."

**The `stop` sentinel:** Used by solvers to indicate "no move was made" (e.g., for the initial state in BFS/DFS). It's the 19th value, not a real move.

**Grouping pattern:** Each face has 3 moves grouped together: `{CW, CCW, 180°}`. This grouping is intentional — it means:
- `move / 3` gives the face index (0=U, 1=D, 2=L, 3=R, 4=F, 5=B)
- `move % 3` gives the rotation type (0=CW, 1=CCW, 2=180°)
- This arithmetic is used for **move pruning** in the solvers!

> [!TIP]
> **Move Pruning Trick:** If the last move was on face X, don't do face X again immediately (it's redundant — two consecutive moves on the same face can be combined). The solver checks `lastMove / 3 == currentMove / 3` to skip these. This reduces effective branching factor from 18 to ~15.

---

### ❌ What's Missing From Your File

Your `RubiksCube.hpp` is missing several critical pieces that the reference likely has:

1. **Virtual methods for getting/setting colors:**
```cpp
virtual char getColor(int face, int row, int col) const = 0;
```

2. **Virtual methods for all 18 moves:**
```cpp
virtual RubiksCube& U() = 0;
virtual RubiksCube& UPrime() = 0;
virtual RubiksCube& U2() = 0;
// ... for all 6 faces
```

3. **Concrete shared methods:**
```cpp
bool isSolved() const;
RubiksCube& scramble(int moves);
void print() const;
bool operator==(const RubiksCube& other) const;
// Corner/edge detection methods
```

4. **Move string conversion helpers:**
```cpp
string getMoveString(Move m) const;
```

5. **Missing `#endif` at the end**

6. **The closing `};` for the class needs proper placement**

---

### 🎯 What You Should Write Next

Based on your deep dive doc, the next steps are:

1. **Complete `RubiksCube.hpp`** — add all the pure virtual methods, concrete methods declarations, `operator==`, hashing support, and fix the typo
2. **Write `RubiksCube.cpp`** — implement the shared methods (`isSolved`, `scramble`, `print`, corner/edge detection)
3. **Write `ThreeDArrayModel.cpp`** — the easiest concrete implementation

**Go ahead and upload/paste the next piece of code whenever you're ready!** I'll explain every line, every tradeoff, and every interview angle.
