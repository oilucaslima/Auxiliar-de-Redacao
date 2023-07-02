#include "aho.hpp"

Aho::Aho()
        : trie{}, words_length{}, size{0}, root{0}, wordID{0}
{
    trie.push_back(Vertex{});
    ++size;
}

void Aho :: add_string(const std::string &s)
{
    int curVertex = root;

    for (char c : s)
    {
        if (trie[curVertex].children.find(c) == trie[curVertex].children.end())
        {
            trie.push_back(Vertex{});

            trie[size].parent = curVertex;
            trie[size].parent_char = c;
            trie[curVertex].children[c] = size;
            size++;
        }

        curVertex = trie[curVertex].children[c];
    }

    trie[curVertex].leaf = true;
    trie[curVertex].word_ID = wordID;
    words_length.push_back(s.size());

    ++wordID;
}

void Aho::prepare()
{
    std::queue<int> vertexQueue{};
    vertexQueue.push(root);

    while (!vertexQueue.empty())
    {
        int curVertex = vertexQueue.front();
        vertexQueue.pop();

        CalcSuffLink(curVertex);

        for (const auto &p : trie[curVertex].children)
            vertexQueue.push(trie[curVertex].children[p.first]);
    }
}

void Aho::process(const std::string &text, std::ofstream &file_OutPut)
{
    int currentState = root;
    int currentParagraph = 1;

    std::vector<expressoes> expressions; // Vetor para armazenar as expressões e seus números de parágrafo

    for (std::size_t j = 0; j < text.size(); j++)
    {
        if (trie[currentState].children.find(text[j]) == trie[currentState].children.end())
        {
            currentState = root;

            // Verifica se são duas quebras de linha consecutivas para contar um novo parágrafo
            if (text[j] == '\n' && j < text.size() - 1 && text[j + 1] == '\n')
            {
                currentParagraph++;
            }

            continue;
        }

        while (true)
        {
            currentState = trie[currentState].children[text[j]];
            break;
        }

        int checkState = currentState;

        while (true)
        {
            checkState = trie[checkState].end_word_link;

            if (checkState == root)
                break;

            int indexOfMatch = j + 1 - words_length[trie[checkState].word_ID];
            occurrences[text.substr(indexOfMatch, words_length[trie[checkState].word_ID])].push_back(getLineNumber(text, indexOfMatch));

            // Obtém a palavra correspondente
            std::string word = text.substr(indexOfMatch, words_length[trie[checkState].word_ID]);

            // Verifica se a expressão já está no vetor
            auto it = find_if(expressions.begin(), expressions.end(), [&](const expressoes &expr)
                                    { return expr.word == word && expr.paragafo == currentParagraph; });

            if (it != expressions.end())
            {
                // A expressão já está no vetor, atualiza as ocorrências e linhas
                it->ocorrencias++;
                it->linhas.push_back(getLineNumber(text, indexOfMatch));
            }
            else
            {
                // A expressão não está no vetor, adiciona uma nova entrada
                expressoes newExpression;
                newExpression.paragafo = currentParagraph;
                newExpression.ocorrencias = 1;
                newExpression.linhas.push_back(getLineNumber(text, indexOfMatch));
                newExpression.word = word;
                expressions.push_back(newExpression);
            }

            checkState = trie[checkState].suffix_link;
        }
    }

    // Escreve as expressões no arquivo de saída, separadas por parágrafos
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << std::endl;
    file_OutPut << "                                                        EXPRESSOES	PARCIAIS		    	   " << std::endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << std::endl;
    file_OutPut << "EXPRESSOES			    	PARAGRAFOS                           APPEARANCES 	               LINE 		   " << std::endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (const auto &expression : expressions)
    {
        if (expression.paragafo != currentParagraph)
        {
            // Insere uma quebra de linha para separar os parágrafos
            file_OutPut << "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
            currentParagraph = expression.paragafo;
        }

        // Escreve a expressão no arquivo de saída
        file_OutPut << std::left << std::setw(32) << expression.word << std::left << std::setw(32);        // palavra
        file_OutPut << std::left << std::setw(32) << expression.paragafo << std::left << std::setw(32);    // Paragrafo
        file_OutPut << std::left << std::setw(32) << expression.ocorrencias << std::left << std::setw(32); // ocorrencias
        for (std::size_t i = 0; i < expression.linhas.size(); i++)
        {
            file_OutPut << std::left << std::setw(6) << expression.linhas[i] << std::left; // linha
            if (i != expression.linhas.size() - 1)
            {
                file_OutPut << ", ";
            }
        }
        file_OutPut << std::endl;
    }
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________________" << endl;
}

void Aho::add_expressions(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string expression;
    while (std::getline(file, expression))
    {
        add_string(expression);
    }

    file.close();
}

void Aho::process_text_file(std::string text, std::ofstream &file_OutPut)
{
    process(text, file_OutPut);
    //print_matches();
}

void Aho :: print_matches()
{
    std::cout << "Expressões:" << std::endl;
    for (const auto &occurrence : occurrences)
    {
        std::cout << occurrence.first << ":" << std::endl;
        std::cout << "\t OCORRENCIAS: " << occurrence.second.size() << std::endl;
        std::cout << "\t REFERENCIAS: {";
        for (std::size_t i = 0; i < occurrence.second.size(); i++)
        {
            std::cout << " " << occurrence.second[i];
            if (i != occurrence.second.size() - 1)
            {
                std::cout << ",";
            }
        }
        std::cout << "}" << std::endl;
    }
}

void Aho::output_expressao(ofstream &file_OutPut)
{
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________" << endl;
    file_OutPut << "EXPRESSOES			    	 APPEARANCES 		                            LINE 		   " << endl;
    file_OutPut << "-----------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;

    for (const auto &occurrence : occurrences)
    {
        file_OutPut << left << setw(32) << occurrence.first << left << setw(32);
        file_OutPut << left << setw(45) << occurrence.second.size() << setw(45);
        for (std::size_t i = 0; i < occurrence.second.size(); i++)
        {
            file_OutPut << left << setw(6) << occurrence.second[i];
        }
        file_OutPut << endl;
    }
    file_OutPut << "_________________________________________________________________________________________________________________________________________________________________________" << endl;
}

void Aho :: CalcSuffLink(int vertex)
    {
        if (vertex == root)
        {
            trie[vertex].suffix_link = root;
            trie[vertex].end_word_link = root;
            return;
        }

        if (trie[vertex].parent == root)
        {
            trie[vertex].suffix_link = root;

            if (trie[vertex].leaf)
                trie[vertex].end_word_link = vertex;
            else
                trie[vertex].end_word_link = trie[trie[vertex].suffix_link].end_word_link;

            return;
        }

        int curBetterVertex = trie[trie[vertex].parent].suffix_link;
        char chVertex = trie[vertex].parent_char;

        while (true)
        {
            if (trie[curBetterVertex].children.find(chVertex) != trie[curBetterVertex].children.end())
            {
                trie[vertex].suffix_link = trie[curBetterVertex].children[chVertex];
                break;
            }

            if (curBetterVertex == root)
            {
                trie[vertex].suffix_link = root;
                break;
            }

            curBetterVertex = trie[curBetterVertex].suffix_link;
        }

        if (trie[vertex].leaf)
            trie[vertex].end_word_link = vertex;
        else
            trie[vertex].end_word_link = trie[trie[vertex].suffix_link].end_word_link;
    }

    int Aho :: getLineNumber(const std::string &text, int index) const
    {
        int lineNumber = 1;

        for (int i = 0; i < index; i++)
        {
            if (text[i] == '\n')
                lineNumber++;
        }

        return lineNumber;
    }