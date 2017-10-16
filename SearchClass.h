#include "Container2D.h"  // for 2d container
#include <string>         // for usage interface, size_t
#include <utility>        // for std::pair
#include <vector>         // for base container type

// class with searchSequence, searchUnordered, searchClosest functionality base on loaded data
// *** to be rewritten to handle any data type
class SearchClass
{
public:
	SearchClass();
	
	
	// load 2D data, returns false on failure of loading
	bool Load(std::string input);
	bool Load(const std::vector<std::vector<int> >&);
	
	
	// Add a row at the end
	bool AddRow(const std::vector<int>&);
	// verify if data available for search
	bool HasData();
	
	
	// search functions base on string or vectors.
	bool SearchSequence(const std::vector<int>& target);
	bool SearchSequence(const std::string& target);
	bool SearchUnordered(const std::vector<int>& target);
	bool SearchUnordered(const std::string& target);
	void SearchClosest(const std::vector<int>& target);
	void SearchClosest(const std::string& target);
	
	
	// For executing commands received
	bool ExecuteCommand(const std::string& target);
	
private:
	// 2D data container
	Container2D _container;
	// For reducing search time on searchUnordered and searchClosest at the expense of memory
	std::vector<std::vector<std::pair<int, size_t> > >_compressSorted;
	
	
	// compare if source vector contain target vector's sequence
	bool HasSequence(const std::vector<int>& source, const std::vector<int>& target);
	// compare if source vector contains all elements of target vector
	bool HasUnordered(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& target);
	// return number of matching elements in source vector from target vector
	size_t MatchElements(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& values);
	// to print row information.
	// *** to be made modified to ostream&
	void PrintRow(size_t idx);
};
