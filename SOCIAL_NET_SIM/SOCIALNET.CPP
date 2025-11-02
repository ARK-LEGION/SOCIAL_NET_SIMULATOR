#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <cctype>
#include <utility>
using namespace std;

class post{
    string content;
    time_t time_of_creation;
    public:
    post(string content) {
        this->content = content;
        time_of_creation = time(0);
    }
    post(){};
    string getContent() {
        return content;
    }
    time_t getTimeOfCreation() {
        return time_of_creation;
    }
};

class node{
    public:
    post data;
    node* left;
    node* right;
    int height;
    node(post p){
        data=p;
        height=0;
        left = right = nullptr;
    }
};

class AVL_TREE{
    node* root;

    int height(node*N){
        if (N==nullptr) return -1;
        else return N->height;
    }

    int getBalance(node*N){
        if (N==nullptr) return 0;
        return height(N->left)-height(N->right);
    }

    node* leftRotate(node* x){
        node* y = x->right;
        node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    node* rightRotate(node* y){
        node* x = y->left;
        node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }
    node* insertHelper(node* N, post p){
        if (N==nullptr){
            return new node(p);
        }
        if (p.getTimeOfCreation() < N->data.getTimeOfCreation()){
            N->left = insertHelper(N->left, p);
        }
        else {
            N->right = insertHelper(N->right, p);
        }


        N->height = 1 + max(height(N->left), height(N->right));
        int balance = getBalance(N);
        time_t key = p.getTimeOfCreation();
        // Left Left Case
        if (balance > 1 && key < N->left->data.getTimeOfCreation()){
            return rightRotate(N);
        }
        // Right Right Case
        if (balance < -1 && key >= N->right->data.getTimeOfCreation()){
            return leftRotate(N);
        }
        // Left Right Case
        if (balance > 1 && key >= N->left->data.getTimeOfCreation()){
            N->left = leftRotate(N->left);
            return rightRotate(N);
        }
        // Right Left Case
        if (balance < -1 && key < N->right->data.getTimeOfCreation()){
            N->right = rightRotate(N->right);
            return leftRotate(N);
        }

        return N;
    }
    public:
    AVL_TREE(){
        root = nullptr;
    }
    node* getRoot(){
        return root;
    }
    void INSERT(post p){
        root=insertHelper(root,p);
    }

};

class user{
    string username;
    int userID;
    AVL_TREE POSTS;
    void OUTPUT_POSTS_HELPER(node* N, vector<string>& inot){
        if (N==nullptr) return;
        stack<node*> st;
        st.push(N);
        while (!st.empty()){
            node* cur = st.top();
            if (cur!=nullptr){
                st.push(cur->left);
            }
            else{
                st.pop();
                if (st.empty()) break;
                cur = st.top();
                inot.push_back(cur->data.getContent());
                st.pop();
                st.push(cur->right);
            }
        }
        return;
    }
    public:
    user(string name, int id){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        username = name;
        userID = id;
        POSTS = AVL_TREE();
    }
    user(){};
    void ADD_POST(string content){
        transform(content.begin(), content.end(), content.begin(), ::tolower);
        post p(content);
        POSTS.INSERT(p);
    }
    void OUTPUT_POSTS(int n){
        //reverse chronological order
        //get the inorder traversal of the tree
        vector<string> inot;
        OUTPUT_POSTS_HELPER(POSTS.getRoot(), inot);
        int x=inot.size();
        if (x==0) return;
        if (n==-1){
            //output all posts
            for (int i=x-1; i>=0; i--){
                cout<<inot[i]<<"\n";
            }
            cout<<endl;
        }
        else{
            n=min(n, (int)inot.size());
            for (int i = 0; i < n; ++i) {
                cout << inot[x - 1 - i] << "\n";
            }
            cout<<endl;
        }
    }
    string getUsername(){
        return username;
    }
    int getUserID(){
        return userID;
    }
};

//graph structure to hold all users
class SocialNetwork{

    map<string, user*> usernames;
    map<int, user*> userIndex;//assign a unique integer index to each user for graph representation
    vector<vector<int>> adj; //adjacency list representation //friendships

    public:

    void ADD_USER(string name){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (usernames.find(name) != usernames.end()){
            cout<<"User already exists"<<endl;    //ignore if user with that name already exists
            return;
        }
        user* newUser = new user(name, usernames.size());
        usernames[name] = newUser;
        userIndex[usernames.size()-1]=newUser;
        adj.push_back(vector<int>());
    }

    void ADD_POST_TO_USER(string name, string content){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        transform(content.begin(), content.end(), content.begin(), ::tolower);
        if (usernames.find(name) == usernames.end()){
            cout<<"User does not exist\n";
            return;
        }
        user* usr = usernames[name];
        usr->ADD_POST(content);
    }

    void OUTPUT_USER_POSTS(string name, int n){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (usernames.find(name) == usernames.end()){
            cout<<"User does not exist"<<endl;
            return;
        }
        user* usr = usernames[name];
        usr->OUTPUT_POSTS(n);
    }

    void ADD_FRIEND(string name1, string name2){
        transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
        if (usernames.find(name1) == usernames.end() || usernames.find(name2) == usernames.end()){
            cout<<"One or both users do not exist"<<endl;
            return;
        }
        if (name1==name2){
            cout<<"Cannot friend oneself\n";
            return;
        }
        for (auto u: adj[usernames[name1]->getUserID()]){
            if (u == usernames[name2]->getUserID()){
                cout<<"Already friends\n";
                return; 
            }
        }
        user* user1 = usernames[name1];
        user* user2 = usernames[name2];
        int idx1 = user1->getUserID();
        int idx2 = user2->getUserID();
        adj[idx1].push_back(idx2);
        adj[idx2].push_back(idx1);
    }

    void LIST_FRIENDS(string name){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        vector<string> friends;
        if (usernames.find(name) == usernames.end()){
            cout<<"User does not exist\n";
            return;
        }
        user* usr = usernames[name];
        int idx = usr->getUserID();
        for (int i=0; i<adj[idx].size(); i++){
            int fi = adj[idx][i];
            friends.push_back(userIndex[fi]->getUsername());
        }
        sort(friends.begin(), friends.end());
        for (auto f: friends){
            cout<<f<<" ";
        }
        cout<<endl;
    }

    void SUGGEST_FRIENDS(string name, int n){
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (n==0) return;
        if (usernames.find(name) == usernames.end()){
            cout<<"Cannot Suggest ! , User does not exist\n";
            return;
        }
        user* usr = usernames[name];
        //LOGIC: BFS type approach to find friends of friends
        int idx = usr->getUserID();
        //all nodes at distance 3 or mode have 0 mutual friends
        queue<pair<vector<int>,string>> q;
        q.push({{0,idx},name});
        vector<string> fof; //friend of friends
        vector<bool> vis(usernames.size(), false);
        map<int, int> mutualFriendCount; //username to mutual friend count
        vis[idx]=true;
        
        while (!q.empty()){
            auto cur = q.front();
            int dist = cur.first[0];
            int id = cur.first[1];
            vis[id]=1;
            string curName = cur.second;
            q.pop();
            
            if (dist==0){
                for (auto u: adj[id]){
                    if (!vis[u]){
                        vis[u]=1;
                        q.push({{dist+1,u}, userIndex[u]->getUsername()});
                    }
                }
            }
            else if (dist==1){
                for (auto u: adj[id]){
                    if (u!=idx){
                        if (!vis[u]){
                            fof.push_back(userIndex[u]->getUsername());
                            vis[u]=true;
                        }
                        mutualFriendCount[u]++;
                    }
                }
            }
            else break; // only need friends of friends, so stop at distance 2

        }
        for (auto u: adj[idx]){
            mutualFriendCount.erase(u); //remove direct friends from suggestion list
        }
        n=min(n, (int)fof.size());
        //sort fof based on mutual friend count and then alphabetically
        sort(fof.begin(), fof.end(), [&](string& a, string& b){
            int cnt1 = mutualFriendCount[usernames[a]->getUserID()];
            int cnt2 = mutualFriendCount[usernames[b]->getUserID()];
            if (cnt1 == cnt2){
                return a < b;
            }
            return cnt1 > cnt2;
        });

        for (int i=0; i<n; i++){
            cout<<fof[i]<<" ";
        }
        cout<<endl;
    }

    int DEGREES_OF_SEPARATION(string name1, string name2){
        transform(name1.begin(), name1.end(), name1.begin(), ::tolower);
        transform(name2.begin(), name2.end(), name2.begin(), ::tolower);
        if (usernames.find(name1) == usernames.end() || usernames.find(name2) == usernames.end()){
            return -1;
        }
        if (name1==name2){
            return 0;
        }
        user* user1 = usernames[name1];
        user* user2 = usernames[name2];
        int idx1 = user1->getUserID();
        int idx2 = user2->getUserID();

        //bfs traversal to find shortest path
        queue<pair<int,int>> q; 
        q.push({idx1,0});
        vector<bool> vis(usernames.size(), false);
        vis[idx1]=true;
        while (!q.empty()){
            auto cur = q.front();
            int id= cur.first;
            int dist = cur.second;
            q.pop();
            if (id==idx2){
                return dist;
            }
            for (auto u:adj[id]){
                if (!vis[u]){
                    vis[u]=1;
                    q.push({u, dist+1});
                }
            }
        }
        return -1; //not connected
    }
};

//main function to handle input and output
int main(){
    SocialNetwork sn;
    string line;
    while (getline(cin, line)){
        if (line.empty()) continue;
        istringstream iss(line);
        string cmd;
        if (!(iss>>cmd)) continue;

        // Commands are CASE-SENSITIVE per your note.
        if (cmd=="ADD_USER"){
            string name;
            if (iss>>name) {
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                sn.ADD_USER(name);
            }
            else cout<<"Invalid ADD_USER command\n";
        }
        else if (cmd=="ADD_FRIEND"){
            string a,b;
            if (iss>>a>>b) {
                transform(a.begin(), a.end(), a.begin(), ::tolower);
                transform(b.begin(), b.end(), b.begin(), ::tolower);
                sn.ADD_FRIEND(a,b);
            }
            else cout<<"Invalid ADD_FRIEND command\n";
        }
        else if (cmd=="LIST_FRIENDS"){
            string name;
            if (iss>>name) {
                transform(name.begin(), name.end(), name.begin(), ::tolower);
                sn.LIST_FRIENDS(name);
            }
            else cout<<"Invalid LIST_FRIENDS command\n";
        }
        else if (cmd=="SUGGEST_FRIENDS"){
            string name; int n;
            if (!(iss>>name)){ cout<<"Invalid SUGGEST_FRIENDS command\n"; continue; }
            if (!(iss>>n)) n = 0;
            sn.SUGGEST_FRIENDS(name,n);
        }
        else if (cmd=="DEGREES_OF_SEPARATION"){
            string a,b;
            if (iss>>a>>b){
                int d = sn.DEGREES_OF_SEPARATION(a,b);
                // always print the result (prints -1 if not connected or if function signals error)
                cout<<d<<endl;
            } else cout<<"Invalid DEGREES_OF_SEPARATION command"<<endl;

        }
        else if (cmd=="ADD_POST"){
            string name;
            if (!(iss>>name)){ cout<<"Invalid ADD_POST command\n"; continue; }
            // remainder of the line after the username is the content (preserve case until user::ADD_POST lowercases it)
            size_t pos = line.find(name);
            string content = "";
            if (pos!=string::npos){
                size_t start = pos + name.size();
                if (start < line.size()){
                    content = line.substr(start);
                    // trim leading spaces
                    size_t first = content.find_first_not_of(' ');
                    if (first!=string::npos) content = content.substr(first);
                    else content = "";
                }
            }
            transform(content.begin(), content.end(), content.begin(), ::tolower);
            sn.ADD_POST_TO_USER(name, content);
        }
        else if (cmd=="OUTPUT_POSTS"){
            string name;
            if (!(iss>>name)){ cout<<"Invalid OUTPUT_POSTS command"<<endl; continue; }
            int n;
            if (!(iss>>n)) n = -1;
            sn.OUTPUT_USER_POSTS(name, n);
        }
        else {
            cout<<"Unknown command"<<endl;
        }
    }

    return 0;

}
