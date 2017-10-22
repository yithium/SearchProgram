#include "Container2D.h"  // for 2d container
#include <string>         // for usage interface, size_t
#include <utility>        // for std::pair
#include <vector>         // for base container type
#include <map>
enum RESULT
{
	FILE_NOT_FOUND,
	INVALID_FILE_DATA,
	LOAD_OK,
	NO_DATA,
	INPUT_LENGTH_EXCEED,
	NOT_FOUND,
	NO_MATCHES,
	NO_INPUT,
	FOUND
};


// class with searchSequence, searchUnordered, searchClosest functionality base on loaded data
// *** to be rewritten to handle any data type
template <typename DataType>
class SearchClass
{
public:
	// for result on operation of SearchClass
	typedef std::pair<DataType, size_t> DataCounts;
	typedef std::vector<DataCounts> VecDataCounts;
	typedef std::pair<size_t, std::vector<size_t> > PairRowColIdxs;
	typedef std::map<DataType, std::vector<PairRowColIdxs> > MapKeyToRowVec;
	
	
	SearchClass();
	
	
	// load 2D data, returns false on failure of loading
	RESULT Load(std::string input, std::string lineSplit = "\n");
	RESULT Load(const std::vector<std::vector<DataType> >&);
	
	
	// verify if data available for search
	bool HasData();
	
	
	// search functions base on string or vectors.
	RESULT SearchSequence(const std::vector<DataType>& target);
	RESULT SearchSequence(const std::string& target);
	RESULT SearchUnordered(const std::vector<DataType>& target);
	RESULT SearchUnordered(const std::string& target);
	RESULT SearchClosest(const std::vector<DataType>& target);
	RESULT SearchClosest(const std::string& target);
	
	
	// For executing commands received
	RESULT ExecuteCommand(const std::string& target);
	
	

	
	
private:
	// 2D data container
	Container2D<DataType> _container;
	// For reducing search time on searchUnordered and searchClosest at the expense of memory
	std::vector<VecDataCounts>_compressSorted;
	// For indexing Data and reducing rows to search, and potentially index to search
	MapKeyToRowVec _dataIdxMap;
	
	// compare if source vector contain target vector's sequence
	bool HasSequence(const std::vector<DataType>& source, const std::vector<DataType>& target);
	// compare if source vector contains all elements of target vector
	bool HasUnordered(const VecDataCounts& source, const VecDataCounts& target);
	// return number of matching elements in source vector from target vector
	size_t MatchElements(const VecDataCounts& source, const VecDataCounts& values);
	// to print row information.
	// *** to be made modified to ostream&
	void PrintRow(size_t idx);
};

#include "SearchClass.inl"
