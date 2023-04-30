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

//clone key
key * cloneKey(key * oldkey){
    key * newkey = (key *)malloc(sizeof(key));
    newkey->child=oldkey->child;
    newkey->rect=oldkey->rect;
    return newkey;
}

// Pick Seeds
void print(key **keys){
     for(int i=0;i<M;i++){
        int x1,y1,x2,y2;
        if(keys[i]==NULL) break;
        x1=keys[i]->rect.x1;
        y1=keys[i]->rect.y1,
        x2=keys[i]->rect.x2,
        y2=keys[i]->rect.y2;
        printf(" %d %d %d %d \n",x1,y1,x2,y2);
    }
printf("\n\n");
}

node *createNode(node *parent)
{
    node *n = (node *)malloc(sizeof(node));
    n->numKeys = 0;
    n->keys = (key **)malloc(sizeof(key *) * M);
    n->isLeaf = true;
    n->parentKey = NULL;
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

int count=0;
void preOrderTraversal(node *root)
{
    if (root == NULL)
        return;
    if (root->isLeaf == true)
    {   
        // print(root->keys);
        count+=root->numKeys;
        printf("Leaf Node\n");
        for (int i = 0; i < root->numKeys; i++)
        {
            
            printf("\tBottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->keys[i]->rect.x1, root->keys[i]->rect.y1, root->keys[i]->rect.x2, root->keys[i]->rect.y2);
        }
    }

    // print(root->keys);

    for (int i = 0; i < root->numKeys; i++)
    {

        preOrderTraversal(root->keys[i]->child);
    }
    
    if (root->isLeaf == true)
        printf("\n\tMBR of Leaf Node is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
    else
      {  printf("\nMBR of Internal is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
       if(root->parent==NULL) printf("Total Leaf Nodes keys :%d\n",count);
      }
}

//complete this
// void preOrderTraversal(node *root)
// {
//     if (root == NULL)
//         return;
//     if (root->isLeaf == true)
//     {   
//         // print(root->keys);
//         count+=root->numKeys;
//         printf("Leaf Node\n");
//         for (int i = 0; i < root->numKeys; i++)
//         {
            
//             printf("\tBottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->keys[i]->rect.x1, root->keys[i]->rect.y1, root->keys[i]->rect.x2, root->keys[i]->rect.y2);
//         }
//     }

//     // print(root->keys);
//     printf("\nMBR of Internal is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
//     for (int i = 0; i < root->numKeys; i++)
//     {
        
       
//         preOrderTraversal(root->keys[i]->child);
//     }
    
//     // if (root->isLeaf == true)
//     //     printf("\n\tMBR of Leaf Node is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
//     // else
//     //   { 
       
//     if(root->parent==NULL) printf("Total Leaf Nodes keys :%d\n",count);
//     //   }
// }


int area(rectangle rect)
{
    return ((rect.x2 - rect.x1) * (rect.y2 - rect.y1));
}

int incresedArea(rectangle parent, rectangle child)
{
    int x1 = MIN(parent.x1, child.x1);
    int x2 = MAX(parent.x2, child.x2);
    int y1 = MIN(parent.y1, child.y1);
    int y2 = MAX(parent.y2, child.y2);
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

node * createKey(node *Node, rectangle rect)
{
    if (Node->numKeys < M)
    {
        int index = Node->numKeys++;
        Node->keys[index]->rect = rect;
        Node->keys[index]->child = NULL;
        updateMBR(Node);
        return Node;
    }
    else
    {
        // Node split
        key * newkey = (key *) malloc(sizeof(key));
        newkey->rect=rect;
        newkey->child=NULL;
        node * insertedNode = quadraticSplit(Node,newkey);
        updateMBR(Node);
        return insertedNode;
    }
}


// Quadratic cost algorithm
node * quadraticSplit(node *Node,key * newkey)
{
    int keysLeft = M+1;
    key **keys = (key **)malloc(sizeof(key *) * keysLeft);
    keys[0]=newkey;
    for(int i=1;i<keysLeft;i++){
        keys[i]= Node->keys[i-1];
        // Node->keys[i-1]=NULL;
    }
    // print(keys);
    // printf("\n\n");
    int *seeds = pickSeeds(Node,keys);
    // int c = seeds[0];
    // int d = seeds[1];

    node *newNode0 = createNode(Node);

    // createKey(newNode0, keys[seeds[0]]->rect);
    newNode0->keys[0] = cloneKey(keys[seeds[0]]);
    node *newNode1 = createNode(Node);
    // createKey(newNode1, keys[seeds[1]]->rect);
    newNode1->keys[0] = cloneKey(keys[seeds[1]]);
    
    if(Node->isLeaf==false) {
        newNode0->isLeaf=newNode1->isLeaf=false;
        }
    
    
    keys[seeds[0]] = NULL;
    keys[seeds[1]] = NULL;


    //update MBR
    newNode0->MBR =newNode0->keys[0]->rect;
    newNode1->MBR =newNode1->keys[0]->rect;
    
    keysLeft -= 2;
    int i,j;
    i=j=1;

    int c = 0;
    while (keysLeft > 0)
    { 


        int *keyAndGroup = pickNext(Node, newNode0, newNode1, keys);
        int a = keyAndGroup[0];
        int b = keyAndGroup[1];

        if ( (keyAndGroup[0] == 0 && keysLeft>(m-newNode1->numKeys)) || keysLeft<=(m-newNode0->numKeys) ) 
        {
            newNode0->keys[i] = cloneKey(keys[keyAndGroup[1]]);
            //update MBR of both nodes after each iteration
            rectangle newMBR0 = {
                    MIN(newNode0->MBR.x1, keys[keyAndGroup[1]]->rect.x1),
                    MAX(newNode0->MBR.x2, keys[keyAndGroup[1]]->rect.x2),
                    MIN(newNode0->MBR.y1, keys[keyAndGroup[1]]->rect.y1),
                    MAX(newNode0->MBR.y2, keys[keyAndGroup[1]]->rect.y2)};
            
            newNode0->MBR =newMBR0;
          // printf("NewNode x:%d y:%d \n",newNode0->keys[i]->rect.x1,newNode0->keys[i]->rect.y1);
            newNode0->numKeys=++i;
            
        }
        else
        {  
           
            newNode1->keys[j] =  cloneKey(keys[keyAndGroup[1]]);
            rectangle newMBR1 = {
                MIN(newNode1->MBR.x1,keys[keyAndGroup[1]]->rect.x1),
                MAX(newNode1->MBR.x2,keys[keyAndGroup[1]]->rect.x2),
                MIN(newNode1->MBR.y1,keys[keyAndGroup[1]]->rect.y1),
                MAX(newNode1->MBR.y2,keys[keyAndGroup[1]]->rect.y2)};

        newNode1->MBR =newMBR1;
        newNode1->numKeys=++j;
        
        }
        // set key in orignal node to null so it's removed in next iteration
        keys[keyAndGroup[1]] = NULL;
        keysLeft--;
        if (keyAndGroup[1] == 0){
            c = keyAndGroup[0];
        }

    }
    // printf("\nNode 1 keys:%d\n",newNode0->numKeys);
   

    // return split nodes
    


    //updating the new parents of splitted nodes
    if(Node->parent==NULL) // if node was root, need to create a new node
    {   
        // node * parent = createNode(NULL);
        newNode0->parent=Node;
        newNode1->parent =Node;

        Node->keys[0]->rect=newNode0->MBR;
        Node->keys[0]->child=newNode0;

        Node->keys[1]->rect=newNode1->MBR;
        Node->keys[1]->child=newNode1;

        newNode0->parentKey = Node->keys[0];
        newNode1->parentKey = Node->keys[1];
        
        Node->isLeaf=false;
        Node->numKeys=2;

        Node->keys[2]=NULL;
        Node->keys[3]=NULL;
        updateMBR(Node);

    }
    else{ // if node was not root, propgate above

        //to be handeled

        newNode1->parent =newNode0->parent = Node->parent;
        //check ??
        if(newNode0->keys[0]->child!=NULL){
            newNode0->isLeaf=false;
        }
        if(newNode1->keys[0]->child!=NULL){
            newNode1->isLeaf=false;
        }

        //this part is BT as upper split changes Node underneath node addresses

        newNode0->parentKey = Node->parentKey;
        newNode0->parentKey->rect = newNode0->MBR;
        //does it mispoint in parent call?
        newNode0->parentKey->child = newNode0;
        newNode0->parent->isLeaf = false;
        updateMBR(newNode0->parent);

        // create new key for second splitted node
        //susss
        key * newkey = (key *) malloc(sizeof(key));
        newkey->rect=newNode1->MBR;
        newkey->child=newNode1;
        node * insertedNode = insertKeyInNode(newNode0->parent,newkey);
        

        newNode1->parent = insertedNode;
        
        newNode1->parentKey = insertedNode->keys[(insertedNode->numKeys)-1];
        newNode1->parent->isLeaf = false;

       
        /// /newNode1->parent = (node *)newNode0->parent->keys[newNode0->parent->numKeys];

     //this line not req ? as child of node 1 already set before passing new key in INSERTkeyinnode ?
        // insertedNode->keys[(insertedNode->numKeys)-1]->child = newNode1;
      
        updateMBR(newNode1->parent);            
    }    

    if (c==0) {
        return newNode0;
    } else{
        return newNode1;
    }
    // print(newNode0->keys);
    // printf("\nNode id  keys:%d\n",newNode1->numKeys);
    // print(newNode1->keys);

    // free(Node->keys);
    // free(keys);
    // free(Node);

}


int * pickSeeds(node *node,key ** keys)
{   
   
    int *seeds = malloc( sizeof(int)*2 );

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
                seeds[0] = i;
                seeds[1] = j;
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
    //iterating over all keys
    for (int i = 0; i < M+1; i++)
    {
      
            // if the node is NULL in orign..
            if (keys[i] == NULL )
                continue;

            rectangle newMBR = {
                MIN(newNode0->MBR.x1, keys[i]->rect.x1),
                MAX(newNode0->MBR.x2, keys[i]->rect.x2),
                MIN(newNode0->MBR.y1, keys[i]->rect.y1),
                MAX(newNode0->MBR.y2, keys[i]->rect.y2)};

            int wasteArea1 = (newMBR.x2 - newMBR.x1) * (newMBR.y2 - newMBR.y1) -
                            (newNode0->MBR.x2 - newNode0->MBR.x1) * (newNode0->MBR.y2 - newNode0->MBR.y1);

            rectangle newMBR1 = {
                MIN(newNode1->MBR.x1,keys[i]->rect.x1),
                MAX(newNode1->MBR.x2,keys[i]->rect.x2),
                MIN(newNode1->MBR.y1,keys[i]->rect.y1),
                MAX(newNode1->MBR.y2,keys[i]->rect.y2)};

            int wasteArea2 = (newMBR1.x2 - newMBR1.x1) * (newMBR1.y2 - newMBR1.y1) -
                             (newNode1->MBR.x2 - newNode1->MBR.x1) * (newNode1->MBR.y2 - newNode1->MBR.y1);

            if (abs(wasteArea2 - wasteArea1) >= wasteAreaMax)
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

    return returnArray;
}



node * insertKeyInNode(node *Node, key * key)
{
    if (Node->numKeys < M)
    {
        int index = Node->numKeys++;
        Node->keys[index] = key;
        // Node->keys[index]->child = NULL;
        updateMBR(Node);
        return Node;
    }
    else
    {
        // Node split
        // key * newkey = (key *) malloc(sizeof(key));
        // newkey->rect=rect;
        // newkey->child=NULL;
        node * insertedNode = quadraticSplit(Node, key);
        updateMBR(Node);
        return insertedNode;
    }
}

void insertkey(rTree *tree, rectangle rect)
{
    if (tree->root == NULL)
    {
        tree->root = createNode(NULL);
    }

    node *Node = ChooseLeaf(tree->root, rect);

    key * newkey = (key *) malloc(sizeof(key));
    newkey->rect=rect;
    newkey->child=NULL;

    insertKeyInNode(Node, newkey);
}

void updateMBR(node *node)
{
    if (node == NULL)
    {
        return;
    }
    rectangle newMBR=node->keys[0]->rect;

    for(int i=0;i<node->numKeys;i++){
        newMBR.x1 = MIN(newMBR.x1, node->keys[i]->rect.x1);
        newMBR.x2 = MAX(newMBR.x2, node->keys[i]->rect.x2);
        newMBR.y1 = MIN(newMBR.y1, node->keys[i]->rect.y1);
        newMBR.y2 = MAX(newMBR.y2, node->keys[i]->rect.y2);
    }
    node->MBR=newMBR;
    
    if (node->parentKey != NULL){
        node->parentKey->rect = newMBR;
    }

    updateMBR(node->parent);
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
    //105000
    while (fscanf(fp, "%d %d\n", &x, &y) == 2 && k < 17)
    {//17 ke upar repetition 
        rectangle rect = {x, x, y, y};
        insertkey(tree, rect);
        k++;
    };
    printf("\n\n Starting New Traverssal..\n");
    preOrderTraversal(tree->root);

    return 0;
}


//ye kaha ka preOrder hai BC... tf