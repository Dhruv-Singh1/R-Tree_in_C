#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdbool.h>

#define M 4
#define m 2
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
// #define int long long int
typedef struct node node;

typedef struct {
    long long int x1;
    long long  int x2;
    long long int y1;
    long long int y2;
} rectangle;



struct node {
    rectangle MBR;
    node *keys[M];
    int numKeys;
    node *parent;
};

typedef struct {
    node *root;
} rTree;


node * createNode();
rTree *createTree();
bool isLeaf(node * Node);
void preOrderTraversal(node* curNode, int level);
long long int area(rectangle rect);
long long int increasedArea(rectangle parent, rectangle child);
node *ChooseLeaf(node *root, rectangle child);
node** quadraticSplit(node *Node,node * newkey);
int * pickSeeds(node *keys[]);
int* pickNext(node *newNode0, node *newNode1 ,node *keys[]);
void insertKey(rTree *root, rectangle rect);
void updateMBR(node *node);

// Create Tree
rTree *createTree() {
    rTree *rtree = (rTree *)malloc(sizeof(rTree));
    rtree->root = NULL;
    return rtree;
}

// Create Node
node *createNode() {
    node *n = (node *)malloc(sizeof(node));
    n->numKeys = 0;
    n->parent = NULL;
    return n;
}

//calculate MBR of two rectangles
rectangle calculateMBR(rectangle a, rectangle b) {
    rectangle rec=  {
                MIN(a.x1, b.x1),
                MAX(a.x2, b.x2),
                MIN(a.y1, b.y1),
                MAX(a.y2, b.y2)};
    return rec;
}

//updates MBR of all Parent Nodes
void updateMBR(node * Node){
    if (Node == NULL) return;
    
    rectangle newMBR=Node->keys[0]->MBR;

    for(int i=1;i<Node->numKeys;i++){
        newMBR.x1 = MIN(newMBR.x1, Node->keys[i]->MBR.x1);
        newMBR.x2 = MAX(newMBR.x2, Node->keys[i]->MBR.x2);
        newMBR.y1 = MIN(newMBR.y1, Node->keys[i]->MBR.y1);
        newMBR.y2 = MAX(newMBR.y2, Node->keys[i]->MBR.y2);
    }
    Node->MBR=newMBR;
    updateMBR(Node->parent);
}


bool isLeaf(node * Node) {
    for( int i = 0; i< Node->numKeys; i++){
        if (Node->keys[i]->numKeys != 0){
            return false;
        }
    }
    return true;
}

//area
long long int area(rectangle rect)
{
    return ((rect.x2 - rect.x1) * (rect.y2 - rect.y1));
}

//increased area
long long int increasedArea(rectangle parent, rectangle child)
{
    int x1 = MIN(parent.x1, child.x1);
    int x2 = MAX(parent.x2, child.x2);
    int y1 = MIN(parent.y1, child.y1);
    int y2 = MAX(parent.y2, child.y2);
    return (area((rectangle){x1, x2, y1, y2}) - area(parent));
}


long long num = 0;
void preOrderTraversal(node* curNode, int level) {
    // Prints the MBR of the entire root node (all its children).
    if(curNode==NULL||curNode->numKeys==0){
        return;
    }
    if(level == 0) {
        printf("\nMBR of Root Node: [%lld,%lld,%lld,%lld]", curNode->MBR.x1,curNode->MBR.y1,curNode->MBR.x2,curNode->MBR.y2);
    }
    else if(!isLeaf(curNode)){
        printf("\nMBR of Internal Node: [%lld,%lld,%lld,%lld]", curNode->MBR.x1,curNode->MBR.y1,curNode->MBR.x2,curNode->MBR.y2);
    }

    // For each call, prints the DFS depth as well as all children associated with the current node.
    if(isLeaf(curNode)){
        num+=curNode->numKeys;
        printf("\nDFS Level: %d \tExternal Node with %d Keys\n", level, curNode->numKeys);
    }
    else{
        printf("\nDFS Level: %d \tInternal Node with %d Keys\n", level, curNode->numKeys);
    }
    for(int i = 0; i < curNode->numKeys; i++) {
        if( curNode->keys[i]->MBR.x1== curNode->keys[i]->MBR.x2&& curNode->keys[i]->MBR.y1== curNode->keys[i]->MBR.y2)
            {   
                printf("[%lld,%lld]", curNode->keys[i]->MBR.x1,curNode->keys[i]->MBR.y1);
            }
        else            
            printf("[%lld,%lld,%lld,%lld]", curNode->keys[i]->MBR.x1,curNode->keys[i]->MBR.y1,curNode->keys[i]->MBR.x2,curNode->keys[i]->MBR.y2);     
        if(i != curNode->numKeys - 1) printf(" , ");
    } 

    // printf("parent Info parent: %p, ", curNode->parent);
    printf("\n\n");

    // Recursive DFS call to visit children of the current node.
    for(int i = 0; i < curNode->numKeys; i++) {
        preOrderTraversal( curNode->keys[i] , level + 1);
    }
}


node *ChooseLeaf(node *root,rectangle rect){
    node *N=root;
    if(isLeaf(N))
        return N;
    long long int minIncrease=increasedArea(N->keys[0]->MBR,rect);;
    node *bestnode=N->keys[0];
    for(int i=1;i<N->numKeys;i++ ){
        long long int increaseTemp=increasedArea(N->keys[i]->MBR,rect);
        if(increaseTemp<minIncrease){
            bestnode=N->keys[i];
            minIncrease=increaseTemp;
        }
        else if(increaseTemp==minIncrease){
            if(area(N->keys[i]->MBR)<area(bestnode->MBR))
            {
                bestnode=N->keys[i];
                minIncrease=increaseTemp;
            }
        }
    }
    return ChooseLeaf(bestnode,rect);
}


//adjustTree
void  AdjustTree(node * L, node * LL, rTree * tree){
    if(L==NULL)
    return;
    node *parent =L->parent;
    updateMBR(parent);
    if(tree->root==L){
        node *newroot=createNode();
        newroot->keys[0]=L;
        newroot->keys[1]=LL;
        newroot->numKeys=2;
        tree->root=newroot;
        L->parent=newroot;
        LL->parent=newroot;
        updateMBR(newroot);
    }
    else if(parent==NULL){
        return;
    }
    else if(parent->numKeys<M)
    {  
        parent->keys[parent->numKeys++]=LL;
        LL->parent=parent;
        updateMBR(parent);
    }
    else{
        node **splitnodes=quadraticSplit(parent,LL);
        AdjustTree(splitnodes[0],splitnodes[1],tree);         
        updateMBR(splitnodes[0]);
        updateMBR(splitnodes[1]);
    }     
}


void insertKey(rTree *tree,rectangle rect){
    node *root=tree->root;
    if(root==NULL)
    {
        root=createNode();
        tree->root=root;
    }
    node *leaf=ChooseLeaf(root,rect);
    node *newKey=createNode();
    newKey->MBR=rect;
    if(leaf->numKeys<M)
    {
        int index=leaf->numKeys;
        leaf->keys[index]=newKey;
        leaf->numKeys++;
        newKey->parent=leaf;
        updateMBR(leaf);
    }
    else
    {
        node **splitnodes=quadraticSplit(leaf,newKey);       
        AdjustTree(splitnodes[0],splitnodes[1],tree);       
    }
}


int * pickSeeds(node *keys[]){
    int *seeds = (int *)malloc(sizeof(int)*2);
    int wasteAreaMax = 0;
    for (int i = 0; i < M+1; i++) {
        for (int j = 0; j < M+1; j++) {
            if (i==j){
                continue;
            }
            rectangle max = calculateMBR(keys[i]->MBR,keys[j]->MBR);
            long long wasteArea = llabs((max.x2 - max.x1) * (max.y2 - max.y1)) -
                                llabs((keys[i]->MBR.x2 - keys[i]->MBR.x1) * (keys[i]->MBR.y2 - keys[i]->MBR.y1)) -
                                llabs((keys[j]->MBR.x2 - keys[j]->MBR.x1) * (keys[j]->MBR.y2 - keys[j]->MBR.y1));

            if (llabs(wasteArea) > wasteAreaMax)
            {   wasteAreaMax = llabs(wasteArea);
                seeds[0] = i;
                seeds[1] = j;
            }
        }
    }
    return seeds;
}

// Quadratic cost algorithm
node ** quadraticSplit(node *Node,node * newkey) {
    int keysLeft = M+1;
    node *keys[keysLeft];
    keys[0]=newkey;
    for(int i=1;i<keysLeft;i++){
        keys[i]= Node->keys[i-1];
    }

    int * seeds = pickSeeds(keys);

    for (int i = 0; i<Node->numKeys; i++){
        Node->keys[i] = NULL;
    }
    Node->numKeys = 0;
    Node->keys[0] = keys[seeds[0]];
    Node->numKeys++;
    node *newNode1 = createNode();
    newNode1->keys[0] = keys[seeds[1]];
    newNode1->numKeys++;
    keys[seeds[0]] = NULL;
    keys[seeds[1]] = NULL;

    //update MBR
    Node->MBR =Node->keys[0]->MBR;
    newNode1->MBR =newNode1->keys[0]->MBR;
    
    keysLeft -= 2;
    int i,j;
    i=j=1;
    while (keysLeft > 0)
    { 
        int *nextKeyGroupAndPosition = pickNext(Node, newNode1, keys);
        int a = nextKeyGroupAndPosition[0];
        int b = nextKeyGroupAndPosition[1];

        if ( (nextKeyGroupAndPosition[0] == 0 && keysLeft>(m-newNode1->numKeys)) || keysLeft<=(m-Node->numKeys) ) 
        {
            Node->keys[i] = keys[nextKeyGroupAndPosition[1]];
            //update MBR of both nodes after each iteration            
            rectangle newMBR0 =  calculateMBR(Node->MBR,keys[nextKeyGroupAndPosition[1]]->MBR);
            Node->MBR =newMBR0;
            Node->numKeys=++i; 
        }
        else
        {  
            newNode1->keys[j] = keys[nextKeyGroupAndPosition[1]];
            newNode1->keys[j]->parent = newNode1;
            rectangle newMBR1 = calculateMBR(newNode1->MBR,keys[nextKeyGroupAndPosition[1]]->MBR);
            newNode1->MBR =newMBR1;
            newNode1->numKeys=++j;      
        }
        // set key in orignal node to null so it's removed in next iteration
        keys[nextKeyGroupAndPosition[1]] = NULL;
        keysLeft--;
    }
    // return split nodes
    node ** splitNodes = (node **)malloc(sizeof(node *)*2);
    splitNodes[0]=Node;
    splitNodes[1]=newNode1;
    //set parent for all the child nodes
    for (int i = 0; i<splitNodes[0]->numKeys; i++){
        splitNodes[0]->keys[i]->parent = splitNodes[0];
    }
    for (int i = 0; i<splitNodes[1]->numKeys; i++){
        splitNodes[1]->keys[i]->parent = splitNodes[1];
    }
    return splitNodes;
}

// Pick Next
int* pickNext(node *newNode0, node *newNode1 ,node *keys[])
{
    int wasteAreaMax = 0;
    int * nextKeyGroupAndPosition = (int *)malloc(sizeof(int)*2);
    //iterating over all keys
    for (int i = 0; i < M+1; i++)
    {
            // if the node is NULL it is already selected
            if (keys[i] == NULL )
                continue;
            rectangle newMBR = calculateMBR(newNode0->MBR,keys[i]->MBR);
            int wasteArea1 = (newMBR.x2 - newMBR.x1) * (newMBR.y2 - newMBR.y1) -
                            (newNode0->MBR.x2 - newNode0->MBR.x1) * (newNode0->MBR.y2 - newNode0->MBR.y1);

            rectangle newMBR1 = calculateMBR(newNode1->MBR, keys[i]->MBR );

            int wasteArea2 = (newMBR1.x2 - newMBR1.x1) * (newMBR1.y2 - newMBR1.y1) -
                             (newNode1->MBR.x2 - newNode1->MBR.x1) * (newNode1->MBR.y2 - newNode1->MBR.y1);

            if (llabs(wasteArea2 - wasteArea1) >= wasteAreaMax)
            {   wasteAreaMax = llabs(wasteArea2 - wasteArea1);
                nextKeyGroupAndPosition[0] = (wasteArea2 - wasteArea1 <= 0) ? 1 : 0;
                nextKeyGroupAndPosition[1] = i;
            }
    }
    return nextKeyGroupAndPosition;
}

int main(int  argc, char ** argv)
{
    rTree *tree = createTree();
    FILE *fp;
    if(argc==1){
        printf("No file name input from command line..\n");
        fp = fopen("data2.txt", "r");    
    }
    else{

        fp = fopen(argv[1], "r");
    }
    
    if (fp == NULL){ 
        printf("Error occured while reading the file");
    }
    int x, y;
    long long k = 0;
    //105000
    while (fscanf(fp, "%d %d\n", &x, &y) == 2)
    {
        rectangle rect = {x, x, y, y};
        insertKey(tree,rect);
        k++;
    };
    printf("\nStarting New Traversal..\nRectangle format [x1,y1,x2,y2] 2D point format [x1,y1]\n");
     preOrderTraversal(tree->root,0);
    // keysLevel(tree->root,0);
    printf("xNm : %lld", num);
    
    // printf("\n num : %lld", k);
    return 0;
}

