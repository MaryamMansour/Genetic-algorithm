#include <bits/stdc++.h>

using namespace std;
int W, n;
double Pm = 0.1;
double Pc = 0.7;
/// weight , value
vector<pair<int, int>> pack;
struct chromosome {
    string gene;
    int fitness;
};

bool cmp(chromosome &a, chromosome &b) {
    return a.fitness < b.fitness;
}

vector<chromosome> population;

void init_pop() {
    int pop_size = 60;
    population.clear();
    pack.clear();
    int mx = (int) pow(2, n);
    while (pop_size--) {
        int chr_val = (rand() % mx);
        bitset<32> tst(chr_val);
        string genes = "";
        int cnt = n;
        for (int i = 0; i < 32; ++i) {
            if (cnt == 0)
                break;
            genes += to_string(tst[i]);
            cnt--;
        }
        reverse(genes.begin(), genes.end());
        chromosome chr;
        chr.gene = genes;
        population.push_back(chr);
    }
}

void fitness(chromosome &ch) {
    int val = 0, w = 0;
    for (int i = 0; i < n; ++i) {
        if (ch.gene[i] == '1') {
            val += pack[i].second;
            w += pack[i].first;
        }
    }
    if (w > W)
        ch.fitness = 0;
    else
        ch.fitness = val;
}

void calc_fitness() {
    for (int i = 0; i < population.size(); ++i) {
        fitness(population[i]);
    }
}

double randoom_double() {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

    double randomValue = dis(gen);

    return randomValue;
}

int random_int(int x) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, x);
    return dis(gen);
}

pair<int, int> selection() {
    int sum = n * (n + 1) / 2;
    vector<double> prob;
    int rank = 1;
    for (int i = 0; i < population.size(); ++i) {
        prob.push_back(1.0 * rank / sum);
        rank++;
    }
    for (int i = 1; i < prob.size(); ++i) {
        prob[i] += prob[i - 1];
    }
    int sz = population.size();
    pair<int, int> selected;
    selected.first = sz - 1;
    selected.second = sz - 1;
    double num1 = randoom_double();
    for (int i = 0; i < prob.size(); ++i) {
        if (num1 < prob[i]) {
            selected.first = i;
            break;
        }
    }
    double num2 = 1.0 - num1;
    for (int i = 0; i < prob.size(); ++i) {
        if (num2 < prob[i]) {
            selected.second = i;
            break;
        }
    }
    return selected;
}

pair<chromosome, chromosome> crossover(chromosome chr1, chromosome chr2) {
    pair<chromosome, chromosome> child;
    double random = randoom_double();
    child.first = chr1;
    child.second = chr2;

    if (random <= Pc) {
        int crossover_point = random_int(chr1.gene.size());
        //cout << "\nPoint " << crossover_point << endl;

        for (int i = crossover_point; i < chr2.gene.size(); i++) {
            child.first.gene[i] = chr2.gene[i];
            child.second.gene[i] = chr1.gene[i];
        }

        fitness(child.first);
        fitness(child.second);
    }
    return child;


}


void mutatation(chromosome &chr) {

    for (int i = 0; i < chr.gene.size(); i++) {
        if (randoom_double() <= Pm) {
            if (chr.gene[i] == '1') {
                chr.gene[i] = '0';
            } else {
                chr.gene[i] = '1';
            }
        }
    }
    fitness(chr);
}
void Replacement(pair<chromosome, chromosome>&selections){
    population.push_back(selections.first);
    population.push_back(selections.second);
    sort(population.begin(), population.end(), cmp);
    int cnt = population.size();
    cnt/=2;
    while (cnt--)
        population.erase(population.begin());
}
int number_of_selected(string s){
    int cnt = 0;
    for (int i = 0; i < s.size(); ++i) {
        if(s[i] == '1')
            cnt++;
    }
    return cnt;
}
void files(){
    freopen("input.txt","r",stdin);
}
int main() {
    files();
    int t ; cin >> t;
    for (int i = 1; i <= t ; ++i) {
        cin >> W >> n;
        init_pop();
        for (int i = 0; i < n; ++i) {
            int w, v;
            cin >> w >> v;
            pack.emplace_back(w, v);
        }
        int number_of_iterations = 20;
        while (number_of_iterations--){
            calc_fitness();
            sort(population.begin(), population.end(), cmp);

            pair<int, int> ans = selection();
//            cout << ans.first << " " << ans.second << endl;
//            cout << population[ans.first].gene << " " << population[ans.first].fitness << "\n"
//                 << population[ans.second].gene
//                 << " " << population[ans.second].fitness;

            pair<chromosome, chromosome> child = crossover(population[ans.first], population[ans.second]);

//            cout << "New Child\n" << child.first.gene << " " << child.first.fitness << "\n" << child.second.gene << " "
//                 << child.second.fitness << endl;
            mutatation(child.first);
            mutatation(child.second);
//            cout << "New Child after mutation\n" << child.first.gene << " " << child.first.fitness << "\n"
//                 << child.second.gene
//                 << " " << child.second.fitness << endl;
            Replacement(child);
        }
        cout << "Test num : " << i <<"\n";
        sort(population.begin(), population.end(), cmp);
        int sz = population.size();
        cout << "Number of selected items : " << number_of_selected(population[sz-1].gene) <<"\n";
        cout << "Total value : " << population[sz-1].fitness << "\n";
        cout << "Chromosome : " << population[sz-1].gene << "\n";
        cout << "Selected items: \n" ;
        int weight = 0;
        for (int j = 0; j < n; ++j) {
            if(population[sz-1].gene[j] == '1'){
                cout << pack[j].first << " " << pack[j].second << "\n";
                weight+=pack[j].first ;
            }
        }
        cout << "Total Weight : " << weight << "\n\n";
    }

}