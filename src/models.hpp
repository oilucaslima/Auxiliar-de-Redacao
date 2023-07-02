#ifndef MODELS_HPP
#define MODELS_HPP
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "header.hpp"
using namespace std;

// hash map info
typedef struct WordInfo
{
  string word;
  int occurrences;
  int qtd_letras;
  int primeiraLinha;
  vector<int> references; // linhas
  vector<int> posicoes;
  vector<int> paragrafos;
  vector<pair<int, int>> sentencas;

} WordInfo;

typedef struct Sentenca
{
  int nPalavras, nStopWords;
  vector<WordInfo> palavras;
} Sentenca;

typedef struct ParagraphInfo
{
  int linhaInicial, linhaFinal, nSentencas;
  vector<Sentenca> sentencas;
  vector<Sentenca> sentencas_SemRepeticao;
} ParagraphInfo;

typedef struct Expression
{
  int occurrences;
  vector<int> references;
} Expression;

#endif