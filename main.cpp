#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <tuple>

using namespace std;

// Structure pour représenter une arête
struct Edge {
    int src, dest;
};

// Prototypes des fonctions
pair<int, vector<int>> longestIncreasingSubsequence(const vector<int>& seq);
tuple<int, vector<int>, int> max_subsequence_divisible_by_k(const vector<int>& seq, int k);
vector<Edge> kruskal(int n, const vector<Edge>& edges);
vector<Edge> readGraph(const string& filename, int& n, int& m);
void writeTree(const string& filename, const vector<Edge>& tree);

// Fonction pour trouver la plus longue sous-séquence croissante
pair<int, vector<int>> longestIncreasingSubsequence(const vector<int>& seq) {
    int n = seq.size();
    vector<int> dp(n, 1); // dp[i] représente la longueur de la liste jusqu'à l'élément i
    vector<int> prev(n, -1); // prev[i] stocke l'index du précédent élément dans la liste
    int maxLength = 1;
    int endIndex = 0; // L'indice de l'élément final de la liste

    // Remplir dp et prev
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (seq[i] > seq[j] && dp[i] < dp[j] + 1) {
                dp[i] = dp[j] + 1;
                prev[i] = j;
            }
        }
        // Mettre à jour la longueur maximale et l'indice final
        if (dp[i] > maxLength) {
            maxLength = dp[i];
            endIndex = i;
        }
    }

    // Reconstruire la sous-séquence croissante
    vector<int> lis;
    for (int i = endIndex; i >= 0; i = prev[i]) {
        lis.push_back(seq[i]);
        if (prev[i] == -1) break;
    }
    reverse(lis.begin(), lis.end()); // Inverser pour obtenir l'ordre correct

    return {maxLength, lis};
}

// Fonction pour trouver la plus longue sous-séquence dont la somme est divisible par k
tuple<int, vector<int>, int> max_subsequence_divisible_by_k(const vector<int>& seq, int k) {
    int n = seq.size();
    vector<int> dp(k, -1); // dp[i] représente la longueur de la liste jusqu'à l'élément i
    vector<vector<int>> track(k); // track[i] représente le reste
    vector<vector<int>> indices(k); // indices[i] stocke l'index

    dp[0] = 0;

    // Remplir dp et track
    for (int i = 0; i < n; ++i) {
        vector<int> next_dp = dp;
        vector<vector<int>> next_track = track;
        vector<vector<int>> next_indices = indices;

        // Mettre à jour dp en fonction des différents restes
        for (int r = 0; r < k; ++r) {
            if (dp[r] != -1) {
                int new_r = (r + seq[i]) % k; // Calculer le nouveau reste

                if (next_dp[new_r] < dp[r] + 1) {
                    next_dp[new_r] = dp[r] + 1;
                    next_track[new_r] = track[r];
                    next_track[new_r].push_back(seq[i]);
                    next_indices[new_r] = indices[r];
                    next_indices[new_r].push_back(i + 1);
                }
            }
        }

        // Mettre à jour la longueur maximale et l'indice final
        dp = next_dp;
        track = next_track;
        indices = next_indices;
    }

    // Reconstruire la sous-séquence croissante
    vector<int> result = track[0];
    vector<int> result_indices = indices[0];
    int max_length = result.size();

    // Calculer la somme finale
    int sum = 0;
    for (int num : result) {
        sum += num;
    }
    return make_tuple(sum, result_indices, max_length);
}



vector<Edge> readGraph(const string& filename, int& n, int& m) {
    vector<Edge> edges;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    file >> n >> m; // Lire le nombre de sommets et d'arêtes
    for (int i = 0; i < m; ++i) {
        Edge edge;
        file >> edge.src >> edge.dest; // Lire les arêtes
        edges.push_back(edge);
    }
    return edges;
}

void writeTree(const string& filename, const vector<Edge>& tree) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    // Écriture des arêtes de l'arbre
    for (const auto& edge : tree) {
        file << edge.src << " " << edge.dest << endl;
    }
}

vector<Edge> kruskal(int n, const vector<Edge>& edges) {
    vector<Edge> mst; // Arbre de recouvrement minimal


    return mst;
}


void exercice1() {
    string inputFileName = "INPMONOSEQ.TXT";
    string outputFileName = "OUTMONOSEQ.TXT";

    int n;

    // Lecture du fichier
    ifstream file(inputFileName);
    if (!file.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    string line;
    getline(file, line); // Lecture du nombre n
    if (line.empty()) {
        cerr << "The first line is empty!" << endl;
        exit(EXIT_FAILURE);
    }

    try {
        n = std::stoi(line); // Taille de la séquence
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        exit(EXIT_FAILURE);
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        exit(EXIT_FAILURE);
    }

    getline(file, line); // Lecture de la séquence d'entiers
    stringstream ss(line);
    int num;
    vector<int> seq;

    while (ss >> num) {
        seq.push_back(num);
    }

    // Recherche de la sous-séquence
    auto result = longestIncreasingSubsequence(seq);
    int maxLength = result.first;
    vector<int> lis = result.second;

    // Écriture du résultat dans le fichier de sortie
    ofstream fileout(outputFileName);
    if (!fileout.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    // Écrire la longueur de la sous-séquence
    fileout << maxLength << endl;

    // Écrire les éléments et leurs indices dans la séquence d'origine
    for (int i = 0; i < lis.size(); ++i) {
        int index = find(seq.begin(), seq.end(), lis[i]) - seq.begin() + 1;
        fileout << "a[" << index << "] = " << lis[i] << endl;
    }

    cout << "Result written to " << outputFileName << endl;
}

void exercice2() {
    string inputFileName = "INPDIVSEQ.TXT";
    string outputFileName = "OUTDIVSEQ.TXT";

    int n;
    ifstream file(inputFileName);
    if (!file.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    string line;

    // Lecture de la première ligne : n et k
    getline(file, line);
    stringstream ss(line);
    int k;
    ss >> n >> k;

    // Lecture de la deuxième ligne : la séquence d'entiers
    getline(file, line);
    stringstream ssSeq(line);
    vector<int> seq;
    int num;
    while (ssSeq >> num) {
        seq.push_back(num);
    }

    auto [sum, result_indices, max_length] = max_subsequence_divisible_by_k(seq, k);
    // Écrire dans le fichier de sortie
    ofstream fileout(outputFileName);
    if (!fileout.is_open()) {
        cerr << "Unable to open the file" << endl;
        exit(EXIT_FAILURE);
    }

    fileout << max_length << endl;
    for (size_t i = 0; i < max_length; ++i) {
        fileout << "a[" << result_indices[i] << "] = " << seq[result_indices[i] - 1] << endl; // Utiliser seq ici
    }
    fileout << "Sum = " << sum << endl;

    cout << "Result written to " << outputFileName << endl;
}

void exercice3() {
    int n, m;
    vector<Edge> edges = readGraph("INPARBGRAPH.TXT", n, m);
    vector<Edge> mst = kruskal(n, edges);
    writeTree("OUTARBGRAPH.TXT", mst);
}



int main() {
    exercice1(); // Appel de la première fonction d'exercice
    exercice2(); // Appel de la deuxième fonction d'exercice
    exercice3(); // Appel de la troisième fonction d'exercice
    return 0;
}
