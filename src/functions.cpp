#include "functions.hpp"
#include <ctype.h>
#include <algorithm>

int aux = 0, numero_deSenteca, contIndiceParagrafo = 0, contadorPalavrasTotal = 0, contadorStopWordTotal = 0;
int contadorLinhaVazia = 0, auxPrimeiroParagrafo = 1, contadorParagrafo = 1, contadorStopWords = 0, contadorSentenca = 1, contadorPosicaoPalavra = 1, contadorPalavrasPorSentenca = 0;
vector<WordInfo> vetorPalavrasPorSentenca;
vector<WordInfo> vetorPalavrasParagrafoSemREpeticao;
vector<WordInfo> vetorPalavrasPorSentencaSemRepeticao;
vector<Sentenca> vetorSentenca;
vector<Sentenca> vetorSentencaSemRepeticao;

void existentWord(string str, int n, map<string, WordInfo> &glossary)
{
  glossary[str].occurrences++;
  glossary[str].references.push_back(n);
}

void existentWordUnorderedMap(unordered_map<string, WordInfo> &glossaryStopWords, string str, int n)
{
  glossaryStopWords[str].occurrences++;
  glossaryStopWords[str].references.push_back(n);
}

void existePalavraNaSentenca(string str, int n)
{

  int indice;

  auto iterador = find_if(vetorPalavrasPorSentencaSemRepeticao.begin(), vetorPalavrasPorSentencaSemRepeticao.end(), [str](const WordInfo &palavra)
                          { return palavra.word == str; });

  if (iterador != vetorPalavrasPorSentencaSemRepeticao.end())
  {
    indice = distance(vetorPalavrasPorSentencaSemRepeticao.begin(), iterador);
    vetorPalavrasPorSentencaSemRepeticao[indice].occurrences++;
    vetorPalavrasPorSentencaSemRepeticao[indice].references.push_back(n);
    vetorPalavrasPorSentencaSemRepeticao[indice].posicoes.push_back(contadorPosicaoPalavra);
  }
}

bool verificar(const vector<WordInfo> palavrasNaSentenca, const string str)
{

  for (const auto &palavra : palavrasNaSentenca)
  {
    if (palavra.word == str)
    {
      return true;
    }
  }

  return false;
}

void existePalavraNoParagrafo(string str, int n)
{

  int indice;

  auto iterador = find_if(vetorPalavrasParagrafoSemREpeticao.begin(), vetorPalavrasParagrafoSemREpeticao.end(), [str](const WordInfo &palavra)
                          { return palavra.word == str; });

  if (iterador != vetorPalavrasParagrafoSemREpeticao.end())
  {
    indice = distance(vetorPalavrasParagrafoSemREpeticao.begin(), iterador);
    vetorPalavrasParagrafoSemREpeticao[indice].occurrences++;
    vetorPalavrasParagrafoSemREpeticao[indice].references.push_back(n);
    vetorPalavrasParagrafoSemREpeticao[indice].posicoes.push_back(contadorPosicaoPalavra);
  }
}

bool verificar2(const vector<WordInfo> palavrasNoparagrafo, const string str)
{

  for (const auto &palavra : palavrasNoparagrafo)
  {
    if (palavra.word == str)
    {
      return true;
    }
  }

  return false;
}

void addVectorPalavraNaSentenca(WordInfo newWord, string str, int n)
{

  bool var = verificar(vetorPalavrasPorSentencaSemRepeticao, str);

  if (var == true)
  {
    newWord.word = str;
    vetorPalavrasPorSentenca.push_back(newWord);
    existePalavraNaSentenca(str, n);
  }
  else
  {
    newWord.word = str;
    newWord.occurrences++;
    newWord.posicoes.push_back(contadorPosicaoPalavra);
    newWord.references.push_back(n);
    vetorPalavrasPorSentenca.push_back(newWord);
    vetorPalavrasPorSentencaSemRepeticao.push_back(newWord);
  }
}

void addVectorParagrafo(string str, int n)
{

  WordInfo newWord2;

  bool var2 = verificar2(vetorPalavrasParagrafoSemREpeticao, str);

  if (var2 == true)
  {
    existePalavraNoParagrafo(str, n);
  }
  else
  {
    newWord2.word = str;
    newWord2.occurrences = 1;
    newWord2.posicoes.push_back(contadorPosicaoPalavra);
    newWord2.references.push_back(n);
    vetorPalavrasParagrafoSemREpeticao.push_back(newWord2);
  }
}

void updateWord(WordInfo newWord, map<string, WordInfo> &glossary, string &str, unordered_map<string, WordInfo> &glossaryStopWords, int &n, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes)
{
  int i = contadorParagrafo;

  if (str != "")
  {

    contadorPalavrasTotal++;

    if (glossary.find(str) != glossary.end())
    {
      existentWord(str, n, glossary);
      addVectorPalavraNaSentenca(newWord, str, n);
      addVectorParagrafo(str, n);
      tabelaHashPosicoes[str].push_back(make_pair(contadorParagrafo, contadorPosicaoPalavra));
      auto iterador = find_if(glossary[str].paragrafos.begin(), glossary[str].paragrafos.end(), [i](int element)
                              { return element == i; });
      if (iterador == glossary[str].paragrafos.end())
      {
        glossary[str].paragrafos.push_back(contadorParagrafo);
      }
    }
    else
    {
      glossary[str] = newWord;
      glossary[str].references.push_back(n);
      addVectorPalavraNaSentenca(newWord, str, n);
      addVectorParagrafo(str, n);
      tabelaHashPosicoes[str].push_back(make_pair(contadorParagrafo, contadorPosicaoPalavra));
      glossary[str].paragrafos.push_back(contadorParagrafo);
    }

    glossary[str].sentencas.push_back(make_pair(contadorParagrafo, contadorSentenca));

    if (glossaryStopWords.find(str) != glossaryStopWords.end())
    {
      contadorStopWords++;
      contadorStopWordTotal++;
    }
  }

  str = "";
}

void updateSentenca()
{

  Sentenca *novaSentenca = new Sentenca();
  Sentenca *novaSentencaSemRepeticao = new Sentenca();

  contadorSentenca++;
  novaSentenca->nPalavras = vetorPalavrasPorSentenca.size();
  novaSentenca->nStopWords = contadorStopWords;
  novaSentenca->palavras = vetorPalavrasPorSentenca;
  novaSentencaSemRepeticao->palavras = vetorPalavrasPorSentencaSemRepeticao;
  vetorPalavrasPorSentenca.clear();
  vetorPalavrasPorSentencaSemRepeticao.clear();
  contadorStopWords = 0;
  contadorPalavrasPorSentenca = 0;
  vetorSentenca.push_back(*novaSentenca);
  vetorSentencaSemRepeticao.push_back(*novaSentencaSemRepeticao);
  delete novaSentenca;
  delete novaSentencaSemRepeticao;
}

void updateParagrafo(char c, char cAnterior, char cAnteriorAnterior, vector<int> &posicaoParagrafo, int &n, vector<ParagraphInfo> &vetorParagrafos, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes)
{

  if (isspace(c) && (c != ' ') && (cAnterior == '\n'))
  {
    contadorLinhaVazia++;
  }

  if (auxPrimeiroParagrafo == 1)
  {
    auxPrimeiroParagrafo = 0;
    if ((cAnterior == '\n') && ((cAnteriorAnterior != '!') && (cAnteriorAnterior != '.') && (cAnteriorAnterior != '?')))
    {
      updateSentenca();
    }
  }

  if (contadorLinhaVazia == 1)
  {
    posicaoParagrafo.push_back(n - 2); // salvando ultima linha ...
    posicaoParagrafo.push_back(n);
    vetorPalavrasParagrafoSemREpeticao[0].primeiraLinha = posicaoParagrafo[contIndiceParagrafo];
    contIndiceParagrafo += 2;
    alocarParagrafo(cAnterior, cAnteriorAnterior, n, posicaoParagrafo, vetorParagrafos, false, file_OutPut, tabelaHashPosicoes);
  }
}

void alocarParagrafo(char cAnterior, char cAnteriorAnterior, int &n, vector<int> &posicaoParagrafo, vector<ParagraphInfo> &vetorParagrafos, bool ultimo_Paragrafo, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes)
{

  ParagraphInfo *novoParagrafo = new ParagraphInfo();
  int cont = 0;

  if ((ultimo_Paragrafo == true) && (n > 1))
  {
    posicaoParagrafo.push_back(n - 1);
  }

  if ((cAnterior == '#') && (cAnteriorAnterior != ' '))
  {
    cont++;
  }

  // imprimir_vetorPalavras();

  novoParagrafo->nSentencas = contadorSentenca;
  novoParagrafo->sentencas = vetorSentenca;
  novoParagrafo->sentencas_SemRepeticao = vetorSentencaSemRepeticao;
  vetorSentenca.clear();
  vetorSentencaSemRepeticao.clear();
  contadorLinhaVazia = 0;
  contadorPosicaoPalavra = 1;
  numero_deSenteca = contadorSentenca;
  output_CadaParagrafo(*novoParagrafo, file_OutPut, tabelaHashPosicoes);
  vetorPalavrasParagrafoSemREpeticao.clear();
  contadorSentenca = 0;
  contadorParagrafo++;
  vetorParagrafos.push_back(*novoParagrafo);

  delete novoParagrafo;
}

void returnWord(char c, char cAnterior, char cAnteriorAnterior, int &n, string &str, map<string, WordInfo> &glossary, vector<int> &posicaoParagrafo, unordered_map<string, WordInfo> &glossaryStopWords, vector<ParagraphInfo> &vetorParagrafos, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes, ifstream &file, ofstream &file_OutPut)
{

  WordInfo newWord;
  newWord.occurrences = 0;

  if (file.eof())
  {
    bool ultimo_Paragrafo = true;
    vetorPalavrasParagrafoSemREpeticao[0].primeiraLinha = posicaoParagrafo[contIndiceParagrafo];
    contIndiceParagrafo += 2;
    alocarParagrafo(cAnterior, cAnteriorAnterior, n, posicaoParagrafo, vetorParagrafos, ultimo_Paragrafo, file_OutPut, tabelaHashPosicoes);
  }

  switch (c)
  {
  case ' ':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    contadorPosicaoPalavra++;
    break;

  case '\n':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    contadorPosicaoPalavra++;
    n++;
    break;

  case '.':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    updateSentenca();
    break;

  case '!':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    updateSentenca();
    break;

  case '?':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    updateSentenca();
    break;

  case ',':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case ':':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case ';':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '[':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case ']':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '(':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case ')':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '"':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '+':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '/':
    updateWord(newWord, glossary, str, glossaryStopWords, n, tabelaHashPosicoes);
    break;

  case '\x9D':
    break;

  default:

    if ((c != '\xE2') && (c != '\x80') && (c != '\x9D') && (c != '\x9C') && (c != '\x94'))
    {
      str += tolower(c);
    }

    if (str == "--" || str == "-" || str == "—" || str == "”" || str == "“" || str == "-”")
    {
      str = "";
    }

    break;
  }

  updateParagrafo(c, cAnterior, cAnteriorAnterior, posicaoParagrafo, n, vetorParagrafos, file_OutPut, tabelaHashPosicoes);
}

void addStopWord(unordered_map<string, WordInfo> &glossaryStopWords, string &str, int n)
{
  WordInfo newWord;
  newWord.occurrences = 1;

  if (str != "")
  {
    if (glossaryStopWords.find(str) != glossaryStopWords.end())
    {
      existentWordUnorderedMap(glossaryStopWords, str, n);
    }
    else
    {
      glossaryStopWords[str] = newWord;
    }
  }

  str = "";
}

void iniciar_output(ofstream &file_OutPut)
{
  file_OutPut << "=================================================================================================================================================================" << endl;
  file_OutPut << "                                                            ### PARTIAL RESULT ###                                                           " << endl;
  file_OutPut << "=================================================================================================================================================================" << endl;
}

void output_CadaParagrafo(ParagraphInfo paragrafo_output, ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes)
{

  int size = paragrafo_output.sentencas_SemRepeticao.size();
  int comStopWord, semStopWord;

  for (int i = 0; i < size; i++)
  {
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << endl;
    file_OutPut << "WORD 			            PARAGRAPH 		       SENTENCE 		      APPEARANCES 		          LINE 		               POSITIONS         " << endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (int j = 0; j < (int)paragrafo_output.sentencas_SemRepeticao[i].palavras.size(); j++)
    {
      output_CadaSentenca(paragrafo_output, i, j, file_OutPut);
    }

    comStopWord = paragrafo_output.sentencas[i].nPalavras;
    semStopWord = comStopWord - paragrafo_output.sentencas[i].nStopWords;

    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << endl;
    file_OutPut << "=> Number of words with stop words: " << comStopWord << "                                                       => Number of words without stop words: " << semStopWord << endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
  }

  output_DistanciaPalavraParagrafo(file_OutPut, tabelaHashPosicoes);
}

void output_DistanciaPalavraParagrafo(ofstream &file_OutPut, unordered_map<string, vector<pair<int, int>>> &tabelaHashPosicoes)
{

  tabelaHashPosicoes["a"].push_back(make_pair(contadorParagrafo, contadorPosicaoPalavra));

  // imprimir_vetorPalavras();

  string str2 = "";
  string distancia = "";
  int result;

  file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << endl;
  file_OutPut << "                                                          DISTANCE  PARAGRAPH   " << contadorParagrafo << "       		               		                            " << endl;
  file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

  for (int i = 0; i < (int)vetorPalavrasParagrafoSemREpeticao.size(); i++)
  {
    if (vetorPalavrasParagrafoSemREpeticao[i].occurrences > 1)
    {
      for (int j = 1; j < (int)vetorPalavrasParagrafoSemREpeticao[i].posicoes.size(); j++)
      {
        result = vetorPalavrasParagrafoSemREpeticao[i].posicoes[j] - vetorPalavrasParagrafoSemREpeticao[i].posicoes[0];
        distancia += to_string(result) + " - ";
      }
      str2 = vetorPalavrasParagrafoSemREpeticao[i].word;
      file_OutPut << str2 << " ------------=> " << distancia << endl;
      distancia.clear();
    }
  }

  int numero_deParagrafo = vetorPalavrasParagrafoSemREpeticao[0].primeiraLinha;

  file_OutPut << ".................................................................................................................................................................." << endl;

  file_OutPut << "==================================================================================================================================================================" << endl;
  file_OutPut << "=> Beginning paragraph in line: " << numero_deParagrafo << "                 "
              << "  Number of sentences: " << numero_deSenteca << endl;
  file_OutPut << "==================================================================================================================================================================" << endl;
  file_OutPut << endl
              << endl
              << endl;
}

int removerAcento(const string &word)
{

  string result = word;
  int cont = 0;

  // Mapeia caracteres acentuados e cedilha para seus equivalentes sem acento
  string acentos = "áàâãäéèêëíìîïóòôõöúùûüç";

  // Remove acentos
  for (int i = 0; i < (int)result.length(); ++i)
  {
    char c = result[i];
    int aux = acentos.find(c);
    if (aux)
    {
      cont++;
    }
  }

  return cont;
}

void output_CadaSentenca(ParagraphInfo paragrafo_output, int i, int j, ofstream &file_OutPut)
{

  string palavra = "", paragrafo = "", sentenca = "", linhaEspaco = "", aux = "", aux2 = "";
  string linhas = "", posicao = "", atual = "", anterior = "", ocorrencia = "", distancia = "";
  int tamanho_Palavra, funcao_EspacoPalavra, funcao_EspacoParagrafo, funcao_EspacoSentenca;
  int funcao_EspacOcorrencia, funcao_EspacoLinha, ocorrencia2;
  char espaco = ' ';

  palavra = paragrafo_output.sentencas_SemRepeticao[i].palavras[j].word;
  tamanho_Palavra = removerAcento(palavra);
  palavra = paragrafo_output.sentencas_SemRepeticao[i].palavras[j].word;
  funcao_EspacoPalavra = (26 - tamanho_Palavra);
  palavra.append(funcao_EspacoPalavra, espaco);

  int qtdCaractere_Paragrafo = to_string(contadorParagrafo).size();
  funcao_EspacoParagrafo = (11 - qtdCaractere_Paragrafo) / 2;
  paragrafo.insert(0, funcao_EspacoParagrafo, espaco);
  aux.append(9, espaco);

  int qtdCarectere_Sentenca = to_string(i).size();
  funcao_EspacoSentenca = (13 - qtdCarectere_Sentenca) / 2;
  sentenca.insert(0, funcao_EspacoSentenca, espaco);
  aux2.append(8, espaco);

  ocorrencia2 = paragrafo_output.sentencas_SemRepeticao[i].palavras[j].occurrences;
  int qtdCarectere_ocorrencias = to_string(ocorrencia2).size();
  funcao_EspacOcorrencia = (13 - qtdCarectere_ocorrencias) / 2;
  ocorrencia.insert(0, funcao_EspacOcorrencia, espaco);

  // file_OutPut << palavra << "|" << paragrafo << contadorParagrafo << paragrafo <<"|" << aux << "|" << sentenca << i+1 << sentenca << "|" << aux << "|"<< ocorrencia << ocorrencia2 << ocorrencia << "|" << aux << "|" << endl;

  for (int k = 0; k < (int)paragrafo_output.sentencas_SemRepeticao[i].palavras[j].references.size(); k++)
  {
    atual = to_string(paragrafo_output.sentencas_SemRepeticao[i].palavras[j].references[k]);
    if (atual != anterior)
    {
      linhas = linhas + atual + " ";
      anterior = atual;
      atual.clear();
    }
  }

  int qtdCarectere_Linha = linhas.size();
  funcao_EspacoLinha = (14 - qtdCarectere_Linha) / 2;
  linhaEspaco.insert(0, funcao_EspacoLinha, espaco);

  for (int k = 0; k < (int)paragrafo_output.sentencas_SemRepeticao[i].palavras[j].posicoes.size(); k++)
  {
    posicao += to_string(paragrafo_output.sentencas_SemRepeticao[i].palavras[j].posicoes[k]) + " ";
    if (k > 0)
    {
      distancia += to_string(paragrafo_output.sentencas_SemRepeticao[i].palavras[j].posicoes[k] - paragrafo_output.sentencas_SemRepeticao[i].palavras[j].posicoes[0]) + " ";
    }
  }

  file_OutPut << palavra << "|" << paragrafo << contadorParagrafo << paragrafo << "|" << aux << "|" << sentenca << i + 1 << sentenca << "|" << aux << "|" << ocorrencia << ocorrencia2 << ocorrencia << "|" << aux << "|";
  file_OutPut << linhaEspaco << linhas << linhaEspaco << "|" << aux << "|" << aux << posicao << endl;
}

void output_Full(ofstream &file_OutPut, map<string, WordInfo> glossary, unordered_map<string, vector<pair<int, int>>> tabelaHashPosicoes, vector<ParagraphInfo> vetorParagrafos)
{
  string str = "", aux = "", aux2 = "";
  int tam = 0, tamAux = 0;

  file_OutPut << "\n\n\n";
  file_OutPut << "=====================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================\n";
  file_OutPut << "=>                                                    ### FULL RESULT ###" << left << setw(265) << ""
              << "### FULL RESULT ###                                                                                                                                                                                                                                                                         \n";
  file_OutPut << "=====================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================================\n";
  file_OutPut << "_____________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
  file_OutPut << left << setw(13) << ""
              << "WORD" << left << setw(22) << ""
              << "APPEARANCES" << left << setw(63) << ""
              << "PARAGRAPH" << left << setw(130) << ""
              << "SENTENCE" << left << setw(119) << ""
              << "LINE" << left << setw(100) << ""
              << "POSITIONS" << endl;
  file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

  for (auto &[key, word] : glossary)
  {

    int tam_paragrafos = word.paragrafos.size();
    int tam_sentencas = word.sentencas.size();
    int tam_referencias = word.references.size();
    int tam_posicoes = tabelaHashPosicoes[key].size();

    int auxPar = 0, auxSen = 0, auxRef = 0, auxPos = 0;
    int stopPar = 0, stopSen = 0, stopRef = 0, stopPos = 0;
    int espacoPar = 0, espacoSen = 0, espacoRef = 0, espacoPos = 0;
    string strPar = "", strSen = "", strRef = "", strPos = "";

    file_OutPut << key;
    tamAux = removerAcento(key);
    tam = 30 - tamAux;
    file_OutPut << left << setw(tam) << "";
    file_OutPut << "|";
    file_OutPut << left << setw(12) << "";

    file_OutPut << word.occurrences + 1;
    tam = 14 - to_string(word.occurrences + 1).size();
    file_OutPut << left << setw(tam) << "";
    file_OutPut << "|";
    file_OutPut << left << setw(16) << "";

    int antPar = 0, proxPar = 14, antSen = 0, proxSen = 6, antRef = 0, proxRef = 14, antPos = 0, proxPos = 14;
    int primeiraVez = 1;

    while ((stopPar == 0) && (stopSen == 0) && (stopRef == 0) && (stopPos == 0))
    {

      if (primeiraVez == 0)
      {
        file_OutPut << "\n";
        file_OutPut << " ";
        file_OutPut << left << setw(29) << "";
        file_OutPut << "|";
        file_OutPut << left << setw(26) << "";
        file_OutPut << "|";
        file_OutPut << left << setw(16) << "";
      }
      strPar = "", strSen = "", strRef = "", strPos = "";

      for (int i = antPar; i <= proxPar; i++)
      {
        if (i < tam_paragrafos)
        {
          strPar += to_string(word.paragrafos[i]) + " ";
        }
        else
        {
          stopPar = 1;
        }
      }
      antPar = proxPar;
      proxPar += 14;

      for (int i = antSen; i <= proxSen; i++)
      {
        if (i < tam_sentencas)
        {
          strSen += (to_string(word.sentencas[i].first) + " - " + to_string(word.sentencas[i].second) + " ; ");
        }
        else
        {
          stopSen = 1;
        }
      }
      antSen = proxSen;
      proxSen += 6;

      for (int i = antRef; i <= proxRef; i++)
      {
        if (i < tam_referencias)
        {
          strRef += to_string(word.references[i]) + " ";
        }
        else
        {
          stopRef = 1;
        }
      }
      antRef = proxRef;
      proxRef += 14;

      for (int i = antPos; i <= proxPos; i++)
      {
        if (i < tam_posicoes)
        {
          strPos += to_string(tabelaHashPosicoes[key][i].second) + " ";
        }
        else
        {
          stopPos = 1;
        }
      }
      antPos = proxPos;
      proxPos += 14;

      auxPar = strPar.size();
      auxSen = strSen.size();
      auxRef = strRef.size(), auxPos = strPos.size();

      espacoPar = 125 - auxPar;
      espacoSen = 125 - (auxSen - 3);
      espacoRef = 125 - auxRef;
      espacoPos = 125 - auxPos;

      file_OutPut << strPar;
      file_OutPut << left << setw(espacoPar - 12) << "";
      file_OutPut << "|";
      file_OutPut << left << setw(12) << "";
      file_OutPut << strSen;
      file_OutPut << left << setw(espacoSen - 12) << "";
      file_OutPut << "|";
      file_OutPut << left << setw(12) << "";
      file_OutPut << strRef;
      file_OutPut << left << setw(espacoRef - 12) << "";
      file_OutPut << "|";
      file_OutPut << left << setw(12) << "";
      file_OutPut << strPos;
      file_OutPut << left << setw(espacoPos) << "";
      file_OutPut << "|";
      primeiraVez = 0;
    }

    file_OutPut << endl;
  }

  file_OutPut << "\n\n";
  file_OutPut << "--------------------------------------------------------------------------------------------" << endl;
  file_OutPut << "_______________________________________________________________________________________________________________________________________\n";
  file_OutPut << "                                       NUMBER OF WORDS IN EACH SENTENCE WITH AND WITHOUT STOP WORDS                                    \n";
  file_OutPut << "---------------------------------------------------------------------------------------------------------------------------------------\n";

  for (int i = 0; i < (int)vetorParagrafos.size(); i++)
  {
    for (int j = 0; j < (int)vetorParagrafos[i].nSentencas; j++)
    {
      file_OutPut << "Paragraph: " << i + 1 << "  Sentence: " << j + 1 << "  Number of words with stop words: " << vetorParagrafos[i].sentencas[j].nPalavras << "  Number of words without stop words: " << vetorParagrafos[i].sentencas[j].nPalavras - vetorParagrafos[i].sentencas[j].nStopWords << endl;
    }
  }
  file_OutPut << "_______________________________________________________________________________________________________________________________________";

  file_OutPut << "\n\n";
  file_OutPut << "_______________________________________________________________________________________________________________________________________\n";
  file_OutPut << "                                               LINE NUMBER THAT START EACH PARAGRAPH                                                   \n";
  file_OutPut << "---------------------------------------------------------------------------------------------------------------------------------------\n";

  for (int i = 0; i < (int)vetorParagrafos.size(); i++)
  {
    file_OutPut << "Paragraph: " << i + 1 << "    Beginning in line: " << vetorParagrafos[i].linhaInicial << "   Number of sentences: " << vetorParagrafos[i].nSentencas << endl;
  }
  file_OutPut << "_______________________________________________________________________________________________________________________________________\n";
  file_OutPut << "\n\n";
  file_OutPut << "_______________________________________________________________________________________________________________________________________\n";
  file_OutPut << "                                                          TOTAL OF WORDS                                                               \n";
  file_OutPut << "---------------------------------------------------------------------------------------------------------------------------------------\n";
  file_OutPut << "Total number of words: " << contadorPalavrasTotal << endl;
  file_OutPut << "Total number of words without stop words: " << contadorPalavrasTotal - contadorStopWordTotal << endl;
  file_OutPut << "_______________________________________________________________________________________________________________________________________\n";
}
