#include <vector>
#include <string>
std::vector<std::string> LoadHistory(void);
std::vector<std::string> LoadHistory(const char *filename);

void SaveHistory(std::vector<std::string> History);
void SaveHistory(const char* filename,std::vector<std::string> History);
