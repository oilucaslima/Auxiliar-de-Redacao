#include "header.hpp"
#include "models.hpp"
#include "functions.hpp"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <time.h>

using namespace std;

int main()
{
  clock_t startTime, endTime;
  startTime = clock();

  vector<int> posicaoParagrafo;
  vector<ParagraphInfo> vetorParagrafos;
  unordered_map<string, vector<pair<int, int>>> tabelaHashPosicoes;
  ofstream file_OutPut("output/output.txt");
  ifstream file;
  ifstream fileStopWords;
  ifstream fileExpressoes;
  map<string, WordInfo> glossary;
  unordered_map<string, WordInfo> glossaryStopWords;
  Aho ahoTrie;
  
  ahoTrie.add_expressions("data/expressao.txt");
  ahoTrie.prepare();

  posicaoParagrafo.push_back(1);

  int n = 1;
  string str = "";
  string strExpressoes;
  char cAnterior, cAnteriorAnterior;

  fileStopWords.open("data/stopwords.txt", ios::in);
  while (fileStopWords)
  {
    getline(fileStopWords, str, '\n');
    addStopWord(glossaryStopWords, str, n);
  }
  fileStopWords.close();

  char c = '\0';
  string text;
  file.open("data/entrada.txt");
  iniciar_output(file_OutPut);
  while (!file.eof())
  {
    cAnteriorAnterior = cAnterior;
    cAnterior = c;
    c = file.get();
    text += c;
    returnWord(c, cAnterior, cAnteriorAnterior, n, str, glossary, posicaoParagrafo, glossaryStopWords, vetorParagrafos, tabelaHashPosicoes, file, file_OutPut);
  }

  ahoTrie.process_text_file(text, file_OutPut);

  file.close();
  int cont = 0;
  for (int i = 0; i < (int)vetorParagrafos.size(); i++)
  {
    vetorParagrafos[i].linhaInicial = posicaoParagrafo[cont];
    vetorParagrafos[i].linhaFinal = posicaoParagrafo[cont + 1];
    cont += 2;
  }
  cout << endl;

  output_Full(file_OutPut, glossary, tabelaHashPosicoes, vetorParagrafos);
  ahoTrie.output_expressao(file_OutPut);
  file_OutPut.close();
  fileStopWords.close();
  fileExpressoes.close();

  endTime = clock();
  clock_t elapsedTime = endTime - startTime;
  double elapsedTimeMs = ((double)elapsedTime/CLOCKS_PER_SEC)*1000;
  cout << "TEMPO DE EXECUÇÃO: " << elapsedTimeMs << " ms " << endl;
  return 0;
}
