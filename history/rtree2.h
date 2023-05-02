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
