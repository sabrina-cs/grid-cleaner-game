/* 
grid_cleaner.c
A simple grid "robot vacuum" game.
Player cleans all dirt tiles while avoiding walls.

Win condition: all dirt cleaned.
*/

#include <stdio.h>
#include <string.h>

#define ROWS 10
#define COLS 10

// Base tile types
enum base {
    BASE_EMPTY = 0,
    BASE_WALL  = 1,
    BASE_CHARGER = 2
};

// One tile on the board
struct tile {
    enum base base;
    int dirt; // 1 = dirty, 0 = clean
};

// Player state
struct player {
    int row;
    int col;
    int moves;
    int battery; // 0..100
};

// Prototypes
void init_board(struct tile board[ROWS][COLS]);
void print_board(struct tile board[ROWS][COLS], struct player p);
void print_border(void);
int in_bounds(int r, int c);
int clamp_line_to_board(int *r1, int *c1, int *r2, int *c2);
void place_line(struct tile board[ROWS][COLS], int r1, int c1, int r2, int c2);
int any_dirt_left(struct tile board[ROWS][COLS]);

int valid_start(struct tile board[ROWS][COLS], int r, int c);
struct player make_player(int r, int c);

void do_setup(struct tile board[ROWS][COLS]);
void play_game(struct tile board[ROWS][COLS], struct player p);

struct player step(struct tile board[ROWS][COLS], struct player p, char dir);
int try_move(struct tile board[ROWS][COLS], int new_r, int new_c);



// Main ---------------------------------------------------------------------------------------------------
int main(void) {
    struct tile board[ROWS][COLS];
    init_board(board);

    printf("=== Grid Cleaner: Setup ===\n");
    print_board(board, (struct player){-1,-1,0,0});
    do_setup(board);

    int sr, sc;
    printf("Enter robot starting position: ");
    scanf("%d %d", &sr, &sc);
    while (!valid_start(board, sr, sc)) {
        printf("Position (%d, %d) is invalid\n", sr, sc);
        printf("Enter robot starting position: ");
        scanf("%d %d", &sr, &sc);
    }

    struct player p = make_player(sr, sc);
    printf("\n=== Starting Grid Cleaner! ===\n");
    print_board(board, p);

    play_game(board, p);
    return 0;
}

// Setup and play loops -----------------------------------------------------------------------------------
void do_setup(struct tile board[ROWS][COLS]) {
    char cmd;
    while (scanf(" %c", &cmd) != EOF) {
        if (cmd == 'q') {
            break;
        } else if (cmd == 'w' || cmd == 'd' || cmd == 'h') {
            int r, c;
            scanf("%d %d", &r, &c);
            if (!in_bounds(r, c)) {
                printf("Location out of bounds\n");
            } else {
                if (cmd == 'w') {
                    board[r][c].base = BASE_WALL;
                    board[r][c].dirt = 0; // walls cannot be dirty
                } else if (cmd == 'd') {
                    if (board[r][c].base != BASE_WALL) {
                        board[r][c].dirt = 1; // mark as dirty
                    }
                } else if (cmd == 'h') {
                    if (board[r][c].base != BASE_WALL) {
                        board[r][c].base = BASE_CHARGER;
                    }
                }
            }
        } else if (cmd == 'L') {
            int r1, c1, r2, c2;
            scanf("%d %d %d %d", &r1, &c1, &r2, &c2);
            if (!clamp_line_to_board(&r1, &c1, &r2, &c2)) {
                printf("Location out of bounds\n");
            }
            place_line(board, r1, c1, r2, c2);
        }
        print_board(board, (struct player){-1,-1,0,0});
    }
}

void play_game(struct tile board[ROWS][COLS], struct player p) {
    char dir;
    while (scanf(" %c", &dir) != EOF) {
        if (dir == 'b') {
            printf("Battery: %d%%\n", p.battery);
            continue;
        } else if (dir == 'c') {
            printf("Moves: %d\n", p.moves);
            continue;
        } else if (dir == 'r') {
            if (board[p.row][p.col].base == BASE_CHARGER) {
                p.battery = 100;
                printf("Recharged to 100%%.\n");
            } else {
                printf("Not on a charger.\n");
            }
            print_board(board, p);
            continue;
        } else if (dir=='w' || dir=='a' || dir=='s' || dir=='d') {
            if (p.battery <= 0) {
                printf("Battery empty — move to a charger and press 'r'.\n");
                print_board(board, p);
                continue;
            }
            struct player np = step(board, p, dir);
            p = np;
        } else {
            // ignore unknown keys
            continue;
        }

        if (!any_dirt_left(board)) {
            print_board(board, p);
            if (p.moves == 1) {
                printf("=== All clean in 1 move! ===\n");
            } else {
                printf("=== All clean in %d moves! ===\n", p.moves);
            }
            return;
        }
        print_board(board, p);
    }
}

// Movement and rules --------------------------------------------------------------------------------------
struct player step(struct tile board[ROWS][COLS], struct player p, char dir) {
    int nr = p.row;
    int nc = p.col;

    if (dir == 'w') nr--;
    if (dir == 's') nr++;
    if (dir == 'a') nc--;
    if (dir == 'd') nc++;

    // wrap-around
    if (nr < 0) nr = ROWS - 1;
    if (nr >= ROWS) nr = 0;
    if (nc < 0) nc = COLS - 1;
    if (nc >= COLS) nc = 0;

    if (try_move(board, nr, nc)) {
        p.row = nr;
        p.col = nc;
        p.moves++;
        p.battery--;

        // clean if dirty
        if (board[p.row][p.col].dirt == 1) {
            board[p.row][p.col].dirt = 0;
        }
    }
    return p;
}

int try_move(struct tile board[ROWS][COLS], int new_r, int new_c) {
    if (!in_bounds(new_r, new_c)) return 0;
    if (board[new_r][new_c].base == BASE_WALL) return 0;
    return 1;
}

// Board rules --------------------------------------------------------------------------------------------
void init_board(struct tile board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j].base = BASE_EMPTY;
            board[i][j].dirt = 0;
        }
    }
}

int in_bounds(int r, int c) {
    return !(r < 0 || r >= ROWS || c < 0 || c >= COLS);
}

int clamp_line_to_board(int *r1, int *c1, int *r2, int *c2) {
    // If both endpoints are fully out-of-bounds in both dimensions, reject.
    int all_out_rows = (*r1 < 0 && *r2 < 0) || (*r1 >= ROWS && *r2 >= ROWS);
    int all_out_cols = (*c1 < 0 && *c2 < 0) || (*c1 >= COLS && *c2 >= COLS);
    if (all_out_rows && all_out_cols) return 0;

    if (*r1 < 0) *r1 = 0;
    if (*r1 >= ROWS) *r1 = ROWS - 1;
    if (*r2 < 0) *r2 = 0;
    if (*r2 >= ROWS) *r2 = ROWS - 1;

    if (*c1 < 0) *c1 = 0;
    if (*c1 >= COLS) *c1 = COLS - 1;
    if (*c2 < 0) *c2 = 0;
    if (*c2 >= COLS) *c2 = COLS - 1;

    return 1;
}

void place_line(struct tile board[ROWS][COLS], int r1, int c1, int r2, int c2) {
    if (r1 == r2) {
        if (c1 > c2) { int t=c1; c1=c2; c2=t; }
        for (int c = c1; c <= c2; c++) {
            board[r1][c].base = BASE_WALL;
            board[r1][c].dirt = 0;
        }
    } else if (c1 == c2) {
        if (r1 > r2) { int t=r1; r1=r2; r2=t; }
        for (int r = r1; r <= r2; r++) {
            board[r][c1].base = BASE_WALL;
            board[r][c1].dirt = 0;
        }
    } else {
        // Only straight lines supported (simple for beginners)
        printf("Only straight lines are supported.\n");
    }
}

int any_dirt_left(struct tile board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j].dirt == 1) return 1;
        }
    }
    return 0;
}

int valid_start(struct tile board[ROWS][COLS], int r, int c) {
    if (!in_bounds(r, c)) return 0;
    if (board[r][c].base == BASE_WALL) return 0;
    return 1;
}

struct player make_player(int r, int c) {
    struct player p;
    p.row = r;
    p.col = c;
    p.moves = 0;
    p.battery = 100;
    return p;
}

// Printing ----------------------------------------------------------------------------------------------
void print_border(void) {
    for (int i = 0; i < COLS * 4 + 1; i++) {
        printf("-");
    }
    printf("\n");
}

void print_board(struct tile board[ROWS][COLS], struct player p) {
    // Title
    print_border();
    const char *title = " G R I D   C L E A N E R ";
    int len = COLS * 4 + 1;
    int n_white = len - (int)strlen(title) - 2;
    printf("|");
    for (int i = 0; i < n_white / 2; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < (n_white + 1) / 2; i++) printf(" ");
    printf("|\n");

    for (int r = 0; r < ROWS; r++) {
        print_border();
        for (int c = 0; c < COLS; c++) {
            printf("|");
            int is_player = (p.row == r && p.col == c);
            struct tile t = board[r][c];

            if (is_player) {
                printf(" @ ");
            } else if (t.base == BASE_WALL) {
                printf("###");
            } else if (t.dirt == 1) {
                printf(" * ");
            } else if (t.base == BASE_CHARGER) {
                printf(" + ");
            } else {
                printf("   ");
            }
        }
        printf("|\n");
    }
    print_border();
    printf("\n");
}
