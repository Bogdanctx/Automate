#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

class L_NFA {
public:
    L_NFA() = default;
    ~L_NFA() = default;

    void citesteStari(std::ifstream &fin);
    void citesteTranzitii(std::ifstream &fin);
    void citesteStareInitiala(std::ifstream &fin);
    void citesteStariFinale(std::ifstream &fin);

    int getStareInitiala() const { return m_stareInitiala; };

    void isAccepted(bool &result, const std::string &word, int startNode, int wordIndex);

private:
    std::unordered_map<int, std::vector<std::pair<int, int>>> m_graf;
    std::unordered_set<int> m_stariFinale;
    int m_stareInitiala{};
    std::unordered_map<int, std::unordered_map<int, bool>> visitedNodes;
};

void L_NFA::isAccepted(bool &result, const std::string &word, const int startNode, const int wordIndex = 0) {
    if(startNode == m_stareInitiala) { // daca a inceput verificarea resetez nodurile vizitate
        visitedNodes = std::unordered_map<int, std::unordered_map<int, bool>>();
    }

    if(wordIndex == word.length()) {
        if(m_stariFinale.find(startNode) != m_stariFinale.end()) {
            result = true;
        }
        else {
            for(const auto &it: m_graf[startNode]) {
                if(it.second == '-') {
                    isAccepted(result, word, it.first, wordIndex);
                }
            }
        }
    }
    else {
        for(const auto &it: m_graf[startNode]) {
            if(it.second == word[wordIndex] && visitedNodes[it.first][wordIndex] == 0) {
                visitedNodes[it.first][wordIndex] = true;
                isAccepted(result, word, it.first, wordIndex + 1);
            }
            if(it.second == '-') {
                isAccepted(result, word, it.first, wordIndex);
            }

        }
    }
}

void L_NFA::citesteStareInitiala(std::ifstream &fin) {
    fin>>m_stareInitiala;
}

void L_NFA::citesteStariFinale(std::ifstream &fin) {
    int numarStariFinale;
    fin>>numarStariFinale;
    for(int i = 0; i < numarStariFinale; i++) {
        int stareFinala;
        fin>>stareFinala;
        m_stariFinale.insert(stareFinala);
    }
}

void L_NFA::citesteStari(std::ifstream &fin) {
    int numarStari;
    fin>>numarStari;
    for(int i = 0; i < numarStari; i++) {
        int stare;
        fin>>stare;
        m_graf[stare] = {};
    }
}

void L_NFA::citesteTranzitii(std::ifstream &fin) {
    int numarTranzitii;
    fin>>numarTranzitii;
    for(int i = 0; i < numarTranzitii; i++) {
        int x, y;
        char l;
        fin >> x >> y >> l;
        m_graf[x].emplace_back(y, l);
    }
}

int main() {
    std::ifstream fin("../input.txt");
    L_NFA l_nfa;

    l_nfa.citesteStari(fin);
    l_nfa.citesteTranzitii(fin);
    l_nfa.citesteStareInitiala(fin);
    l_nfa.citesteStariFinale(fin);

    int nrCuvinte;

    fin>>nrCuvinte;

    for(int i = 0; i < nrCuvinte; i++) {
        std::string cuvant;
        fin>>cuvant;

        bool wasAccepted = false;
        l_nfa.isAccepted(wasAccepted, cuvant, l_nfa.getStareInitiala());

        std::cout<<wasAccepted<<'\n';
    }

    fin.close();

    return 0;
}