#include <bits/stdc++.h>
using namespace std;

struct Commit {
    string commitID;
    string message;
    vector<string> files;
    Commit* parent;

    Commit(string id, string msg, vector<string> f, Commit* p = nullptr)
        : commitID(id), message(msg), files(f), parent(p) {}
};

class MiniGit {
private:
    Commit* head;
    vector<string> stagingArea;
    string repoPath = ".minigit/";

    string generateCommitID() {
        static int counter = 1;
        return "c" + to_string(counter++);
    }

    void makeDir(const string& path) {
#ifdef _WIN32
        string cmd = "mkdir " + path;
#else
        string cmd = "mkdir -p " + path;
#endif
        system(cmd.c_str());
    }

    void copyFile(const string& src, const string& dest) {
        ifstream in(src, ios::binary);
        ofstream out(dest, ios::binary);
        out << in.rdbuf();
    }

public:
    MiniGit() : head(nullptr) {}

    void init() {
        ifstream test(repoPath.c_str());
        if (test.good()) {
            cout << "Repository already exists.\n";
            return;
        }
        makeDir(repoPath);
        makeDir(repoPath + "commits");
        cout << "Repository initialized.\n";
    }

    void add(const string& filename) {
        ifstream test(filename.c_str());
        if (!test.good()) {
            cout << "File not found: " << filename << "\n";
            return;
        }
        stagingArea.push_back(filename);
        cout << "Staged file: " << filename << "\n";
    }

    void commit(const string& msg) {
        if (stagingArea.empty()) {
            cout << "No files staged.\n";
            return;
        }
        string commitID = generateCommitID();
        string commitDir = repoPath + "commits/" + commitID + "/";
        makeDir(commitDir);

        for (auto& file : stagingArea) {
            string filename = file.substr(file.find_last_of("/\\") + 1);
            string dest = commitDir + filename;
            copyFile(file, dest);
        }

        head = new Commit(commitID, msg, stagingArea, head);

        cout << "Commit created: " << commitID << "\n";
        stagingArea.clear();
    }

    void log() {
        Commit* temp = head;
        while (temp) {
            cout << "CommitID: " << temp->commitID
                 << "  Message: " << temp->message << "\n";
            temp = temp->parent;
        }
    }

    void checkout(const string& commitID) {
        string commitDir = repoPath + "commits/" + commitID + "/";
        ifstream test(commitDir.c_str());
        if (!test.good()) {
            cout << "Commit not found: " << commitID << "\n";
            return;
        }

        // On Linux/Mac: "cp commitDir/* ."
        // On Windows: use "copy"
#ifdef _WIN32
        string cmd = "copy " + commitDir + "* .";
#else
        string cmd = "cp " + commitDir + "* .";
#endif
        system(cmd.c_str());

        cout << "Checked out to commit " << commitID << "\n";
    }
};

int main() {
    MiniGit git;
    string cmd;

    cout << "MiniGit (C++ portable version)\n";
    cout << "Commands: init, add <file>, commit -m <msg>, log, checkout <id>, exit\n";

    while (true) {
        cout << "> ";
        getline(cin, cmd);
        stringstream ss(cmd);
        string token;
        ss >> token;

        if (token == "init") {
            git.init();
        } else if (token == "add") {
            string fname; ss >> fname;
            git.add(fname);
        } else if (token == "commit") {
            string dashM, msg;
            ss >> dashM;
            getline(ss, msg);
            if (!msg.empty() && msg[0] == ' ') msg = msg.substr(1);
            git.commit(msg);
        } else if (token == "log") {
            git.log();
        } else if (token == "checkout") {
            string id; ss >> id;
            git.checkout(id);
        } else if (token == "exit") {
            break;
        } else {
            cout << "Unknown command.\n";
        }
    }
    return 0;
}
