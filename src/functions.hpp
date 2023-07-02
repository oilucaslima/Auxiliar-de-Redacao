#include "header.hpp"
#include "models.hpp"

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

void existentWord(std::string str, int n, std::map<std::string, WordInfo> &glossary);

void returnWord(char c, char cAnterior, char cAnteriorAnterior, int &n, std::string &str, std::map<std::string, WordInfo> &glossary, vector<int> &posicaoParagrafo, std::unordered_map<std::string, WordInfo> &glossaryStopWords, vector<ParagraphInfo> &vetorParagrafos, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes, ifstream &file, ofstream &file_OutPut);

void addStopWord(unordered_map<string, WordInfo> &glossaryStopWords, string &str, int n);

void existentWordUnorderedMap(std::unordered_map<std::string, WordInfo> &glossaryStopWords, std::string str, int n);

void existePalavraNaSentenca(string str, int n);

void existePalavraNoParagrafo(string str, int n);

void addVectorPalavraNaSentenca(WordInfo newWord, string str, int n);

void addVectorParagrafo(string str, int n);

void updateWord(WordInfo newWord, map<string, WordInfo> &glossary, string &str, unordered_map<string, WordInfo> &glossaryStopWords, int &n, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes);

void updateParagrafo(char c, char cAnterior, char cAnteriorAnterior, vector<int> &posicaoParagrafo, int &n, vector<ParagraphInfo> &vetorParagrafos, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes);

void alocarParagrafo(char cAnterior, char cAnteriorAnterior, int &n, vector<int> &posicaoParagrafo, vector<ParagraphInfo> &vetorParagrafos, bool ultimo_Paragrafo, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes);

void imprimir_vetorPalavras();

void updateSentenca();

bool verificar(const vector<WordInfo> palavrasNaSentenca, const string str);

bool verificar2(const vector<WordInfo> palavrasNoparagrafo, const string str);

int removerAcento(const string &word);

void iniciar_output(ofstream &file_OutPut);

void output_DistanciaPalavraParagrafo(ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes);

void output_CadaParagrafo(ParagraphInfo paragrafo_output, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes);

void output_CadaSentenca(ParagraphInfo paragrafo_output, int i, int j, ofstream &file_OutPut);

void output_Full(ofstream &file_OutPut, map<string, WordInfo> glossary, unordered_map<string, vector<pair<int, int>>> tabelaHashPosicoes, vector<ParagraphInfo> vetorParagrafos);

#endif
