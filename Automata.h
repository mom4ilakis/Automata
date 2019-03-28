#pragma once
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <queue>
using string = std::string;


class Automata
{
private:
	struct Node
	{
		
		//using a map so we can find a child in O(1) time. Future-proofing with a map;
		std::map<char, Node*> children;

		//a set of all words that end with the letter that is mapped to this node
		std::set<std::string> rec_words;
		//default c-tor
		Node();

	};
public:
	Automata();
	~Automata();
public:
	//retunrs number of known words
	unsigned numberKnownWords()const;

	void saveAsDOT();

	void testFunc();

	//prinst all known words
	void dictionary()const;
	//saves all known words to a txt file
	void SaveDict()const;

	void learnFromFile(const string& filename);
	//adds a word to the dictionary(automaton)
	void learnWord(const string& word);
	//returs true if teh word is know, word is from dictionary(automaton)
	bool recWord(const string& word) ;

	bool isEmpthy()const;
private:
	//before learing a word, creates a node for every letter that appears for the first time
	void createNodes(const string& word);

	void enqueueNodes(std::queue<char>& q, const char CH);


private:
	
	//this where every node lives, it is list so we can add new nodes without breaking pointers to the other ones
	std::list<Node> list_of_nodes;
	//maps a letter to a node
	std::map<char, Node*> all_vertices;


};

