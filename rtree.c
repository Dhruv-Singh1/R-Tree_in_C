#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdbool.h>
// Constant definitions
#include "rtree.h"


// macro to handle 2D points
#define dimInit(...) FUNC(__VA_ARGS__, -1, -1)
#define FUNC(x1, y1, x2, y2, ...) dimInit(x1, y1, x2, y2)

node *createNode(node *parent)
{
    node *n = (node *)malloc(sizeof(node));
    n->numKeys = 0;
    n->keys = (key **)malloc(sizeof(key *) * M);
    n->isLeaf = true;

    for (int i = 0; i < M; i++)
    {
        n->keys[i] = (key *)malloc(sizeof(key));
    }
    n->parent = parent;
    return n;
}

rTree *createTree()
{
    rTree *rtree = (rTree *)malloc(sizeof(rTree));
    rtree->height = 0;
    rtree->root = NULL;
    return rtree;
}

void preOrderTraversal(node *root)
{
    if (root == NULL)
        return;
    if (root->isLeaf == true)
    {
        printf("External (Leaf Node)\n");
        for (int i = 0; i < root->numKeys; i++)
        {
            printf(" Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->keys[i]->rect.x1, root->keys[i]->rect.y1, root->keys[i]->rect.x2, root->keys[i]->rect.y2);
        }
    }

    for (int i = 0; i < root->numKeys; i++)
    {
        preOrderTraversal(root->keys[i]->child);
    }
    if (root->isLeaf == true)
        printf("\nMBR of External (Leaf Node) is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
    else
        printf("\nMBR of Internal (Non Leaf Node) is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
}

int area(rectangle rect)
{
    return ((rect.x2 - rect.x1) * (rect.y2 - rect.y1));
}

int incresedArea(rectangle parent, rectangle child)
{
    int x1 = MIN(parent.x1, child.x1);
    int x2 = MAX(parent.x2, child.x2);
    int y1 = MIN(parent.y1, child.y1);
    int y2 = MIN(parent.y2, child.y2);
    return (area((rectangle){x1, x2, y1, y2}) - area(parent));
}

node *ChooseLeaf(node *root, rectangle child)
{
    if (root->isLeaf == true)
    {
        return root;
    }

    int increase1 = INT_MAX;
    node *best_leaf = NULL;
    for (int i = 0; i < root->numKeys; i++)
    {
        int increase2 = incresedArea(root->keys[i]->rect, child);
        if (increase2 < increase1)
        {
            best_leaf = root->keys[i]->child;
            increase1 = increase2;
        }
    }
    return ChooseLeaf(best_leaf, child);
}

// Quadratic cost algorithm
void quadraticSplit(node *Node,key * newkey)
{
    int keysLeft = M+1;
    key **keys = (key **)malloc(sizeof(key *) * keysLeft);
    keys[0]=newkey;
    for(int i=1;i<keysLeft;i++){
        keys[i]= Node->keys[i-1];

    }

    key **seeds = pickSeeds(Node,keys);

    node *newNode0 = createNode(Node);
    createKey(newNode0, seeds[0]->rect);
    node *newNode1 = createNode(Node);
    createKey(newNode1, seeds[1]->rect);

    // newNode0->keys[0] = seeds[0];
    // newNode1->keys[0] = seeds[1];
    keysLeft -= 2;
    while (keysLeft > 0)
    {
        int *keyAndGroup = pickNext(Node, newNode0, newNode1, keys);
        if (keyAndGroup[0] == 0)
        {
            newNode0->keys[keyAndGroup[1]] = keys[keyAndGroup[1]];
        }
        else
        {
            newNode1->keys[keyAndGroup[1]] = keys[keyAndGroup[1]];
        }

        // set key in orignal node to null so it's removed in next iteration
        keys[keyAndGroup[1]] = NULL;
        keysLeft--;
    }

    // updating the new parents of splitted nodes
    newNode0->parent = Node->parent;
    // create new key for second splitted node
    createKey(Node->parent, newNode1->MBR);
    newNode1->parent = (node *)newNode0->parent->keys[newNode0->parent->numKeys];

    free(Node->keys);
    free(keys);
    free(Node);
}

// Pick Seeds
void print(key **keys){
     for(int i=1;i<M+1;i++){
        int x1=keys[i]->rect.x1,
        y1=keys[i]->rect.y1,
        x2=keys[i]->rect.x2,
        y2=keys[i]->rect.y2;
        printf("i%d cord%d %d %d %d ",i,x1,y1,x2,y2);
    }
}
key **pickSeeds(node *node,key ** keys)
{   
    print(keys);
    key **seeds = malloc( sizeof(key *)*2 );
    seeds[0] = (key *)malloc(sizeof(key));
    seeds[1] = (key *)malloc(sizeof(key));

    int wasteAreaMax = 0;

    for (int i = 0; i < M+1; i++)
    {
        for (int j = 0; j < M+1; j++)
        {
            rectangle max = {
                MIN(keys[i]->rect.x1, keys[j]->rect.x1),
                MAX(keys[i]->rect.x2, keys[j]->rect.x2),
                MIN(keys[i]->rect.y1, keys[j]->rect.y1),
                MAX(keys[i]->rect.y2, keys[j]->rect.y2)};

            int wasteArea = abs(abs((max.x2 - max.x1) * (max.y2 - max.y1)) -
                            abs((keys[i]->rect.x2 - keys[i]->rect.x1) * (keys[i]->rect.y2 - keys[i]->rect.y1)));
            if (abs(wasteArea) > wasteAreaMax)
            {
                wasteAreaMax = abs(wasteArea);
                seeds[0] = keys[i];
                seeds[1] = node->keys[j];
            }
        }
    }
    return seeds;
}

// Pick Next
int *pickNext(node *Node, node *newNode0, node *newNode1,key ** keys)
{
    int wasteAreaMax = 0;
    int *returnArray = (int *)malloc(sizeof(int) * 2);

    for (int i = 0; i < M+1; i++)
    {
        for (int j = i; j < M+1; j++)
        {
            // if the node is NULL in orign..
            if (keys[i] == NULL || keys[j] == NULL)
                continue;

            rectangle max0 = {
                MIN(newNode0->MBR.x1, keys[i]->rect.x1),
                MAX(newNode0->MBR.x2, keys[i]->rect.x2),
                MIN(newNode0->MBR.y1, keys[i]->rect.y1),
                MAX(newNode0->MBR.y2, keys[i]->rect.y2)};

            int wasteArea1 = (max0.x2 - max0.x1) * (max0.y2 - max0.y1) -
                             (newNode0->keys[i]->rect.x2 - newNode0->MBR.x1) * (newNode0->MBR.y2 - newNode0->MBR.y1);

            rectangle max1 = {
                MAX(newNode1->MBR.x1,keys[i]->rect.x1),
                MAX(newNode1->MBR.x2,keys[i]->rect.x2),
                MAX(newNode1->MBR.y1,keys[i]->rect.y1),
                MAX(newNode1->MBR.y2,keys[i]->rect.y2)};

            int wasteArea2 = (max1.x2 - max1.x1) * (max1.y2 - max1.y1) -
                             (newNode1->MBR.x2 - newNode1->MBR.x1) * (newNode1->MBR.y2 - newNode1->MBR.y1);

            if (abs(wasteArea2 - wasteArea1) > wasteAreaMax)
            {
                wasteAreaMax = abs(wasteArea2 - wasteArea1);
                if (wasteArea2 - wasteArea1 <= 0)
                {

                    returnArray[0] = 0;
                }
                else
                {
                    returnArray[0] = 1;
                }
                returnArray[1] = i;
            }
        }
    }

    return returnArray;
}

void createKey(node *node, rectangle rect)
{
    if (node->numKeys < M)
    {
        int index = node->numKeys++;
        node->keys[index]->rect = rect;
        node->keys[index]->child = NULL;
        updateMBR(node, rect);
    }
    else
    {
        // Node split
        key * newkey = (key *) malloc(sizeof(key));
        newkey->rect=rect;
        newkey->child=NULL;
        quadraticSplit(node,newkey);
    }
}

void insertkey(rTree *tree, rectangle rect)
{
    if (tree->root == NULL)
    {
        tree->root = createNode(NULL);
    }

    node *Node = ChooseLeaf(tree->root, rect);
    createKey(Node, rect);
}

void updateMBR(node *node, rectangle rect)
{
    if (node == NULL)
    {
        return;
    }
    if (node->numKeys == 1)
    {
        node->MBR = rect;
    }
    else
    {
        node->MBR.x1 = MIN(node->MBR.x1, rect.x1);
        node->MBR.x2 = MAX(node->MBR.x2, rect.x2);
        node->MBR.y1 = MIN(node->MBR.y1, rect.y1);
        node->MBR.y2 = MAX(node->MBR.y2, rect.y2);
    }
    updateMBR(node->parent, rect);
}

int main()
{
    // rTree *tree = (rTree *)malloc(sizeof(rTree));
    // tree->root = (node *)malloc(sizeof(node));

    // tree->root->isLeaf = true;
    // tree->root->parent = NULL;
    // tree->root->numKeys = 0;
    // tree->root->MBR.x1 = 0;
    // tree->root->MBR.y1 = 0;
    // tree->root->MBR.x2 = 0;
    // tree->root->MBR.y2 = 2;
    // tree->root->keys = (key **)malloc(sizeof(key *) * M);
    // for (int i = 0; i < M; i++)
    // {
    //     tree->root->keys[i] = (key *)malloc(sizeof(key));
    // }

    rTree *tree = createTree();
    FILE *fp;
    fp = fopen("data.txt", "r");
    if (fp == NULL)
    {
        printf("Error occured while reading the file");
    }
    int x, y;
    // fscanf(fp, "%d %d\n", &x, &y);
    // printf("%d %d", x, y);
    int k = 0;
    while (fscanf(fp, "%d %d\n", &x, &y) == 2 && k < 5)
    {
        rectangle rect = {x, x, y, y};
        insertkey(tree, rect);
        k++;
    };
    preOrderTraversal(tree->root);

    return 0;
}