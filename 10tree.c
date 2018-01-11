#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct tree 
{
	int value;
	struct tree *left,*right;
};

struct tree *root;

// ----------------------------------- tree_function modul ---------------------------------

struct tree *get_memory()
{
	struct tree *temp_ptr;
	if((temp_ptr = malloc(sizeof(struct tree))) != NULL)
	{
		temp_ptr->left = NULL;
		temp_ptr->right = NULL;
		return temp_ptr;
	}
	printf("\nget_memory error");
	return NULL;
}

struct tree *find_node(struct tree *ptr, int node_value)
{
	struct tree *temp_ptr;
	if(ptr == NULL)
	{
		printf("\nError find_node. ptr == NULL");
		return NULL;
	}
	if(ptr->value == node_value)
	{
		return ptr;
	}
	if(ptr->left != NULL)
	{
		if((temp_ptr = find_node(ptr->left,node_value)) != NULL)
			return temp_ptr;
	}
	if(ptr->right != NULL)
	{
		if((temp_ptr = find_node(ptr->right,node_value)) != NULL)
			return temp_ptr;
	}
	return NULL;
}

struct tree *find_parent(struct tree *ptr,int child_value)
{
	if(ptr == NULL)
	{
		printf("\nfind_parent error. ptr == NULL");
		return NULL;
	}
	if(root->value == child_value)
	{
		printf("\n\tThis is root.");
		return NULL;
	}
	struct tree *temp_ptr = NULL;
	if(ptr->left != NULL)
	{
		if(ptr->left->value == child_value)
			return ptr;
		if((temp_ptr = find_parent(ptr->left,child_value)) != NULL)
		{
			return temp_ptr;
		}
	}
	if(ptr->right != NULL)
	{
		if(ptr->right->value == child_value)	
			return ptr;
		
		if((temp_ptr = find_parent(ptr->right,child_value)) != NULL)
			return temp_ptr;
	}
	return NULL;
}

void add_node(int node_value, struct tree *ptr)
{
	if(ptr == NULL)
	{
		printf("\nError add_node. ptr == NULL");
		return;
	}
	if(ptr->left == NULL)
	{
		ptr->left = get_memory();
		ptr->left->value = node_value;
		printf("\na node is added (left).");
	}
	else if(ptr->right == NULL)
	{
		ptr->right = get_memory();
		ptr->right->value = node_value;
		printf("\na node is added (right).");
	} else printf("\nThis node already had 2 child.");

}

void delete_node_(struct tree *ptr)
{
	if(ptr == NULL)
	{
		printf("\ndelete_node error. ptr == NULL");
		return;
	}
	if(ptr->left != NULL)
	{
		delete_node_(ptr->left);
	}
	if(ptr->right != NULL)
	{
		delete_node_(ptr->right);
	}
	free(ptr);
}

void delete_node(int node_value)
{
	struct tree *temp_ptr = NULL;
	if((temp_ptr = find_node(root,node_value)) == NULL)
	{
		printf("\ndelete_node error. node does not exists");
		return;
	}
	if((temp_ptr->left != NULL) || (temp_ptr->right != NULL))
	{
		printf("This node has child. Delete [yes/no]");
		char com[255];
		scanf("%s",com);
		if((strncmp(com,"no",2)) == 0)
			return;
		if((strncmp(com,"yes",3)) != 0 )
			{
				printf("Invalid command == no");
				return;
			}
	}
	if(temp_ptr == root)
	{
		delete_node_(root);
		root = NULL;
		return;
	}
	struct tree *parent_ptr;
	parent_ptr = find_parent(root,node_value);
	if(parent_ptr->left != NULL)
	{
		if(parent_ptr->left->value == node_value)
		{
			delete_node_(parent_ptr->left);
			parent_ptr->left = NULL;
			return;
		}
	}
	if(parent_ptr->right != NULL)
	{
		if(parent_ptr->right->value == node_value)
		{
			delete_node_(parent_ptr->right);
			parent_ptr->right = NULL;
			return;
		}
	}
	return;
}

void add_root(int root_value)
{
	if(root != NULL)
	{
		printf("\nThe root already exists. Delete?\n");
		printf("\nEnter \"yes\" to delete current root.");
		printf("\nEnter \"no\" to create new root and add current root as a subtree.");
		printf("\nEnter \"cancel\" to leave as is.\n");
		char com[255];
		scanf("%s",com);
		if(strncmp(com,"yes",3) == 0)
		{
			delete_node(root->value);
			root = get_memory();
			root->value = root_value;
			return;
		}
		else if(strncmp(com,"no",2) == 0)
		{
			struct tree *temp_ptr = root;
			root = get_memory();
			root->value = root_value;
			root->left = temp_ptr;
			return;
		}
		else if(strncmp(com,"cancel",6) != 0)
		{
			printf("\nInvalid command\n");
			return;
		}
	}
	root = get_memory();
	root->value = root_value;
}



// -----------------------------------save and load modul --------------------------------




void load(char *filename)
{
	printf("\nload\n");
	FILE *fp;
	if((fp = fopen(filename,"r")) == NULL)
	{
		printf("\nError file\n");
		return;
	}
	int root_value,parent_value,node_value;
	fscanf(fp,"%d",&root_value);
	add_root(root_value);
	while(!feof(fp))
	{
		fscanf(fp,"%d%d",&parent_value,&node_value);
		add_node(node_value,find_node(root,parent_value));
	}
	fclose(fp);
}

void print_to_file(FILE *fp,struct tree *ptr)
{
	if(ptr == NULL)
		return;
	if(ptr->left != NULL)
	{
		fprintf(fp,"\n%d\n%d",ptr->value,ptr->left->value);
		print_to_file(fp,ptr->left);
	}
	if(ptr->right!= NULL)
	{
		fprintf(fp,"\n%d\n%d",ptr->value,ptr->right->value);
		print_to_file(fp,ptr->right);
	}
}

void save(char *filename)
{
	FILE *fp;
	if(root == NULL)
	{
		printf("\nTree does not exist\n");
		return;
	}
	if((fp = fopen(filename,"w")) == NULL)
	{
		printf("\nError file\n");
		return;
	}
	fprintf(fp,"%d",root->value);
	print_to_file(fp,root);
	fclose(fp);
}
// ------------------------------------ save and load modul ------------------------------





// ---------------------------------- statistic modul ------------------------------------




int count_node(struct tree *ptr)
{
	if(ptr == NULL)
		return 0;
	int count = 0;
	if( ptr->left != NULL)
		count += count_node(ptr->left);
	if(ptr->right != NULL)
		count += count_node(ptr->right);
	count ++;
	return count;

}

int c_node()
{
	return count_node(root);
}

int count_leaf(struct tree *ptr)
{
	if(ptr == NULL)
		return 0;
	int count = 0;
	if(ptr->left != NULL)
		count += count_leaf(ptr->left);
	if(ptr->right != NULL)
		count += count_leaf(ptr->right);
	if(ptr->left == NULL && ptr->right == NULL)
		return 1;
	return count;
}

int c_leaf()
{
	return count_leaf(root);
}

int heap_(struct tree *ptr, int parent_value)
{
	int flag = 0; // 0 - true 1 - false
	if(ptr == NULL)
	{
		return 0;
	}
	else if((ptr->value >= parent_value) && (ptr != root))
		return 1;
	if(ptr->left != NULL)
		if((flag = heap_(ptr->left,ptr->value)) == 1)
			return flag;
	if(ptr->right != NULL)
		if((flag = heap_(ptr->right,ptr->value)) == 1)
			return flag;
	return 0;
}

int heap()
{
	if(root != NULL)
		return heap_(root,root->value);
	printf("\nTree does not exist\n");
	return 0;
}

int height_(struct tree *ptr)
{
	if(ptr == NULL)
	{
		return 0;
	}
	int l = 1,r = 1;
	if(ptr->left != NULL)
	{
		l = 1 + height_(ptr->left);
	}
	if(ptr->right !=NULL)
	{
		r = 1 + height_(ptr->right);
	}
	if(l > r)
		return l;
	return r;
}

int height()
{
	return height_(root);
}


// ------------------------------------ statistic modul ---------------------------------




// ------------------------------------ print modul --------------------------------------



void print_grape(int node_value)
{
	struct tree *temp_ptr;
	if((temp_ptr = find_parent(root,node_value)) != NULL)
	{
		printf("\n\tparent_value == %d\n",temp_ptr->value);
	} else printf("\n");
	if((temp_ptr = find_node(root,node_value)) != NULL)
	{
		printf("\tnode_value == %d\n",node_value);
		if(temp_ptr->left != NULL)
		{
			printf("\tLeft_child_value == %d\n",temp_ptr->left->value);
		}
		if(temp_ptr->right != NULL)
		{
			printf("\tRight_child_value == %d\n",temp_ptr->right->value);
		}
	}	
}

int power2(int n)
{
	if(n == 0)
		return 1;
	return 2*power2(n-1);
}

void print_space(int n)
{
	int i;
	for(i = 0; i < power2(n) - 1; i++)
	{
		printf(" ");
	}
}

void print_line(struct tree *ptr,int max_lvl, int lvl, int current_lvl)
{
	if(ptr == NULL)
	{
		return;
	}
	current_lvl++;
	if(current_lvl + 1 == lvl)
	{
		if(ptr->left != NULL)
		{
			printf("%d",ptr->left->value);
		}
		else printf(" ");
		print_space(max_lvl - lvl + 1 );
		if(ptr->right != NULL)
		{
			printf("%d",ptr->right->value);
		} else printf(" ");
		print_space(max_lvl - lvl + 1 );
	}
	if(current_lvl + 1 < lvl)
	{
	   if(ptr->left != NULL)
	   		print_line(ptr->left,max_lvl,lvl,current_lvl);
	   if(ptr->right != NULL)
   			print_line(ptr->right,max_lvl,lvl,current_lvl);	
	}	   
}

void print_tree()
{
	if(root == NULL)
	{
		printf("\nprint_tree error. Tree does not exist");
		return;
	}
	int lvl,max_lvl = 2 + height();
	print_space(max_lvl - 1);
	printf(" ");
	printf("%d",root->value);
	printf("\n\n");
	for(lvl = 2; lvl <= max_lvl; lvl++)
	{
		print_space(max_lvl - lvl);
		printf(" ");
		print_line(root,max_lvl,lvl,0);
		printf("\n\n");
	}
}


// ---------------------------------- print modul ---------------------------------------





//----------------------------------- interface modul -------------------------------------

void man()
{
	printf("\n1 - add_root");
	printf("\n2 - add_node");
	printf("\n3 - add_node_sibling");
	printf("\n4 - find_parent");
	printf("\n5 - find_node");
	printf("\n6 - delete_node");
	printf("\n7 - statistic");
	printf("\n8 - save");
	printf("\n9 - load");
	printf("\n10 - man");
	printf("\n11 - print tree");
	printf("\n0 - exit\n\n");

}
//----------------------------------- interfase modul -------------------------------------
int main()
{
	man();
	int com;
	int node_value,parent_value,root_value,sibling_value;
	char filename[255];
	for(;;)
	{
		printf("\n\nEnter command\t");	
		scanf("%d",&com);
		printf("\n");
		switch(com)
		{
			case 1:
			{
				printf("\nEnter root_value\t");
				scanf("%d",&root_value);
				add_root(root_value);
				break;
			}
			case 2:
			{
				printf("\nEnter node_value\t");
				scanf("%d",&node_value);
				printf("\nEnter parent_value\t");
				scanf("%d",&parent_value);
				add_node(node_value,find_node(root,parent_value));
				break;

			}
			case 3:
			{
				printf("\nEnter node_value\t");
				scanf("%d",&node_value);
				printf("\nEnter sibling_value\t");
				scanf("%d",&sibling_value);
				add_node(node_value,find_parent(root,sibling_value));
				break;
			}
			case 4:
			{
				printf("\nEnter node_value\t");
				scanf("%d",&node_value);
				struct tree *temp_ptr = NULL;
				temp_ptr = find_parent(root,node_value);
				if(temp_ptr != NULL)
					printf("\n\tparent value == %d\n",temp_ptr->value);
				break;

			}
			case 5:
			{
				printf("\nEnter node_value\t");
				scanf("%d",&node_value);
				struct tree *temp_ptr = NULL;
				temp_ptr = find_node(root,node_value);
				if(temp_ptr != NULL)
				 print_grape(node_value);
				break;	
			}
			case 6:
			{
				printf("\nEnter node_value\t");
				scanf("%d",&node_value);
				delete_node(node_value);
				break;
			}
			case 7:
			{
				printf("\n\tall node = %d",c_node());
				printf("\n\tleaf     = %d",c_leaf());
				printf("\n\theight   = %d",height());
				if(root == NULL)
					break;
				if(heap() == 0)
				{
					printf("\n\theap     - yes");
				} else printf("\n\theap     - no");
				break;
			}
			case 8:
			{
				printf("\nEnter filename\n");
				scanf("%s",filename);
				save(filename);
				break;
			}
			case 9:
			{
				printf("\nEnter filename\n");
				scanf("%s",filename);
				load(filename);
				break;
			}
			case 10:
			{
				man();
				break;
			}
			case 11:
			{
				print_tree();
				break;
			}
			case 0:
			{
				exit(0);
				break;
			}
			default:
			{
				printf("\ninvalid command\n");
				break;
			}
		}
	}
	return 0;
}

