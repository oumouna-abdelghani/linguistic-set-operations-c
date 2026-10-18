/*  OUMOUNA ABDELGHANI & LAOUICI MOHAMMED AMINE ----- G 08         */
/*  OUMOUNA ABDELGHANI & LAOUICI MOHAMMED AMINE ----- G 08         */
/*  OUMOUNA ABDELGHANI & LAOUICI MOHAMMED AMINE ----- G 08         */
/*  OUMOUNA ABDELGHANI & LAOUICI MOHAMMED AMINE ----- G 08         */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================
 * ADDS Lab Work — Linguistic Set Operations
 * Structure: Linked List of BSTs
 * Each list node = one paragraph
 * Each BST = the unique words of that paragraph
 * ============================================================ */

/* ── Structs ─────────────────────────────────────────────── */

typedef struct TreeNode {
    char            *word;      /* IMPROVEMENT: Dynamic pointer instead of fixed array */
    struct TreeNode *fg;        /* left child  */
    struct TreeNode *fd;        /* right child */
} TreeNode;

typedef struct ListNode {
    int              para_num;
    TreeNode        *root;
    struct ListNode *next;
} ListNode;

/* ============================================================
 * Abstract Machine — TreeNode (BST)
 * ============================================================ */

TreeNode *Allocate_T() {
    TreeNode *p = (TreeNode *)malloc(sizeof(TreeNode));
    if (!p) { printf("Memory error\n"); exit(1); }
    p->word = NULL;             /* IMPROVEMENT: Initialize pointer to NULL */
    p->fg = NULL;
    p->fd = NULL;
    return p;
}

/* IMPROVEMENT: Added a primitive to free the dynamically allocated word and the node */
void Free_T(TreeNode *p) {
    if (p->word) free(p->word);
    free(p);
}

char *val(TreeNode *p) {
    return p->word;
}

void ass_val(TreeNode *p, const char *w) {
    /* IMPROVEMENT: Malloc exact length of the word (+1 for null terminator) */
    if (p->word) free(p->word); 
    p->word = (char *)malloc(strlen(w) + 1);
    if (!p->word) { printf("Memory error\n"); exit(1); }
    strcpy(p->word, w);
}

TreeNode *fg(TreeNode *p) { return p->fg; }
TreeNode *fd(TreeNode *p) { return p->fd; }

void ass_fg(TreeNode *p, TreeNode *q) { p->fg = q; }
void ass_fd(TreeNode *p, TreeNode *q) { p->fd = q; }

/* ============================================================
 * Abstract Machine — ListNode (Linked List)
 * ============================================================ */

ListNode *Allocate_L() {
    ListNode *p = (ListNode *)malloc(sizeof(ListNode));
    if (!p) { printf("Memory error\n"); exit(1); }
    p->para_num = 0;
    p->root = NULL;
    p->next = NULL;
    return p;
}

int      para_num(ListNode *p)              { return p->para_num; }
void     ass_para_num(ListNode *p, int n)   { p->para_num = n; }

TreeNode *root(ListNode *p)               { return p->root; }
void      ass_root(ListNode *p, TreeNode *r) { p->root = r; }

ListNode *next_node(ListNode *p)          { return p->next; }
void      ass_next(ListNode *p, ListNode *q) { p->next = q; }

/* ============================================================
 * BST Operations
 * ============================================================ */

/*
 * BST_insert — inserts a word alphabetically.
 * strcmp < 0 → go left | strcmp > 0 → go right | = 0 → duplicate, skip.
 */
TreeNode *BST_insert(TreeNode *r, const char *word) {
    if (r == NULL) {
        TreeNode *node = Allocate_T();
        ass_val(node, word);
        ass_fg(node, NULL);
        ass_fd(node, NULL);
        return node;
    }
    int cmp = strcmp(word, val(r));
    if      (cmp < 0) ass_fg(r, BST_insert(fg(r), word));
    else if (cmp > 0) ass_fd(r, BST_insert(fd(r), word));
    /* cmp == 0: already exists, do nothing */
    return r;
}

/* BST_search — returns 1 if word found, 0 otherwise. O(log n). */
int BST_search(TreeNode *r, const char *word) {
    if (r == NULL) return 0;
    int cmp = strcmp(word, val(r));
    if (cmp == 0) return 1;
    if (cmp < 0)  return BST_search(fg(r), word);
    return              BST_search(fd(r), word);
}

/* BST_inorder — Left → Root → Right (prints words alphabetically) */
void BST_inorder(TreeNode *r) {
    if (r == NULL) return;
    BST_inorder(fg(r));
    printf("  %s\n", val(r));
    BST_inorder(fd(r));
}

/* BST_size — number of nodes in the BST */
int BST_size(TreeNode *r) {
    if (r == NULL) return 0;
    return 1 + BST_size(fg(r)) + BST_size(fd(r));
}

/* BST_free — frees all BST memory, postorder (children before parent) */
void BST_free(TreeNode *r) {
    if (r == NULL) return;
    BST_free(fg(r));
    BST_free(fd(r));
    Free_T(r); /* IMPROVEMENT: Use the new primitive to properly free the word + node */
}

/* ============================================================
 * Linked List Operations
 * ============================================================ */

/* create_list — appends a new paragraph node at the end of the list */
void create_list(ListNode **head, int num, TreeNode *bst_root) {
    ListNode *new_node = Allocate_L();
    ass_para_num(new_node, num);
    ass_root(new_node, bst_root);
    ass_next(new_node, NULL);

    if (*head == NULL) { *head = new_node; return; }

    ListNode *current = *head;
    while (next_node(current) != NULL)
        current = next_node(current);
    ass_next(current, new_node);
}

/* get_paragraph — returns the node for paragraph n, or NULL */
ListNode *get_paragraph(ListNode *head, int n) {
    ListNode *current = head;
    while (current != NULL) {
        if (para_num(current) == n) return current;
        current = next_node(current);
    }
    return NULL;
}

/* display_list — prints all paragraphs and their word sets */
void display_list(ListNode *head) {
    ListNode *current = head;
    while (current != NULL) {
        printf("--- Paragraph %d  (%d unique words) ---\n",
               para_num(current), BST_size(root(current)));
        BST_inorder(root(current));
        printf("\n");
        current = next_node(current);
    }
}

/* count_list — returns the number of paragraphs */
int count_list(ListNode *head) {
    int count = 0;
    ListNode *current = head;
    while (current != NULL) { count++; current = next_node(current); }
    return count;
}

/* free_list — frees the entire list and all its BSTs */
void free_list(ListNode **head) {
    ListNode *current = *head;
    while (current != NULL) {
        ListNode *nxt = next_node(current);
        BST_free(root(current));
        free(current);
        current = nxt;
    }
    *head = NULL;
}

/* ============================================================
 * File Reading and Pre-processing
 * ============================================================ */

/* clean_word — lowercases and strips punctuation from both ends */
void clean_word(char *word) {
    for (int i = 0; word[i]; i++)
        word[i] = tolower((unsigned char)word[i]);

    int len = strlen(word);
    while (len > 0 && ispunct((unsigned char)word[len - 1]))
        word[--len] = '\0';

    int start = 0;
    while (word[start] && ispunct((unsigned char)word[start])) start++;
    if (start > 0) memmove(word, word + start, strlen(word) - start + 1);
}

/*
 * load_file — reads a text file and builds a linked list of BSTs.
 * Paragraphs are separated by blank lines.
 * Each word is cleaned then inserted into the current paragraph's BST.
 */
ListNode *load_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) { printf("Error: cannot open '%s'\n", filename); return NULL; }

    ListNode *head        = NULL;
    TreeNode *current_bst = NULL;
    int       num         = 1;
    int       has_content = 0;
    char      line[1024];

    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';

        int blank = 1;
        for (int i = 0; line[i]; i++)
            if (!isspace((unsigned char)line[i])) { blank = 0; break; }

        if (blank) {
            if (has_content) {
                create_list(&head, num, current_bst);
                num++;
                current_bst = NULL;
                has_content = 0;
            }
        } else {
            char  temp[1024];
            strcpy(temp, line);
            char *token = strtok(temp, " \t\n\r.,;:!?()\"'*");
            while (token != NULL) {
                char word[100];
                strncpy(word, token, 99); word[99] = '\0';
                clean_word(word);
                if (strlen(word) > 0) {
                    current_bst = BST_insert(current_bst, word);
                    has_content = 1;
                }
                token = strtok(NULL, " \t\n\r.,;:!?()\"'*");
            }
        }
    }
    if (has_content)
        create_list(&head, num, current_bst);

    fclose(f);
    return head;
}

/* ============================================================
 * Set Operations
 * All return a brand-new BST. Originals are never modified.
 * ============================================================ */

void _copy(TreeNode *t, TreeNode **result) {
    if (t == NULL) return;
    *result = BST_insert(*result, val(t));
    _copy(fg(t), result);
    _copy(fd(t), result);
}

void _intersect(TreeNode *t1, TreeNode *t2, TreeNode **result) {
    if (t1 == NULL) return;
    if (BST_search(t2, val(t1)))
        *result = BST_insert(*result, val(t1));
    _intersect(fg(t1), t2, result);
    _intersect(fd(t1), t2, result);
}

void _difference(TreeNode *t1, TreeNode *t2, TreeNode **result) {
    if (t1 == NULL) return;
    if (!BST_search(t2, val(t1)))
        *result = BST_insert(*result, val(t1));
    _difference(fg(t1), t2, result);
    _difference(fd(t1), t2, result);
}

/* union_set — all words from t1 AND t2, duplicates removed automatically */
TreeNode *union_set(TreeNode *t1, TreeNode *t2) {
    TreeNode *result = NULL;
    _copy(t1, &result);
    _copy(t2, &result);
    return result;
}

/* intersection — words present in BOTH t1 and t2 */
TreeNode *intersection(TreeNode *t1, TreeNode *t2) {
    TreeNode *result = NULL;
    _intersect(t1, t2, &result);
    return result;
}

/* difference — words in t1 that are absent from t2  (A \ B) */
TreeNode *difference(TreeNode *t1, TreeNode *t2) {
    TreeNode *result = NULL;
    _difference(t1, t2, &result);
    return result;
}

/* ============================================================
 * Subset and Complement
 * ============================================================ */

int _is_subset(TreeNode *t1, TreeNode *t2) {
    if (t1 == NULL) return 1;
    if (!BST_search(t2, val(t1))) return 0;
    return _is_subset(fg(t1), t2) && _is_subset(fd(t1), t2);
}

/* is_subset — returns 1 if t1 ⊆ t2, 0 otherwise */
int is_subset(TreeNode *t1, TreeNode *t2) {
    /* IMPROVEMENT: Mathematical cardinality check */
    if (BST_size(t1) > BST_size(t2)) return 0; 
    
    return _is_subset(t1, t2);
}

/* build_universe — union of all paragraphs in a list */
TreeNode *build_universe(ListNode *head) {
    TreeNode *universe = NULL;
    ListNode *current  = head;
    while (current != NULL) {
        _copy(root(current), &universe);
        current = next_node(current);
    }
    return universe;
}

/* complement — words in the universe that are NOT in t */
TreeNode *complement(TreeNode *universe, TreeNode *t) {
    return difference(universe, t);
}

/* ============================================================
 * User Interface
 * ============================================================ */

void print_sep() { printf("========================================\n"); }

void print_result(TreeNode *result, const char *op_name) {
    print_sep();
    printf("Result of %s:\n", op_name);
    if (result == NULL) printf("  (empty set)\n");
    else {
        printf("  Words (%d total):\n", BST_size(result));
        BST_inorder(result);
    }
    print_sep();
}

int pick_paragraphs(ListNode *list1, ListNode *list2, int same_file,
                    ListNode **p1, ListNode **p2) {
    int n1, n2;
    printf("Enter first paragraph number: "); scanf("%d", &n1);
    *p1 = get_paragraph(list1, n1);
    if (!*p1) { printf("Paragraph %d not found in file 1.\n", n1); return 0; }

    if (same_file) {
        printf("Enter second paragraph number (same file): "); scanf("%d", &n2);
        *p2 = get_paragraph(list1, n2);
        if (!*p2) { printf("Paragraph %d not found in file 1.\n", n2); return 0; }
    } else {
        if (!list2) { printf("No second file loaded.\n"); return 0; }
        printf("Enter second paragraph number (file 2): "); scanf("%d", &n2);
        *p2 = get_paragraph(list2, n2);
        if (!*p2) { printf("Paragraph %d not found in file 2.\n", n2); return 0; }
    }
    return 1;
}

/* ============================================================
 * Main
 * ============================================================ */

int main() {
    ListNode *list1 = NULL, *list2 = NULL;
    char filename[256];
    int  choice;

    print_sep();
    printf("  ADDS Lab — Linguistic Set Operations\n");
    printf("  Structure: Linked List of BSTs\n");
    print_sep();

    printf("Enter path to file 1: ");
    scanf("%255s", filename);
    list1 = load_file(filename);
    if (!list1) { printf("Could not load file 1.\n"); return 1; }
    printf("File 1 loaded: %d paragraph(s).\n\n", count_list(list1));

    printf("Load a second file? (1=yes / 0=no): ");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("Enter path to file 2: ");
        scanf("%255s", filename);
        list2 = load_file(filename);
        if (!list2) printf("Could not load file 2. Continuing with file 1 only.\n");
        else printf("File 2 loaded: %d paragraph(s).\n\n", count_list(list2));
    }

    int running = 1;
    while (running) {
        print_sep();
        printf("MAIN MENU\n");
        printf("1. Display all paragraphs (file 1)\n");
        printf("2. Display all paragraphs (file 2)\n");
        printf("3. Union\n");
        printf("4. Intersection\n");
        printf("5. Difference  (A \\ B)\n");
        printf("6. Subset check  (is A a subset of B?)\n");
        printf("7. Complement  (of a paragraph in file 1)\n");
        printf("0. Exit\n");
        print_sep();
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) { running = 0; break; }
        if (choice == 1) { display_list(list1); continue; }
        if (choice == 2) {
            if (!list2) printf("No second file loaded.\n");
            else display_list(list2);
            continue;
        }

        int same_file = 1;
        if (list2 && choice >= 3 && choice <= 6) {
            printf("Paragraphs from (1) same file or (2) different files? ");
            int src; scanf("%d", &src);
            same_file = (src == 1);
        }

        ListNode *p1 = NULL, *p2 = NULL;
        if (choice >= 3 && choice <= 6)
            if (!pick_paragraphs(list1, list2, same_file, &p1, &p2)) continue;

        TreeNode *result = NULL;

        switch (choice) {
            case 3:
                result = union_set(root(p1), root(p2));
                print_result(result, "Union");
                BST_free(result); break;
            case 4:
                result = intersection(root(p1), root(p2));
                print_result(result, "Intersection");
                BST_free(result); break;
            case 5:
                result = difference(root(p1), root(p2));
                print_result(result, "Difference (A \\ B)");
                BST_free(result); break;
            case 6:
                printf("Is paragraph %d a subset of paragraph %d?  %s\n",
                       para_num(p1), para_num(p2),
                       is_subset(root(p1), root(p2)) ? "YES" : "NO");
                break;
            case 7: {
                int n;
                printf("Enter paragraph number to complement: ");
                scanf("%d", &n);
                ListNode *para = get_paragraph(list1, n);
                if (!para) { printf("Paragraph not found.\n"); break; }
                TreeNode *universe = build_universe(list1);
                result = complement(universe, root(para));
                print_result(result, "Complement");
                BST_free(universe);
                BST_free(result);
                break;
            }
            default: printf("Invalid choice.\n");
        }
    }

    free_list(&list1);
    if (list2) free_list(&list2);
    printf("Goodbye!\n");
    return 0;
}