#define M 4
#define m 2
#define MIN(a, b) ((a) <= (b) ? (a) : (b))
#define MAX(a, b) ((a) >= (b) ? (a) : (b))

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

node *createNode(node *parent);
rTree *createTree();
void preOrderTraversal(node *root);
int area(rectangle rect);
int incresedArea(rectangle parent, rectangle child);
node *ChooseLeaf(node *root, rectangle child);
node * quadraticSplit(node *Node,key * newkey);
int *pickSeeds(node *node,key ** keys);
int *pickNext(node *Node, node *newNode0, node *newNode1,key ** keys);

node * createKey(node *node, rectangle rect);
void insertkey(rTree *root, rectangle rect);
void updateMBR(node *node);
