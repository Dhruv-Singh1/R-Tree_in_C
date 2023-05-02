void preorderTraversal(node *root)
{
    if (root == NULL)
        return;
    if (root->isLeaf == true)
    {   
        // print(root->keys);
        count+=root->numKeys;
        printf(" Leaf Node\n");
        for (int i = 0; i < root->numKeys; i++)
        {
            
            //printf("\tBottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n", root->keys[i]->rect.x1, root->keys[i]->rect.y1, root->keys[i]->rect.x2, root->keys[i]->rect.y2);
            printf("%d,%d,%d,%d\n", root->keys[i]->rect.x1, root->keys[i]->rect.y1, root->keys[i]->rect.x2, root->keys[i]->rect.y2);
        }

    }

    // print(root->keys);

    for (int i = 0; i < root->numKeys; i++)
    {

        preorderTraversal(root->keys[i]->child);
    }
    
    if (root->isLeaf == true)
        //printf("\n\tMBR of Leaf Node is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
                    printf("%d,%d,%d,%d\n",root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);

    else
      {  
                    printf("%d,%d,%d,%d\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
        //printf("\nMBR of Internal is Bottom Left Point x:%d y:%d Top Right Point x:%d y:%d\n\n", root->MBR.x1, root->MBR.y1, root->MBR.x2, root->MBR.y2);
       //if(root->parent==NULL) //printf("Total Leaf Nodes keys :%d\n",count);
      }
}


void keysLevel(node* curNode, int level) {

    if(curNode==NULL||curNode->numKeys==0){
        return;
    }
    // For each call, prints the DFS depth as well as all children associated with the current node.
    if(isLeaf(curNode)){
        num+=curNode->numKeys;
        printf("\nDFS Level: %d External Node with %d Keys\n", level, curNode->numKeys);
    }
    else{
        printf("\nDFS Level: %d Internal Node with %d Keys\n", level, curNode->numKeys);
    }

    // Recursive DFS call to visit children of the current node.
    for(int i = 0; i < curNode->numKeys; i++) {
        keysLevel( curNode->keys[i] , level + 1);
    }
}