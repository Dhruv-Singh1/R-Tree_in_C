#define M 4
#define m 2
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))
// #define int long long int
typedef struct node node;
typedef struct key key;

typedef struct
{
    int x1;
    int x2;
    int y1;
    int y2;
} rectangle;

struct key
{
    rectangle rect;
    node *child;
};

struct node
{
    rectangle MBR;
    key **keys;
    int numKeys;
    bool isLeaf;
    node *parent;
    key *parentKey;
};

typedef struct
{
    node *root;
    int height;
} rTree;

// typedef struct 
// {
//     int splitedNo;
//     node ** splitNode;
// } splitNodes;


node *createNode(node *parent);
rTree *createTree();
long long int increasedArea(rectangle parent, rectangle child);
void preOrderTraversal(node* curNode, int level);
int area(rectangle rect);
node *ChooseLeaf(node *root, rectangle child);
splitNodes * quadraticSplit(node *Node,key * newkey);
int *pickSeeds(node *node,key ** keys);
int *pickNext(node *Node, node *newNode0, node *newNode1,key ** keys);

splitNodes *insertKeyInNode(node *node, key * key);
void insertkey(rTree *root, rectangle rect);
void updateMBR(node *node);
