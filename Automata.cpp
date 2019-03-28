#include "Automata.h"
#include <iostream>
#include <chrono>
#include <queue>


//#ifdef DEBUG
#define PER_POINT_1 std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
#define PER_POINT_2 std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
#define PER_LOG auto durr = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();std::cout << durr << '\n';
//#else
//#define PER_POINT_1
//#define PER_POINT_2
//#define PER_LOG
//#endif // DEBUG


Automata::Automata()
{
	



}


Automata::~Automata()
{
}

unsigned Automata::numberKnownWords() const
{
	unsigned sum=0;

	for (const Node& n : list_of_nodes)
		sum += n.rec_words.size();



	return sum;
}
//the name of the node is the last read letter
void Automata::saveAsDOT()
{
	std::ofstream DOTfile;
	
	//this will hold all the children of the current node
	std::queue<char> chld;

	std::string filename;

	std::string start="qs", fin = "qf", name = "q";

	unsigned count = 0;

	std::cout << "Enter file name:\n";
	
	std::cin >> filename;
	
	filename += ".gv";

	DOTfile.open(filename);

	DOTfile << "digraph{\n";

	for (char ch = 'a'; ch <= 'z'; ++ch)
	{
		if (all_vertices.find(ch) != all_vertices.end())
		{
			
			//enqueues all the children of node ch
			enqueueNodes(chld, ch);

			while (chld.size() > 0)
			{
				DOTfile << ch << "->" << chld.front() <<"[label="<<ch<<"]"<< ";\n";
				chld.pop();
			}

			if (all_vertices[ch]->rec_words.size() > 0)
			{
				DOTfile << ch << "[color=red,style=filled,fillcolor=\"#ffefef\"];\n";
				//DOTfile << ch << "->" << fin << "[label=\"" << ch << "\"];\n";
			}

		}
	}


	/*for (std::pair<char, Node*> pr : all_vertices)
	{
		count++;
		DOTfile << pr.first << "[label=\"" << name + std::to_string(count) << "\"];\n";
	}*/


	DOTfile << '}';
	DOTfile.close();
}

void Automata::testFunc()
{
	PER_POINT_1;

	for (int i = 0; i < 1000;i++)
	{
		;
	}
	PER_POINT_2;

	PER_LOG;

}

void Automata::dictionary() const
{
	/*for (const std::pair<char, Node*> p : all_vertices)
		for (const string& str : p.second->rec_words)
			std::cout << str << '\n';*/
	if (isEmpthy())
		std::cout << "No words known\n";
	else
	for (const Node& n : list_of_nodes)
		for (const string& str : n.rec_words)
			printf("%s \n", str.c_str());

}

void Automata::SaveDict() const
{
	std::string filename;

	std::cout << "Name of file:\n";

	std::cin >> filename;

	filename += ".txt";

	std::ofstream outputFile;

	outputFile.open(filename , std::ios::ate);


	if (outputFile.is_open())
	{
		for (const std::pair<char, Node*> p : all_vertices)
			for (const string& str : p.second->rec_words)
				outputFile << str << '\n';
	}
	outputFile.close();
}











void Automata::learnFromFile(const string & filename)
{
	std::cout << "Learing form file: " << filename << '\n';

	std::ifstream inputFile;

	inputFile.open(filename);

	if (inputFile.is_open())
	{
		std::string inputWord;

		while (inputFile.good() && !inputFile.eof())
		{
			inputFile >> inputWord;

			learnWord(inputWord);
		}

		inputFile.clear();
		inputFile.close();
	}
	else
		throw std::invalid_argument("FILE FAILED TO OPEN !\n");




}

void Automata::learnWord(const string & word)
{
	createNodes(word);
	
	Node* cur_node = all_vertices[word[0]];

	size_t word_len = word.size();


	for (size_t i = 1; i < word_len; ++i)
	{
		cur_node->children[word[i]] = all_vertices[word[i]];
		cur_node = all_vertices[word[i]];
		//std::cout << "Created: " << word[i - 1] << "--->" << word[i] << '\n';
	}
	cur_node->rec_words.insert(word);


	




}

bool Automata::recWord(const string & word) 
{
	

	//used for perfomance benchmark
	PER_POINT_1

	//checks if the first letter is known
	if (all_vertices.find(word[0]) != all_vertices.end())
	{
		//this is the node that will traverse the automaton
		Node* curr_node = all_vertices[word[0]];

		size_t size = word.size();
		//for every letter from the word, beggining from the second letter
		for (size_t i = 1; i < size; ++i)
		{
			//check if the letter is know
			if (curr_node->children.find(word[i]) != curr_node->children.end())
			{
				//goes to the node that is mapped to the letter
				curr_node = all_vertices[word[i]];
			}
			else
			{
				//perfomance monitoring
				PER_POINT_2
				PER_LOG
				//if the letter is not known
				return false;
			}
		}
		//checks if the word is known, we can have all the nodes, but there might be a loop. 
		//Example google and gooooogle, if this check was not in place both words will be recognisable by the automaton, there will be a loop for o
		if (curr_node->rec_words.find(word) != curr_node->rec_words.end())
		{
			PER_POINT_2
			PER_LOG
			return true;
		}
		else
		{
			PER_POINT_2
			PER_LOG
			return false;
		}

	}
	else
		return false;
	




	return false;
}

bool Automata::isEmpthy() const
{
	return list_of_nodes.empty();
}

void Automata::createNodes(const string & word)
{
	using map_it = std::map<char, Node*>::iterator;

	//this shoud create nodes with no children
	for (const char ch : word)
	{
		//itterator for the vertices, aka mapped nodes to a letter 
		map_it it = all_vertices.find(ch);

		//if the letter is not mapped, is encontered for the fist time
		if (it == all_vertices.end())
		{
			//we create a node in the list
			list_of_nodes.push_back(Node());
			//mapping the creadet node to the letter
			all_vertices.insert(std::pair<char, Node*>(ch, &list_of_nodes.back()));
		}
	}






}

void Automata::enqueueNodes(std::queue<char>& q, const char CH)
{
	if (all_vertices.find(CH) != all_vertices.end())
	{

		for (std::pair<char, Node*> pr : all_vertices[CH]->children)
		{
			q.push(pr.first);
		}


	}
}

Automata::Node::Node()
{
	;
}
