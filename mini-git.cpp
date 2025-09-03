#include <bits/stdc++.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    string logFile = ".minigit/log.txt";

    // Check if a directory exists
    bool dirExists(const string& path) {
        struct stat info;
        return (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
    }

    // Create directory cross-platform
    void makeDir(const string& path) {
#ifdef _WIN32
        string cmd = "mkdir \"" + path + "\" >nul 2>&1";
#else
        string cmd = "mkdir -p \"" + path + "\"";
#endif
        system(cmd.c_str());
    }

    // Copy file from src → dest
    void copyFile(const string& src, const string& dest) {
        ifstream in(src, ios::binary);
        ofstream out(dest, ios::binary);
        out << in.rdbuf();
    }

    // Generate commit ID (incremental)
    string generateCommitID() {
        static int counter = 1;
        return "c" + to_string(counter++);
    }

    // Load commits from log file
    void loadCommits() {
        ifstream fin(logFile);
        if (!fin.good()) return;

        map<string, Commit*> commitMap;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, msg, filesStr, parentID;
            getline(ss, id, '|');
            getline(ss, msg, '|');
            getline(ss, filesStr, '|');
            getline(ss, parentID, '|');

            vector<string> files;
            stringstream fs(filesStr);
            string f;
            while (getline(fs, f, ',')) if (!f.empty()) files.push_back(f);

            Commit* parent = parentID.empty() ? nullptr : commitMap[parentID];
            Commit* c = new Commit(id, msg, files, parent);
            commitMap[id] = c;
            head = c; // latest commit
        }
        fin.close();
    }

    // Append a commit to log
    void saveCommitToLog(Commit* c) {
        ofstream fout(logFile, ios::app);
        fout << c->commitID << "|" << c->message << "|";
        for (size_t i = 0; i < c->files.size(); i++) {
            fout << c->files[i];
            if (i != c->files.size() - 1) fout << ",";
        }
        fout << "|";
        fout << (c->parent ? c->parent->commitID : "") << "|\n";
        fout.close();
    }

public:
    MiniGit() : head(nullptr) {
        if (!dirExists(repoPath)) makeDir(repoPath);
        if (!dirExists(repoPath + "commits")) makeDir(repoPath + "commits");
        loadCommits(); // Load commits at startup
    }

    void init() {
        ifstream test(logFile);
        if (test.good()) {
            cout << "Repository already exists.\n";
            return;
        }
        makeDir(repoPath);
        makeDir(repoPath + "commits");
        ofstream(logFile); // create empty log
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
            copyFile(file, commitDir + filename);
        }

        Commit* c = new Commit(commitID, msg, stagingArea, head);
        head = c;
        saveCommitToLog(c);

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
        // Search for commit in memory
        Commit* temp = head;
        while (temp) {
            if (temp->commitID == commitID) break;
            temp = temp->parent;
        }
        if (!temp) {
            cout << "Commit not found: " << commitID << "\n";
            return;
        }

        string commitDir = repoPath + "commits/" + commitID + "/";
        for (auto& file : temp->files) {
            string filename = file.substr(file.find_last_of("/\\") + 1);
            copyFile(commitDir + filename, filename); // restore file
        }

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
