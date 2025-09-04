# 📂 Mini-Git Version Control (C++)

This project implements a **simplified Git-like version control system** in C++.  
It allows you to create commits, manage versions of files, and track changes in a minimalistic way.  

---

## 🔹 Features

- **Commit changes**: Store snapshots of files with commit messages.  
- **Track files**: Stage files before committing.  
- **View commit history**: See previous commits and their details.  
- **Parent linkage**: Each commit knows its parent commit.  
- **Simple, minimal, educational**: Focused on learning how Git works internally.  

---

## 🔹 How It Works

1. **Files are staged** in a staging area before committing.  
2. A **commit** contains:
   - Commit ID (unique)
   - Commit message
   - List of tracked files
   - Pointer to the parent commit  
3. All commits are stored in memory or a simple local repository folder (`.minigit/`).  
4. **Commit history** can be traversed using parent pointers.  

This project mimics the **core logic of Git**, but simplified for educational purposes.

---

## 🔹 Example Usage

```cpp
MiniGit repo;

// Stage files
repo.addFile("file1.txt");
repo.addFile("file2.txt");

// Commit changes
repo.commit("Initial commit");

// Make changes, stage again, commit
repo.addFile("file3.txt");
repo.commit("Added file3");

// View commit history
repo.showHistory();
```

---

## 🔹 Data Structures Used

- **Struct Commit**: Represents each commit, contains:
  - `commitID`
  - `message`
  - `vector<string> files`
  - `Commit* parent`
- **Vector for Staging Area**: Tracks files ready to commit.  
- **Vector of Commits**: Stores all commits sequentially (or linked via parent pointers).  

---

## 🔹 Complexity

- **Commit creation:** O(number of staged files)  
- **Tracking files:** O(1) per file using vectors  
- **History traversal:** O(number of commits)  

---

## 🔹 How to Build and Run

```bash
g++ mini-git.cpp -o mini-git
./mini-git
```

---

## 🔹 Notes

- This is **not a full Git implementation**.  
- No branching or merging is included.  
- Focused on demonstrating **basic version control concepts** in C++.  

---

## 🔹 Future Improvements

- Add **branch support**  
- Implement **merge conflicts**  
- Store commits permanently in `.minigit/` for persistence  
- Add a **command-line interface (CLI)**  

---

✨ A great project to learn **how version control systems work internally**!
