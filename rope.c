// Copyright Raduta Lavinia && Baiatu Bianca 313CA 2021

#include "./rope.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

char *strdup(const char *s);


// structs
typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
    void* data;
    ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

typedef struct stack_t stack_t;
struct stack_t
{
	struct linked_list_t *list;
};

// linked list functions
linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t* list = (linked_list_t *)malloc(sizeof(linked_list_t));
	list->head = NULL;
    list->data_size = data_size;
	list->size = 0;
	return list;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *new_node, *curr;
    unsigned int i = 0;

    new_node = (ll_node_t*)malloc(sizeof(ll_node_t));
    DIE(!new_node, "malloc failed!");
    new_node->data = malloc(list->data_size);
    DIE(!new_node->data, "malloc failed!");
    memcpy(new_node->data, new_data, list->data_size);

    if (n >= list->size) {
        n = list->size;
    }
    // add at the begining of the list
    if (n == 0) {
        new_node->next = list->head;
        list->head = new_node;
        list->size++;
    } else {
        curr = list->head;
        while (i  < n - 1 && curr->next != NULL) {
            curr = curr->next;
            i++;
        }
        new_node->next = curr->next;
        curr->next = new_node;
        list->size++;
    }
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *curr = list->head, *to_delete;
    unsigned int i = 0;
    // if the list is empty
    if (!list->head) {
        DIE(1, "error, list is empty!");
    } else {
        // remove the first node
        if (n == 0) {
            list->head = curr->next;
            list->size--;
            return curr;
        } else {
            i = 0;
            if (n > list->size) {
                n = list->size - 1;
            }
            while (i  < n - 1) {
                curr = curr->next;
                i++;
            }
            to_delete = curr->next;
            curr->next = to_delete->next;
            list->size--;
            return to_delete;
        }
    }
}

unsigned int
ll_get_size(linked_list_t* list)
{
    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
    ll_node_t *curr;
    while ((*pp_list)->head != NULL) {
        curr = (*pp_list)->head;
        (*pp_list)->head = curr->next;
        free(curr->data);
        free(curr);
    }
    free(*pp_list);
    pp_list = NULL;
}

// stack functions
stack_t *
st_create(unsigned int data_size)
{
	stack_t *st = (stack_t *)malloc(sizeof(stack_t));
	DIE(!st, "malloc failed!");
	st->list = ll_create(data_size);
	return st;
}

unsigned int
st_get_size(stack_t *st)
{
	if (st->list->head) {
		return ll_get_size(st->list);
	} else {
		return 0;
	}
}

unsigned int
st_is_empty(stack_t *st)
{
	if (st_get_size(st)) {
		return 0;
	} else {
		return 1;
	}
}

void *
st_peek(stack_t *st)
{
	if (st_get_size(st)) {
		return st->list->head->data;
	} else {
		return NULL;
	}
}

void
st_pop(stack_t *st)
{
	if (!st_is_empty(st)) {
		ll_node_t *popped = ll_remove_nth_node(st->list, 0);
		free(popped->data);
		free(popped);
	} else {
		printf("The stack is empty!");
	}
}

void
st_push(stack_t *st, void *new_data)
{
	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{
	while (!st_is_empty(st)) {
		st_pop(st);
	}
}

void
st_free(stack_t *st)
{
	st_clear(st);
	ll_free(&st->list);
	free(st);
}

// rope functions
RopeNode* makeRopeNode(const char* str) {
	RopeNode* node = calloc(sizeof(RopeNode), 1);
	DIE(!node, "malloc failed");
	node->str = str;
	node->left = node->right = NULL;
	node->weight = strlen(str);
	return node;
}

RopeTree* makeRopeTree(RopeNode* root) {
	RopeTree* tree = calloc(sizeof(RopeTree), 1);
	DIE(!tree, "malloc failed");
	tree->root = root;
	return tree;
}

void free_node(RopeNode* rn) {
	if (!rn)
		return;
	if(rn->str)
		free((void*)rn->str);
	free_node(rn->left);
	free_node(rn->right);
	free(rn);
}

void free_tree(RopeTree* rt) {
	if (!rt)
		return;

	free_node(rt->root);
	free(rt);
}

// calculate the weight of the subtree that has node as root
int weight(RopeNode* rn) {
    if (!rn)
        return 0;

    return rn->weight + weight(rn->right);
}

void recalculate_weight(RopeNode *node) {
    if (!node->left) {
        return;
    }
    recalculate_weight(node->left);
    node->weight = weight(node->left);
}

void printRopeNode(RopeNode* rn) {
	if (!rn)
		return;

	if (!(rn->left) && !(rn->right)) {
		printf("%s", rn->str);
		return;
	}

	printRopeNode(rn->left);
	printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
	if(!rt->root) {
		printf("\n");
	}
	if (rt && rt->root) {
		printRopeNode(rt->root);
		printf("%s", "\n");
	}
}

// function used for copying a subtree with root in rn
RopeNode* copyrn(RopeNode* rn) {
	if (!rn)
		return NULL;

	RopeNode* new_rn = makeRopeNode(strdup(rn->str));
	new_rn->weight = rn->weight;
	new_rn->left = copyrn(rn->left);
	new_rn->right = copyrn(rn->right);
	return new_rn;
}

// returns the deep copy of a tree
RopeTree* copyrt(RopeTree* rt) {
	if (!rt)
		return NULL;

	RopeTree* new_rt = makeRopeTree(copyrn(rt->root));
	return new_rt;
}

RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
	if (!rt1) {
		return makeRopeTree(rt2->root);
	} else if (!rt2) {
		return makeRopeTree(rt1->root);
	} else {
		RopeNode* new_root = makeRopeNode(strdup(EMPTY));
		// remake the links between the 2 roots
		new_root->left = rt1->root;
		new_root->right = rt2->root;
		// recalculate the weight of the new root using the left subtree
		new_root->weight = weight(rt1->root);
		RopeTree* new_tree = makeRopeTree(new_root);
		return new_tree;
	}
}


// returns the node that contains the char at index idx in the rope
RopeNode* node_containing_char(RopeNode* node, int *idx) {
	if (node->weight <= *idx && node->right != NULL) {
		*idx = *idx - node->weight;
		return node_containing_char(node->right, idx);
	}
	if (node->weight > *idx && node->left != NULL) {
		return node_containing_char(node->left, idx);
	}
	return node;
}

char indexRope(RopeTree* rt, int idx) {
	RopeNode* found_node = node_containing_char(rt->root, &idx);
	return found_node->str[idx];
}

// the substring is created starting with the first character
char* search(RopeTree* rt, int start, int end) {
	char *str = calloc(end - start + 1, sizeof(char));
	DIE(!str, "malloc failed");
	int pos = 0;
	for (int i = start; i < end; i++) {
		int idx = i;
		RopeNode * node = node_containing_char(rt->root, &idx);
		str[pos] = node->str[idx];
		pos++;
	}
	str[pos] = '\0';
	return str;
}

// splitting the node in case the idx is in the middle of a string
void split_node(RopeNode* node, int idx) {
	char *str = calloc(strlen(node->str) + 1, sizeof(char));
	DIE(!str, "calloc failed");
	strcpy(str, node->str);

	// string used to store the first part of the string
	char *aux = calloc(idx + 1, sizeof(char));
	DIE(!aux, "calloc failed");
	strncpy(aux, str, idx);
	if (idx > (int)strlen(str)) {
		idx = strlen(str);
	}

	// string used to store the second part of the string
	char *aux1 = calloc(strlen(node->str) - idx + 1, sizeof(char));
	DIE(!aux1, "calloc failed");
	strcpy(aux1, str + idx);

	// the sring in the old node has to be replaced with EMPTY
	free((void *)(node->str));
	free(str);
	node->str = strdup(EMPTY);

	// create the new left and right children
	RopeNode *n_left = makeRopeNode(aux);
	RopeNode *n_right = makeRopeNode(aux1);
	node->left = n_left;
	node->right = n_right;
	node->weight = n_left->weight;
}

int num_children(RopeNode* node) {
	if (!node) {
		return 0;
	}
	if (node->right && node->left) {
		return 2;
	}
	if ((node->left && !node->right) || (!node->left && node->right)) {
		return 1;
	}
	if (!node->left && !node->right) {
		return 0;
	}
	return 0;
}

// splitting the tree while forming the right subtree
// we keep the parent in case the current node has to be removed
void split_tree(RopeNode *node, RopeNode *parent, int *idx,
				stack_t *st) {
	// verifies whether to go left or right
	if (node->weight <= *idx && node->right) {
		// going on the right subtree
		*idx = *idx - node->weight;
		// if the node is parent of a leaf
		if (!num_children(node->left) && !num_children(node->right)) {
			RopeTree *tree = makeRopeTree(node->right);
			// forming the right part containing the removed subtrees
            st_push(st, tree);
			// break the links with the removed subtree
			node->right = NULL;
            free(tree);
			return;
		} else {
			parent = node;
			split_tree(node->right, parent, idx, st);
		}
	} else {
		// going on the left subtree
		RopeTree *tree;
		if (node->weight > *idx) {
			// if it's a leaf parent
			if (!num_children(node->left) && !num_children(node->right)) {
				tree = makeRopeTree(node);
                st_push(st, tree);
				// break the link with its parent
				parent->right = NULL;
                free(tree);
				return;
			} else if (node->right) {
				// if the right subtree exists we cut it
				tree = makeRopeTree(node->right);
                st_push(st, tree);
				node->right = NULL;
                free(tree);
				if (node->left) {
					parent = node;
					split_tree(node->left, parent, idx, st);
				}
			}
		}
	}
}

SplitPair split(RopeTree* rt, int idx) {
	SplitPair pair;
	if (idx == 0) {
		pair.left = NULL;
		pair.right = rt->root;
		return pair;
	}
	// make a copy of the tree to not modify the original
	RopeTree *copy = makeRopeTree(copyrn(rt->root));
	int start = idx;
	RopeNode *starting_node = node_containing_char(copy->root, &start);
	// if the starting character is inside the string we have to split the node
	if (start > 0 || start < (int)strlen(starting_node->str)) {
		split_node(starting_node, start);
	}
	RopeTree *r_right = NULL;
	RopeNode *parent = NULL;

    stack_t *st = st_create(sizeof(RopeTree));
	split_tree(copy->root, parent, &idx, st);
	// the left part of the pair remains the tree we modify
    while (!st_is_empty(st)) {
        // RopeTree *tree = copyrt((RopeTree *)st_peek(st));
        RopeTree *tree = (RopeTree *)st_peek(st);
        RopeTree* tmp = r_right;
        r_right = concat(r_right, tree);
        free(tmp);
        st_pop(st);
    }
	pair.left = copy->root;
	// if we found something to cut we have to free the memory
	if (r_right) {
		pair.right = r_right->root;
		// free_tree(r_right);
	} else {
		pair.right = NULL;
	}
	free(copy);
    free(r_right);
    st_free(st);
    recalculate_weight(pair.left);
	return pair;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str) {
	// split the tree at idx
	SplitPair pair = split(rt, idx);
	RopeTree *rtleft = makeRopeTree(pair.left);
	RopeNode *new_node = makeRopeNode(str);
	RopeTree *rtadded = makeRopeTree(new_node);
	RopeTree *rtright = makeRopeTree(pair.right);
	RopeTree* rt1 = concat(rtadded, rtright);
	RopeTree* rt2 = concat(rtleft, rt1);

	free(rtleft);
	free(rtadded);
	free(rt1);
	free(rtright);
	return rt2;
}

RopeTree* delete(RopeTree* rt, int start, int len) {
	SplitPair pair1 = split(rt, start + len);
	SplitPair pair2 = split(rt, start);
	RopeTree *rtright = makeRopeTree(pair1.right);
	RopeTree *rtleft = makeRopeTree(pair2.left);
	RopeTree* rt1 = concat(rtleft, rtright);

    free(rtleft);
    free(rtright);
	return rt1;
}
