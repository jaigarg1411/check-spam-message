/* Purpose :- Identifying message is spam or ham
   Author :- Jai Garg
   Date :- 06-04-2018 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int arr2[20] = {0};

int absdifference(int a, int b)
{
	int c = a - b;
	if (c > 0)
		return c;
	else
		return -c;
}

int ReturnDistance(char* word1, char* word2)
{
	int w1 = strlen(word1);
	int w2 = strlen(word2);
	int A[w1+1][w2+1];
	int i;
	for(i=0 ; i<=w1 ; i++)
	{
		A[i][0] = i;
	}
	for(i=0 ; i<=w2 ; i++)
	{
		A[0][i] = i;
	}
	char x;
	int j;
	for(i=1 ; i<=w1 ; i++)
	{
		x = word1[i-1];
		for(j=1 ; j<=w2 ; j++)
		{
			char y;
			y = word2[j-1];
			if (x==y)
			{
				A[i][j] = A[i-1][j-1];
			}
			else
			{
				int d = A[i-1][j] + 1;
				int in = A[i][j-1] + 1;
				int s = A[i-1][j-1] + 1;
				int m = d;
				if(in < m)
				{
					m=in;
				}
				if(s < m)
				{
					m=s;
				}
				A[i][j] = m;
			}
		}
	}
	return A[w1][w2];
}

void ComputeKMeans(char A[8754][20], int n)
{
	int num[8754] = {0};
	for (int y = 0; y < 8754; y++)
	{
		int feq[20];
		int k;
		for (k=0; k < 20; k++)
		{
			feq[k] = ReturnDistance(A[(arr2[k])],A[y]);
		}
		int q = feq[0];
		int e = 0, w = 0;
		while (w < 20)
		{
			if(feq[w] < q)
			{
				q = feq[w];
				e = w;
			}
			w++;
		}
		num[y] = e;
		if( (y % 60) == 0 )
		{
			int sum, j, temp, o=0;
			for (j = 0; j <= y; j++)
			{
				sum=0;
				for(k = 0; k <= y; k++)
				{
					if((num[j] == e) && (num[k] == e))
					{
						sum = sum + ReturnDistance(A[j],A[k]);
					}
				}
				if(j==0)
				{
					temp = sum;
					o = j;
				}
				else
				{
					if(temp > sum)
					{
						temp = sum;
						o = j;
					}
				}
			}
			arr2[e] = y;
		}
	}
}

int* ComputeHistogram(char A[8754][20])
{
	int i = 0, k;
	static int j = 0;
	char b[50][25];
	static int feq[20] = {0};
	while ( (strcmp(A[j],"spam") != 0) && (strcmp(A[j],"ham") != 0))
		{
			strcpy(b[i],A[j]);
				i++;
				j++;
		}
		j++;
		b[i][0] = '\0';
	int sum;
	for (i = 0; i < 20; i++)
	{
		k = 0, sum = 0;
		while (b[k][0] != '\0')
		{
			sum = sum + ReturnDistance(A[(arr2[i])],b[k]);
			k++;
		}
		feq[i] = sum;
	}
	return feq;
}

int* ComputeHistogram1(char B[6871][20], char A[8754][20])
{
	int i = 0, k;
	static int j = 0;
	char b[50][25];
	static int feq[20] = {0};
	while ( (strcmp(B[j],"spam") != 0) && (strcmp(B[j],"ham") != 0))
		{
			strcpy(b[i],B[j]);
			i++;
			j++;
		}
		j++;
		b[i][0] = '\0';
	int sum;
	for (i = 0; i < 20; i++)
	{
		k = 0;
		sum = 0;
		while (b[k][0] != '\0')
		{
			sum = sum + ReturnDistance(A[(arr2[i])],b[k]);
			k++;
		}
		feq[i] = sum;
	}
	return feq;
}

int ComputeL1distance (int a[20], int b[20])
{
	int i,sum=0;
	for (i = 0; i < 20; i++)
	{
		sum = sum + absdifference(a[i],b[i]);
	}
	return sum;
}

void main()
{
	char A[8754][20];
	char ctemp, ctemp1, ctemp2;
	int i = 0, j = 0;
	FILE *fp;
	fp = fopen ("Train.txt" , "r");
	if (fp == NULL)
	{
		printf("Error opening the file !");
	}
	else
	{
		while ((ctemp = getc (fp)) != EOF)
		{
			ctemp1 = tolower (ctemp);
			if (ctemp1 >= 'a' && ctemp1 <= 'z')
			{
				A[i][j] = ctemp1;
				j++;
				ctemp2 = ctemp1;
			}
			else if (ctemp2 >= 'a' && ctemp2 <= 'z')
			{
				ctemp2 = ctemp;
				A[i][j] = '\0';
				j = 0;
				i++;
			}
		}
		strcpy(A[8753],"ham");
	}
	fclose(fp);
	for (i = 0; i < 20; i++)
	{
		arr2[i] = rand() % 8754;
	}
	printf ("Please wait!\n");
	printf("K-Means computation started!\n");
	ComputeKMeans(A,20);
	printf ("\nRepresentative strings are:- \n");
	int e;
	for (e=0; e<20; e++)
	{
		printf ("%s\n", A[arr2[e]]);
	}
	printf("\n");
	int H[542][20];
	for (i = 0; i < 542; i++)
	{
		for (j = 0; j < 20; j++)
		{
			H[i][j] = 0;
		}
	}
	int x ,y = 0;
	int *u;
	for (x = 0; x < 542; x++)
	{
		u = ComputeHistogram(A);
		for(y = 0; y < 20; y++)
		{
			H[x][y] = u[y];
		}
	}
	char B[6871][20];
	char ctemp3, ctemp4, ctemp5;
	int i1 = 0, j1 = 0;
	FILE *fp1;
	fp1 = fopen ("Test.txt" , "r");

	if (fp1 == NULL)
	{
		printf("Error opening the file !");
	}
	else
	{
		while ((ctemp3 = getc (fp1)) != EOF)
		{
			ctemp4 = tolower (ctemp3);
			if (ctemp4 >= 'a' && ctemp4 <= 'z')
			{
				B[i1][j1] = ctemp4;
				j1++;
				ctemp5 = ctemp4;
			}
			else if (ctemp5 >= 'a' && ctemp5 <= 'z')
			{
				ctemp5 = ctemp3;
				B[i1][j1] = '\0';
				j1 = 0;
				i1++;
			}
		}
		strcpy(B[6871],"ham");
	}
	fclose(fp1);
	int H1[542][20];
	for (i = 0; i < 542; i++)
	{
		for (j = 0; j < 20; j++)
		{
			H1[i][j] = 0;
		}
	}
	int p ,l = 0;
	int *w;
	for (p = 0; p < 542; p++)
	{
		w = ComputeHistogram1(B,A);
		for(l = 0; l < 20; l++)
		{
			H1[p][l] = w[l];
		}
	}
	int k = 0, spam = 0, ham = 0;
	printf("Results :-\n");
	for (i = 0; i < 542; i++)
	{
		int dis[542] = {0};
		for (j = 0; j < 542; j++)
		{
			dis[j] = ComputeL1distance(H1[i],H[j]);
		}
		int min = dis[0];
		int f = 0;
		for(k = 0; k < 542; k++)
		{
			if (dis[k] < min)
			{
				min = dis[k];
				f = k;
			}
		}
		if (f < 41)
		{
			printf ("Message number %d is :- spam",i+1);
		}
		else
		{
			printf ("Message number %d is :- ham",i+1);
		}
		if (f < 41 && i < 41)
			spam++;
		else if (f > 41 && i > 41)
			ham++;
		printf("\n");
	}
	printf("\nData Results\n");
	printf("Number of spam messages correctly identified are  (out of 41) :- %d\n",spam);
	printf("Number of ham messages correctly identified are  (out of 501) :- %d\n",ham);
	printf("Total number of messages correctly identified are  (out of 542) :- %d\n",spam + ham);

	double eff;
	eff = (spam + ham) / 542.00;
	printf("\nPercentage Effeciency :- %f",eff * 100);
}
