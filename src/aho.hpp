#ifndef AHO_CORASICK_HEADER
#define AHO_CORASICK_HEADER

#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

class Aho
{
    struct Vertex
    {
        std::unordered_map<char, int> children{};
        int parent = -1;
        int suffix_link = -1;
        int end_word_link = -1;
        int word_ID = -1;
        char parent_char;
        bool leaf = false;
    };

    struct expressoes
    {
        string word;
        int paragafo;
        int ocorrencias;
        std::vector<int> linhas;
    };

    std::vector<Vertex> trie;
    std::vector<int> words_length;
    int size;
    int root;
    int wordID;
    std::unordered_map<std::string, std::vector<int>> occurrences;

public:
    Aho();

    void add_string(const std::string &s);

    void prepare();

    void process(const std::string &text, std::ofstream &file_OutPut);

    void add_expressions(const std::string &filename);

    void process_text_file(std::string text, std::ofstream &file_OutPut);

    void print_matches();

    void output_expressao(ofstream &file_OutPut);

private:
    void CalcSuffLink(int vertex);

    int getLineNumber(const std::string &text, int index) const;
};

#endif // AHO_CORASICK_HEADER
