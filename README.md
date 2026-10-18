# Linguistic Set Operations (C)

A C program that treats each paragraph of a text file as a **set of unique words** and performs set operations on them: union, intersection, difference, subset and complement.

Developed as a data structures project at ESI by Abdelghani Oumouna and Laouici Mohammed Amine.

## How it works

### Data structure: a linked list of BSTs

Each paragraph is stored as a binary search tree (BST) of its unique words. The paragraphs are chained together in a linked list, in the order they appear in the file.

```c
typedef struct TreeNode {
    char *word;              // dynamically allocated string
    struct TreeNode *fg;     // left child
    struct TreeNode *fd;     // right child
} TreeNode;

typedef struct ListNode {
    int para_num;            // paragraph number
    TreeNode *root;          // root of the paragraph's BST
    struct ListNode *next;
} ListNode;
```

**Why not a linked list of linked lists?** Searching a word would be O(n), and set operations need many searches. With a BST, search is O(log n) on average, and duplicates are ignored naturally on insertion (`strcmp == 0`), which gives the set property for free.

### Abstract machine

The code never touches struct fields directly. Each structure has primitive operations (`fg()`, `fd()`, `val()`, `ass_fg()`, `ass_fd()`, `next()`, `root()`, ...) and everything else is built on top of them.

### File reading

1. Read the file line by line with `fgets`.
2. Split each line into words with `strtok` (spaces, tabs and punctuation as delimiters).
3. Clean each word: convert to lowercase and strip leading/trailing punctuation.
4. Insert it into the current paragraph's BST.
5. On a blank line, save the current BST as a new list node and start a new paragraph.

## Operations

| Operation | Description |
|---|---|
| Union | All words from both paragraphs |
| Intersection | Words present in both paragraphs |
| Difference (A \ B) | Words in A that are not in B |
| Subset | Whether every word of A is also in B |
| Complement | All words in the file (union of all paragraphs) that are not in the chosen paragraph |

The operations are built on three helpers: `copy`, `intersect` and `difference`. Trees are traversed in **preorder** when copying, so the new tree keeps the shape of the original (inserting in alphabetical order would produce a degenerate, list-like tree).

## Build and run

```bash
gcc -o setops linguistic_sets.c
./setops
```

Two sample inputs are included: `data1.txt` and `data2.txt`. Paragraphs must be separated by a blank line.

## Limitations

- The BST is not self-balancing, so the worst case (e.g. already sorted input) degrades to O(n).
- Word cleaning is basic: it only lowercases and strips punctuation at the edges of words.

## Authors

- Abdelghani Oumouna
- Laouici Mohammed Amine