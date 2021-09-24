#include <iostream>
#include <vector>
#include <queue>


class Graph
{
private:
	int count;
    std::vector <std::vector <int>> matrix;
public:
	Graph()
	{
        count = 0;
    	}

	Graph(std::vector <std::vector <int>> init_matr)
	{
		int size = init_matr.size();
		bool type = true;
		for (int i = 0; i < size; i++)
		{
			if (size != init_matr[i].size())
			{
			type = false;
			}
		}
		if (type == true)
		{
		    count = size;
		    matrix.insert(matrix.begin(), init_matr.begin(), init_matr.end());
		}
    	}
	int getSize()
	{
        	return count;
    	}

	std::vector<std::vector <int>> getMatrix()
	{
        	return matrix;
   	}

	void printMatrix()
	{
        for (int i = 0; i < count; i++)
	{
        	for (int j = 0; j < count; j++)
		{
                	if (matrix[i][j] == 1)
			{
                    		std::cout << 1 << " ";
                	}
            		else 
			{
                    		std::cout << 0 << " ";
                	}
            	}
            	std::cout << std::endl;
        }
	std::cout << count << std::endl;
    	}
};


class DFS
{
private:
    std::vector <char> color; //w - white, b - black, g - gray;
    std::vector <int> p;
	
    void depth_first_search(int start, Graph &G)
	{
        discover_vertex(start, G);
        for (int i = 0; i < G.getSize(); i++)
		{
            if (G.getMatrix()[start][i] == 1)
			{
                examine_edge(start, i, G);
            }
        }
        finish_vertex(start, G);
    }
	
	void initialize_vertex(int num, Graph &G)
	{
        	color[num] = 'w';
       	 	p[num] = num;
    	}

    void start_vertex(int num, Graph &G)
    {
        if (color[num] == 'w')
	{
            depth_first_search(num, G);
        }
    }

    void discover_vertex(int num, Graph &G)
	{
        color[num] = 'g';
    }

    void examine_edge(int from, int to, Graph &G)
   {
        if (color[to] == 'w')
	{
            tree_edge(from, to, G);
        }
    	if (color[to] == 'g')
	{
            back_edge(from, to, G);
        }
    	if (color[to] == 'b')
	{
            forward_or_cross_edge(from, to, G);
        }
    }

    void tree_edge(int from, int to, Graph &G)
    {
        p[to] = from;
        start_vertex(to, G);
    }

    void back_edge(int from, int to, Graph &G){return;}

    void forward_or_cross_edge(int from, int to, Graph &G){return;}

    void finish_edge(int from, int to, Graph &G){return;}

    void finish_vertex(int num, Graph &G)
    {
        color[num] = 'b';
    }

public:
    DFS(Graph &G, int start)
    {
	p = std::vector<int>(G.getSize());
        color = std::vector<char>(G.getSize());
        for (int i = 0; i < G.getSize(); i++)
	{
            initialize_vertex(i, G);
        }
        start_vertex(start, G);
    }

    std::vector<char> GetVisited(){
        return color;
    }

    ~DFS()
    {
    	color.clear();
    	color.shrink_to_fit();
        p.clear();
	p.shrink_to_fit();
    }
};

void find_comps(Graph &G) {
	int count = 0;
	size_t i=0;
	while (i<G.getSize())
	{
		
		DFS dfs(G, i);
		std::cout << "Comp:";
		count++;
		for (size_t j=0; j<dfs.GetVisited().size(); j++)
		{
			if(dfs.GetVisited()[j]=='b')
			{
				std::cout << ' ' <<j;
				i++;
			}
		}
		std::cout << std::endl;
	}
	std::cout << "Count: " << count << std::endl;
}

class BFS
{
private:
    std::vector <char> color; //w - white, b - black, g - gray;
    std::vector <int> p;
	std::vector <int> d;
	std::queue<int> q;
    void breadth_first_search(int start, Graph &G)
	{
	int rez = start;
	d[start] = 0;
        discover_vertex(start, G);
	while(q.empty()==false)
	{
		int start = q.front();
		q.pop();
		for (int i = 0; i < G.getSize(); i++)
		{
			if (G.getMatrix()[start][i] == 1)
			{
			examine_edge(start, i, G);
		   	}
		}
		finish_vertex(start, G);
	}
	std::cout<<"BFS len to all node from "<<rez<<":";
	for(int z = 0; z < d.size(); z++)
	{
		std::cout<<" "<<d[z]<<" ";
	}
	std::cout<<std::endl;
    	}
	
	void initialize_vertex(int num, Graph &G)
	{
        color[num] = 'w';
		d[num]= -1;
        p[num] = num;
    	}

    void start_vertex(int num, Graph &G)
    {
        if (color[num] == 'w')
	{
            depth_first_search(num, G);
        }
    }

    void discover_vertex(int num, Graph &G)
	{
        color[num] = 'g';
	q.push(num);
        }

    void examine_edge(int from, int to, Graph &G)
	{
        if (color[to] == 'w')
	{
            tree_edge(from, to, G);
        }
    	if (color[to] == 'g')
	{
            back_edge(from, to, G);
        }
    	if (color[to] == 'b')
	{
            forward_or_cross_edge(from, to, G);
        }
    }

    void tree_edge(int from, int to, Graph &G)
    {
        p[to] = from;
	d[to] = d[from] + 1;
        discover_vertex(to, G);
    }

    void back_edge(int from, int to, Graph &G){return;}

    void forward_or_cross_edge(int from, int to, Graph &G){return;}

    void finish_edge(int from, int to, Graph &G){return;}

    void finish_vertex(int num, Graph &G)
	{
		
        color[num] = 'b';
    }

public:
    BFS(Graph &G, int start)
    {
	p = std::vector<int>(G.getSize());
	d = std::vector<int>(G.getSize());
        color = std::vector<char>(G.getSize());
        for (int i = 0; i < G.getSize(); i++)
	{
            initialize_vertex(i, G);
        }
        start_vertex(start, G);
    }

    ~BFS()
    {
        color.clear();
	color.shrink_to_fit();
        p.clear();
	p.shrink_to_fit();
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
	int n;
	std::cin >> n;
	std::vector <std::vector <int>> R;
    	for (int i = 0; i < n; i++)
	{
        	R.push_back(std::vector<int> (n));
    	}
    	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
		    int type;
		    std::cin >> type;
		    R[i][j] = type;
        	}
	}
   	Graph G(R);
	BFS len(G,6);
	G.printMatrix();
	find_comps(G);
	system("pause");
	return 0;
}



/*
example:
7
0
1
1
0
0
0
1
1
0
1
0
0
0
0
1
1
0
0
0
0
0
0
0
0
0
1
1
0
0
0
0
1
0
0
0
0
0
0
1
1
0
0
1
0
0
0
0
0
0
*/
