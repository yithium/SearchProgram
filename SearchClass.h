#include "Container2D.h"
#include <string>
#include <utility>
#include <vector>

class SearchClass
{
public:
	SearchClass();
	
	bool Load(std::string input);
	bool Load(const std::vector<std::vector<int> >&);
	
	bool AddRow(const std::vector<int>&);
	bool HasData();
	bool SearchSequence(const std::vector<int>& target);
	bool SearchSequence(const std::string& target);
	bool SearchUnordered(const std::vector<int>& target);
	bool SearchUnordered(const std::string& target);
	void SearchClosest(const std::vector<int>& target);
	void SearchClosest(const std::string& target);
	bool ExecuteCommand(const std::string& target);
	
private:
	Container2D _container;
	std::vector<std::vector<std::pair<int, size_t> > >_compressSorted;
	bool HasSequence(const std::vector<int>& source, const std::vector<int>& seq);
	bool HasUnordered(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& values);
	size_t MatchElements(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& values);
	void PrintRow(size_t idx);
};
