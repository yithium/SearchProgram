/*
  SearchClass contain three main utilities which operates on a 2D matrix of data.
  The three utilities are Search Sequence, Search Unordered and Search Closest.
  
  SearchSequence will search for a specific subset in each row of the 2D matrix,
  if found, it will then print the row that it found.
  
  SearchUnordered will search for a subset of Data elements ignoring their order, 
  it will print the row Data on found.
  
  SearchClosest will look for the row with the highest elements matches ignoring
  their order, if more than one row exist with the same number of matching elements, 
  the row with the smallest index of those will be printed.
  
  On load, it will create an indexing with Data Value as the key, storing all
  the position that they appeared in. Also, for each row, it also creates a 
  vector of sorted elements with their respective count of appearancein that row.
  
*/

#include "Container2D.h"  // for 2d container
#include <string>         // for usage interface, size_t
#include <utility>        // for std::pair
#include <vector>         // for base container type
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <map>
#include <set>
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



template <typename DataType>
class SearchClass
{
public:
  // for result on operation of SearchClass
  typedef std::pair<DataType, size_t>                      DataCounts;
  typedef std::vector<DataCounts>                          VecDataCounts;
  typedef std::pair<size_t, std::vector<size_t> >          PairRowColIdxs;
  typedef std::map<DataType, std::vector<PairRowColIdxs> > MapKeyToRowVec;
  typedef std::tr1::unordered_set<size_t>                  RowSet;
  typedef std::map<DataType, RowSet >                      MapKeyRowSet;
  
  SearchClass();
  
  // load 2D data, returns false on failure of loading
  RESULT Load(std::string input, std::string lineSplit = "\n");
  RESULT Load(const std::vector<std::vector<DataType> >&);
  
  // verify if data available for search
  bool HasData();
  
  // For executing commands received
  RESULT ExecuteCommand(const std::string& target);
  
  

  
  
private:
  // 2D data container
  Container2D<DataType> _container;
  // For reducing search time on searchUnordered and searchClosest at the expense of memory
  std::vector<VecDataCounts>_compressSorted;
  // For indexing Data and reducing rows to search, and potentially index to search
  MapKeyToRowVec _dataIdxMap;
  MapKeyRowSet _dataRowMap;
  
  // search functions base on string or vectors.
  RESULT SearchSequence(const std::vector<DataType>& target);
  RESULT SearchSequence(const std::string& target);
  RESULT SearchUnordered(const std::vector<DataType>& target);
  RESULT SearchUnordered(const std::string& target);
  RESULT SearchClosest(const std::vector<DataType>& target);
  RESULT SearchClosest(const std::string& target);
  
  
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
