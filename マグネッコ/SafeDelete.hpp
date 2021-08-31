template <class T> 
void SafeDelete(T*& p) {
    if (p!=nullptr) {
        delete p;
        p = nullptr;
    }
}