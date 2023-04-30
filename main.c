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