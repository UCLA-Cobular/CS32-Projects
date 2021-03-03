void listAll(const File *f, string path)  // two-parameter overload
{
    string curPath = path + "/" + f->name();
    cout << curPath << endl;
    if (f->files() == nullptr) return;
    for (auto i : *f->files()) {
        listAll(i, curPath);
    }
}
