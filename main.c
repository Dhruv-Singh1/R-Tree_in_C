#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>
// Constant definitions
#define M 4
#define m 2
#define MIN(a,b) ((a) <= (b) ? (a) : (b))
#define MAX(a,b) ((a) >= (b) ? (a) : (b))

typedef struct  {
    int x1;
    int x2;
    int y1;
    int y2;
} dimensions;

typedef struct {
    dimensions dim;
    node * child;
} key;

typedef struct {
  dimensions MCR;
  key **keys;
  int numKeys;
  bool isLeaf;
} node;

typedef struct  {
    node * root;
    int height;
} rTree ;

// new node is assumed to be a leaf
node * createNode(dimensions MCR){
    node * n=(node*)malloc(sizeof(node));
    n->MCR=MCR;
    n->numKeys=0;
    n->keys=(key **)malloc(sizeof(key *)*M);
    for(int i=0; i<M; i++){
        n->keys[i] = (key *)malloc(sizeof(key));
    }
    return n;
}

rTree * createTree() {
    rTree * rtree = (rTree *)malloc(sizeof(rTree));
    rtree->height = 0;
    rtree->root=NULL;
    return rtree;
}
int area(dimensions dim){
    return ((dim.x2-dim.x1)*(dim.y2-dim.y1));
}
int incresedArea(dimensions parent,dimensions child){
   int x1=MIN(parent.x1,child.x1);
   int x2=MAX(parent.x2,child.x2);
   int y1=MIN(parent.y1,child.y1);
   int y2=MIN(parent.y2,child.y2);
   return (area((dimensions){x1,x2,y1,y2})-area(parent));
}

node *ChooseLeaf(node* root,dimensions child){
  if(root->isLeaf==true)
      {  return root;}

  int increase1=INT_MAX;
  node *best_leaf=NULL;
  for(int i=0;i<root->numKeys;i++){
     int increase2=incresedArea(root->keys[i]->dim,child);
     if(increase2<increase1){
       best_leaf=root->keys[i]->child;
       increase1=increase2;
     }
  }
   return ChooseLeaf(best_leaf,child);
}



void insertkey(node *node,dimensions dims){
   if(node->numKeys<M)
   {
    key *k=node->keys[node->numKeys++];
    k->dim=dims;
    updateMCR(node *node,dimensions dims);
   }
   else{
    
   }
   int index=(node->numKeys)++;
   node->keys[index]->dim=dims;
   node->keys[index]->child=NULL;
}

void insertnode(rTree * tree,dimensions dims){
    if(tree->root==NULL){
        tree->root=createNode(dims);
        insertkey(tree->root,dims);
    }
    else{
        node *node=ChooseLeaf(tree->root,dims);
        insertkey(node,dims);
    }
}


//Quadratic cost algorithm
void quadraticSplit(node * Node){
    int keysLeft= Node->isLeaf +1;
    key ** seeds = (key **)malloc(sizeof(key *)*2);
    seeds = pickSeeds(Node);
    node * newNode0 = createNode((dimensions){seeds[0]->dim.x1, seeds[0]->dim.x2, seeds[0]->dim.y1, seeds[0]->dim.y2});
    node * newNode1 = createNode((dimensions){seeds[1]->dim.x1, seeds[1]->dim.x2, seeds[1]->dim.y1, seeds[1]->dim.y2});

    newNode0->keys[0] = seeds[0];
    newNode1->keys[0] = seeds[1];
    keysLeft-=2;
    while(keysLeft>0){
        int * keyAndGroup = pickNext(Node);
        if(keyAndGroup[0]==0){ 
            newNode0->keys[keyAndGroup[1]]=Node->keys[keyAndGroup[1]];
        }else{
            newNode1->keys[keyAndGroup[1]]=Node->keys[keyAndGroup[1]];   
        }  
    }
}

//Pick Seeds
key ** pickSeeds(node * node){
    key ** seeds = (key **)malloc(sizeof(key *)*2);
    seeds[0] = (key *)malloc(sizeof(key));
    seeds[1] = (key *)malloc(sizeof(key));

    int wasteAreaMax = 0;
    
    for(int i=0; i<node->numKeys; i++){
        for(int j=0; j<node->numKeys; j++){
            dimensions max = {
                MAX(node->keys[i]->dim.x1, node->keys[j]->dim.x1), 
                MAX(node->keys[i]->dim.x2, node->keys[j]->dim.x2),
                MAX(node->keys[i]->dim.y1, node->keys[j]->dim.y1),
                MAX(node->keys[i]->dim.y2, node->keys[j]->dim.y2)
            };

            int wasteArea = (max.x2-max.x1)*(max.y2-max.y1);
            if (wasteArea>wasteAreaMax){
                wasteAreaMax = wasteArea;
                seeds[0]=node->keys[i];
                seeds[1]=node->keys[j];
            }
        }
    }
    return seeds;
}


//Pick Next
key * pickNext(node * node){
    
}

//Choose Leaf
// Split Node
//Adjust Tree
//Minimum Bounded Rectangles
// Node Split



		

		


// Search ?
