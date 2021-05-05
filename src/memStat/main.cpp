# include <iostream>
# include <vector>

# include "Mem.hpp"
# include "processMem.hpp"

using namespace std;

int main() {
    MemInfo::getCurMemInfo();
    cout << MemInfo::m_total << endl;
    cout << MemInfo::m_free << endl;
    cout << MemInfo::m_cached << endl;
    cout << MemInfo::m_buffers << endl;
    cout << MemInfo::m_used << endl;
}
