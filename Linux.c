#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <linux/types.h>
#include <linux/list.h>
#define EWL 15
#define LENGTH 50

char empty_word[][EWL]={"of","to","in","and","as","from","for","with","that","have",
					"by","on","upon","about","above","across","among","ahead","after","a",
					"an","although","at","also","along","around","always","away","any","up",
					"under","until","before","between","beyond","behind","because","what","when","would",
					"could","who","whom","whose","which","where","why","without","whether","down",
					"during","despite","over","off","only","other","out","than","the","then",
					"through","throughout","that","these","this","those","there","therefore","till","some",
					"such","since","so","can","many","much","more","may","might","must",
					"ever","even","every","each"
};
typedef struct Word
{
	char ch[50];
	int num;
	struct list_head list;
}Word;
int totalWord,totalNode;
char choice='2';
int  dsnum=LENGTH;
int getWord(char word[],FILE *fp)
{
	char c;
	int count=0;
	c=fgetc(fp);
	while(!feof(fp)&&!(c>='a'&&c<='z')&&!(c>='A'&&c<='Z'))c=fgetc(fp);
	while(!feof(fp)&&(c>='a'&&c<='z')||(c>='A'&&c<='Z')||c=='-'||c=='\'')
	{
		word[count++]=(c>='A'&&c<='Z')?c+32:c;
		c=fgetc(fp);
		if(c=='\n'&&word[count-1]=='-')c=fgetc(fp);	
	}
	if(count>0)
	{
		if(word[count-1]=='\'')count--;
		word[count]='\0';
		return 1;
	}
	return 0;	
}
int isNotEmptyWord(char word[])
{
	int EWlength=sizeof(empty_word)/sizeof(empty_word[0]);	
	int i;
	for(i=0;i<EWlength;i++)if(strcmp(word,empty_word[i])==0)return 0;
	return 1;
}
int listEach(struct list_head *plist,struct list_head *head,char tempword[],Word *node)
{
	list_for_each(plist,head)
	{
		Word *listnode=list_entry(plist,Word,list);
		if(strcmp(listnode->ch,tempword)==0)
		{
		listnode->num+=node->num;
		return 1;
		}
	}
	return 0;
}
int isEndOfs(Word *node,struct list_head *plist,struct list_head *head)
{
	if(strcmp(node->ch,"is")==0)return 0;
	char suffix[20]="ies",*suffixp=suffix; 
	char tempword[50];
	char *p=tempword;
	while(*suffixp)suffixp++;suffixp--;
	strcpy(tempword,node->ch);
	while(*p)p++;p--;
	if(*p&&*p!=*suffixp)return 0;
	if(*(p-1)&&*(p-1)!='\'')*p='\0';
	else return 0;
	if(listEach(plist,head,tempword,node))return 1;
	p--;
	suffixp--;
	if(*p&&*p!=*suffixp)return 0;
	*p='\0';
	if(listEach(plist,head,tempword,node))return 1;
	p--;
	suffixp--;
	if(*p&&*p!=*suffixp)return 0;
	*p='y';
	p++;
	*p='\0';
	if(listEach(plist,head,tempword,node))return 1;
	return 0;
}
int isEndOfd(Word *node,struct list_head *plist,struct list_head *head)
{
	char suffix[20]="ed",*suffixp=suffix;
	char tempword[50];
	char *p=tempword;
	while(*suffixp)suffixp++;suffixp--;
	strcpy(tempword,node->ch);
	while(*p)p++;p--;
	if(*p&&*p!=*suffixp)return 0;
	*p='\0';
	if(listEach(plist,head,tempword,node))return 1;
	p--;
	suffixp--;
	if(*p&&*p!=*suffixp)return 0;
	*p='\0';
	if(listEach(plist,head,tempword,node))return 1;
	return 0;
}
void sortByNum(struct list_head *head)
{	
	int max,t;
	char tempWord[50];
	struct list_head *temp=head->next,*plist;
	while(temp->next!=head)
	{
		struct list_head *next = temp->next;
		Word *node1 = list_entry(temp,Word,list);
		max=node1->num; 
		plist=temp;
		while(next!=head)
		{
			Word *node2 = list_entry(next,Word,list);
			if(max<node2->num)
			{
				max=node2->num;
				plist=next;
			}
			next=next->next;
		}
		if(max!=node1->num)
		{
			Word *tempNode = list_entry(plist,Word,list);
			t=node1->num;
			node1->num=tempNode->num;
			tempNode->num=t;
		
			strcpy(tempWord,node1->ch);
			strcpy(node1->ch,tempNode->ch);
			strcpy(tempNode->ch,tempWord); 
		}
	temp=temp->next;
	}
}
void delNegative(struct list_head *head)
{
	struct list_head *temp1=head->next,*temp2;
	Word *node;
	while(temp1!=head)
	{
		node=list_entry(temp1,Word,list);
		if(node->num!=-1)break;
		temp1=temp1->next;	
	}
	temp1->prev=head;	
	head->next=temp1;
	temp2=temp1->next;
	while(temp2!=head)
	{
		node=list_entry(temp2,Word,list);
		if(node->num==-1){temp2=temp2->next;continue;}
		temp1->next=temp2;
		temp2->prev=temp1;
		temp1=temp2;
		temp2=temp2->next;
	}
	temp1->next=temp2;
	temp2->prev=temp1;
}
void showAssist()
{
	printf("+--------------------------------------------------+----------------+------------------+\n");	
}
void display(struct list_head *plist,struct list_head *head)
{
	int printLength=totalNode;
	if(choice=='1')printLength=LENGTH>totalNode?totalNode:LENGTH;
		else if(choice=='2')printLength=totalNode;
			else if(choice=='3')
							{
								while(1)
								{
								printf("Please enter the number of words you want to display(The range is 1 to %4d):   ",totalNode);
								scanf("%d",&dsnum);	
								if(dsnum<1||dsnum>totalNode)printf("Error:Your input is incorrect. Please try again\n\n");
								else break;
								}
								printf("\n\n\n");
								printLength=dsnum;
							}
				else printLength=LENGTH>totalNode?totalNode:LENGTH;
	showAssist();
	printf("|                     word                         |   frequency    |    percentage    |\n");
	showAssist();           
	list_for_each(plist,head)
	{
		Word *node=list_entry(plist,Word,list);
		printf("|%-50s",node->ch);
		printf("|  %4d          |\t%5.2f%%\t       |\n",node->num,1.0*node->num/totalWord*100);
		showAssist();	
		printLength--;
		if(printLength==0)break;
	}
	printf("After removing the Empty words, The total number of words in the article:%4d\n",totalWord);
}
void tips()
{
	printf("\n\n\nStatistics of English words,Please select:\n\n");
	printf("1.Displaying words in descending order (The default display is 50).\n\n");
	printf("2.Displays all words in descending order.\n\n");
	printf("3.Displaying words in descending order (manually enter the number of words to display).\n\n");
	printf("Press Enter to confirm the selection:   ");
	scanf("%c",&choice);
	printf("\n\n\n");	
}
int main()
{
	int num=0;
	FILE *fp;
	char temp[50];
	char filename[100]="test.txt";
	int flag=0;
	struct list_head head,*plist;
	Word *ptmp;
	if((fp=fopen(filename,"r"))==NULL)exit(0);
	INIT_LIST_HEAD(&head);
	tips();
	while(!feof(fp))
	{
		flag=0;
		if(getWord(temp,fp))
		{
			if(isNotEmptyWord(temp))
			{
				totalWord++;
				if(!list_empty(&head))
				{
					list_for_each(plist,&head)
					{
						Word *node=list_entry(plist,Word,list);
						if(strcmp(temp,node->ch)==0)
						{
							flag=1;
							node->num++;
						}
					}
				}
				if(flag==0)
				{
					ptmp=(Word *)malloc(sizeof(Word));
					strcpy(ptmp->ch,temp);
					ptmp->num=1;
					list_add_tail(&(ptmp->list),&head);
					totalNode++;	
				}
			}
		}
	}
	fclose(fp);
	list_for_each(plist,&head)
	{
		Word *node=list_entry(plist,Word,list);
		if(isEndOfs(node,plist,&head)||isEndOfd(node,plist,&head)){node->num=-1;totalNode--;}
	}
	delNegative(&head);
	sortByNum(&head);
	display(plist,&head);
}
