#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<limits.h>
#include <stdbool.h>
// Constant definitions


typedef struct node node;
typedef struct key key;

//move them to header files
int * pickNext(node * Node,node * newNode0,node * newNode1);
key ** pickSeeds(node * node);

//macro to handle 2D points
#define dimInit(...) FUNC(__VA_ARGS__,-1,-1)
#define FUNC(x1,y1,x2,y2, ...) dimInit(x1, y1,x2,y2)

dimensions (dimInit)(int x1,int y1,int x2,int y2)
{
    dimensions dim;
    if(x2==-1&&y2==-1)
    {x2=x1;y2=y1;}
    dim.x1=x1;
    dim.y1=y1;
    dim.x2=x2;
    dim.y2=y2;
    return dim;
}

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

struct key {
    dimensions dim;
    node * child;
};

struct node {
  dimensions MCR;
  key **keys;
  int numKeys;
  bool isLeaf;
  node *parent;
};


typedef struct  {
    node * root;
    int height;
} rTree ;


// new node is assumed to be a leaf
node * createNode(dimensions MCR,node *parent){
    node * n=(node*)malloc(sizeof(node));
    n->MCR=MCR;
    n->numKeys=0;
    n->keys=(key **)malloc(sizeof(key *)*M);
    n->isLeaf=true;

    for(int i=0; i<M; i++){
        n->keys[i] = (key *)malloc(sizeof(key));
    }
    n->parent=parent;
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


void updateMCR(node *node,dimensions dims){
 node->MCR.x1=MIN(node->MCR.x1,dims.x1);
  node->MCR.x1=MAX(node->MCR.x2,dims.x2);
 node->MCR.x1=MIN(node->MCR.y1,dims.y1);
 node->MCR.x1=MAX(node->MCR.y2,dims.y2);
 if(node->parent==NULL)
 return;
 else
 updateMCR(node->parent,dims);
}

void insertkey(rTree * root,dimensions dims){
    node * Node=ChooseLeaf(root->root,dims);
    Node->keys[1]->dim=dims;
    // printf("index %d ",Node->numKeys);
    if(Node->numKeys<M)
    {
        int index=Node->numKeys++;
        printf(" index %d \n",index);
        Node->keys[index]->dim=dims;
        Node->keys[index]->child=NULL;
        updateMCR(Node,dims);
    }
    else{
       
    }
     
     
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
    int keysLeft= M +1;
    key ** seeds = (key **)malloc(sizeof(key *)*2);
    seeds = pickSeeds(Node);
    node * newNode0 = createNode((dimensions){seeds[0]->dim.x1, seeds[0]->dim.x2, seeds[0]->dim.y1, seeds[0]->dim.y2});
    node * newNode1 = createNode((dimensions){seeds[1]->dim.x1, seeds[1]->dim.x2, seeds[1]->dim.y1, seeds[1]->dim.y2});

    newNode0->keys[0] = seeds[0];
    newNode1->keys[0] = seeds[1];
    keysLeft-=2;
    while(keysLeft>0){
        int * keyAndGroup = pickNext(Node,newNode0,newNode1);
        if(keyAndGroup[0]==0){ 
            newNode0->keys[keyAndGroup[1]]=Node->keys[keyAndGroup[1]];
        }else{
            newNode1->keys[keyAndGroup[1]]=Node->keys[keyAndGroup[1]];   
        }  
        //set key in orignal node to null so it's removed in next iteration
        Node->keys[keyAndGroup[1]]=NULL;
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

            int wasteArea = (max.x2-max.x1)*(max.y2-max.y1) - 
            (node->keys[i]->dim.x2-node->keys[i]->dim.x1)*(node->keys[i]->dim.y2-node->keys[i]->dim.y1);
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
int * pickNext(node * Node,node * newNode0,node * newNode1){
    int wasteAreaMax = 0;
    int * returnArray= (int * )malloc(sizeof(int )*2);

    for(int i=0; i<Node->numKeys; i++){
        for(int j=i; j<Node->numKeys; j++){
            // if the node is NULL in orign..
            if(Node->keys[i]==NULL || Node->keys[j]==NULL) continue;

            dimensions max0 = {
                MAX(newNode0->MCR.x1, Node->keys[i]->dim.x1), 
                MAX(newNode0->MCR.x2, Node->keys[i]->dim.x2),
                MAX(newNode0->MCR.y1, Node->keys[i]->dim.y1),
                MAX(newNode0->MCR.y2, Node->keys[i]->dim.y2)
            };

            int wasteArea1 = (max0.x2-max0.x1)*(max0.y2-max0.y1)- 
            (newNode0->keys[i]->dim.x2-newNode0->MCR.x1)*(newNode0->MCR.y2-newNode0->MCR.y1);

            dimensions max1 = {
                MAX(newNode1->MCR.x1, Node->keys[i]->dim.x1), 
                MAX(newNode1->MCR.x2, Node->keys[i]->dim.x2),
                MAX(newNode1->MCR.y1, Node->keys[i]->dim.y1),
                MAX(newNode1->MCR.y2, Node->keys[i]->dim.y2)
            };

            int wasteArea2 = (max1.x2-max1.x1)*(max1.y2-max1.y1)- 
            (newNode1->MCR.x2-newNode1->MCR.x1)*(newNode1->MCR.y2-newNode1->MCR.y1);

            if (abs(wasteArea2-wasteArea1)>wasteAreaMax){
                wasteAreaMax=abs(wasteArea2-wasteArea1);
                if(wasteArea2-wasteArea1<=0){

                    returnArray[0]=0;
                }
                else{
                    returnArray[0]=1;
                }
                returnArray[1]=i;
            }
        }
    }
    return returnArray;

}





int main(int argc, char **argv){
    rTree * rtree= createTree();
    
 
    // dimensions dim2 = {1,3,4,8};
    // dimensions dim3 = {1,3,4,8};
    // dimensions dim4 = {1,3,4,8};
    // dimensions dim5 = {1,3,4,8};
    // dimensions dim1=dimInit(1,3,4,8);
    // rtree->root=createNode(dim1);
    // insertkey(rtree,dim1);
    // insertkey(rtree,dim2);
    // insertkey(rtree,dim3);
    // insertkey(rtree,dim4);
    // insertkey(rtree,dim5);

    return 0;
}

//Choose Leaf
// Split Node
//Adjust Tree
//Minimum Bounded Rectangles
// Node Split
// Search ?
