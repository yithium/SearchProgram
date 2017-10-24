

//#include "SearchClass.h"   // SearchClass declaration
#include "Cryptography.h"  // Encryption/Decryption operation
#include <iostream>        // std::cout for printing operation results
#include <fstream>         // for reading data file
#include <algorithm>       // sort
#include <vector>          // container
#include <sstream>         // getline from string streams


#define KEY "unlock"       // encryption/decryption key
#define BUFFERSIZE 1000    // assumed buffer size
#define ASSUMESIZE 10      // defaulted vector reserve size, to reduce number of allocation due to no reserve

#ifdef LESSPRINT
  #define PRINT(actions)
#else
  #define PRINT(actions) actions
#endif


#ifdef MEASURESPEED
  #include <ctime> // for benchmarking
  #include <iomanip>
  std::clock_t start;
  double duration = 0;

  #define START_TIMER            start = std::clock(); \
                                 duration = 0;

  #define START_LAP              start = std::clock();
  #define END_LAP                duration += ( std::clock() - start );

  #define PRINTDURATION(message) std::cout << message << ": " << std::setprecision(12) << duration/ (double) CLOCKS_PER_SEC <<'\n';
#else
  #define START_TIMER
  #define START_LAP
  #define END_LAP
  #define PRINTDURATION(message)
#endif


/***************************/
/*** Help functions Begin***/
/***************************/

inline void HandleResult(RESULT result, const std::string& target)
{

  switch (result)
  {
  case FILE_NOT_FOUND:
    std::cout << "File not found.\n";
    break;
  case INVALID_FILE_DATA:
    std::cout << "Please ensure data is valid.\n";
    break;
  case LOAD_OK:
    std::cout << "Successfully loaded.\n";
    break;
  case NO_DATA:
    std::cout << "Failed as no data is loaded.\n";
    break;
  case INPUT_LENGTH_EXCEED:
    std::cout << "No result found for '" << target << "' as number of elements greater than data.\n";
    break;
  case NOT_FOUND:
    std::cout << "No result found for '" << target << "'.\n";
    break;
  case NO_MATCHES:
    std::cout << "\nWas returned as no matches in all rows.\n";
    break;
  case NO_INPUT:
    std::cout << "No input was listed skipping\n";
  case FOUND:
    std::cout << "End\n";
    break;

  }

  // leave a line after each user request for easier viewing
  std::cout << "\n";
}

// convert input reference to lower case
inline std::string& toLowerCase(std::string& input)
{
  for (size_t idx = 0; idx < input.size(); ++idx)
  {
    input[idx] = std::tolower(input[idx]);
  }
  return input;
}

// splitting a string by newlines or delimiter string
// for splitting input data into their respective rows
inline std::vector<std::string> splitString(const std::string& input, std::string delimiter = "\n")
{
  std::vector<std::string> result;
  result.reserve(ASSUMESIZE);

  size_t sPos = 0;
  size_t ePos = 0;

  // split until end of string
  while (ePos != std::string::npos)
  {
    // search for end point of first string
    ePos = input.find(delimiter, sPos);
    // push string to be split into result vector
    result.push_back(
      ePos == std::string::npos          //
      ? input.substr(sPos)             // push back remaining string if reached end of string
      : input.substr(sPos, ePos - sPos) // push back sub string found
      );

    sPos = ePos + delimiter.length();    // update start position to after delimiter found
  }

  // remove empty string created by potential different input string format
  if (result.back().size() == 0)
    result.pop_back();

  // return split result
  return result;
}

// split string into int
template <typename DataType>
inline std::vector<DataType> splitStringData(const std::string& input)
{
  std::vector<DataType> result;
  result.reserve(ASSUMESIZE);

  // create string stream to utilize stl
  std::istringstream isstream(input);

  DataType val = 0;
  // get first value from string
  if (isstream.good())
  {
    // get all valid data from string
    while (isstream >> val)
    {
      // push previously obtained valid data into vector
      result.push_back(val);
    }
  }

  return result;
}

// returns sorted element value and counts from unsorted data vector
template <typename DataType>
std::vector<std::pair<DataType, size_t> > countElem(std::vector<DataType> input)
{
  std::vector<std::pair<DataType, size_t> > result;
  result.reserve(input.size());

  std::sort(input.begin(),
    input.end()
    );

  // combine sorted data
  DataType curElem = input[0];
  size_t count = 0;
  // count number of each elements and push into result when element is different from previous
  for (size_t elem = 0; elem < input.size(); ++elem)
  {
    // increase count if same element

    if (curElem == input.at(elem))
    {
      ++count;
    }
    // push if different element and initialize value for next element
    else if (curElem != input.at(elem))
    {
      result.push_back(std::pair<DataType, size_t>(curElem, count));
      curElem = input[elem];
      count = 1;
    }
  }

  // push the last element counts that has not been pushed
  result.push_back(std::pair<DataType, size_t>(curElem, count));
  return result;
}


/***************************/
/**** Help functions End****/
/***************************/


/*********************************************************************************************/
/*********************************************************************************************/
/******************************* Search class Public functions Starts*************************/
/*********************************************************************************************/
/*********************************************************************************************/

// constructor
template <typename DataType>
SearchClass<DataType>::SearchClass()
{
}

// load from data from file
template <typename DataType>
RESULT SearchClass<DataType>::Load(std::string input, std::string lineSplit)
{
  // opening file
  std::ifstream fileStream(input.c_str(), std::ios::binary);
  Cryptography crypto(KEY);
  RESULT result = FILE_NOT_FOUND;
  // read data if file opened successfully
  if (fileStream.good())
  {
    // get data size
    fileStream.seekg(0, fileStream.end);
    int length = fileStream.tellg();
    fileStream.seekg(0, fileStream.beg);

    std::string fileData;
    // read data into string using buffer of BUFFERSIZE 
    while (length > 0 && fileStream.good())
    {
      char buffer[BUFFERSIZE] = { 0 };
      int readSize = length > BUFFERSIZE ? BUFFERSIZE : length;

      fileStream.read(buffer, readSize);
      fileData.append(buffer, readSize);

      length -= readSize;
    }
    fileStream.close();

    // decrypt data
    fileData = crypto.Decrypt(fileData);
    // split into data into rows
    std::vector<std::string> vecStr = splitString(fileData, lineSplit);
    // split data rows into elements
    std::vector<std::vector<DataType> > data2D;
    data2D.reserve(ASSUMESIZE);
    for (size_t idx = 0; idx < vecStr.size(); ++idx)
    {
      data2D.push_back(splitStringData<DataType>(vecStr[idx]));
    }
    // load container using 2d elements data
    result = Load(data2D);
  }
  return result;
}

// load container 2d using 2d vector
template <typename DataType>
RESULT SearchClass<DataType>::Load(const std::vector<std::vector<DataType> >& input)
{
  // verify number of elements in each row is the same
  RESULT result = INVALID_FILE_DATA;
  bool conRes = _container.Load(input);
  // create data for searchUnordered and searchClosest optimization
  if (conRes)
  {
    _compressSorted.reserve(_container.GetRowNum());
    // 
    for (size_t row_idx = 0; row_idx < _container.GetRowNum(); ++row_idx)
    {
      // storing elements and amount that exist
      _compressSorted.push_back(
        countElem(
        _container.GetRow(row_idx)
        )
        );

      // storing index of elements that exist
      const std::vector<DataType>& rowRef = _container.GetRow(row_idx);
      for (size_t col_idx = 0; col_idx < rowRef.size(); ++col_idx)
      {
        // index
        typename MapKeyToRowVec::iterator elemFound = _dataIdxMap.find(rowRef[col_idx]);
        if (elemFound != _dataIdxMap.end())
        {
          // key found in map
          if (elemFound->second.back().first != row_idx)
          {
            // if last row is not current row. Create row vector pair for current row
            elemFound->second.push_back(PairRowColIdxs(row_idx, std::vector<size_t>(1, col_idx)));
          }
          else
          {
            elemFound->second.back().second.push_back(col_idx);
          }
        }
        else
        {
          // key not found in map
          //std::vector<PairRowColIdxs> temp;
          //temp.push_back(PairRowColIdxs(row_idx, std::vector<size_t> (1,col_idx)));
          _dataIdxMap[rowRef[col_idx]] = std::vector<PairRowColIdxs>(1, PairRowColIdxs(row_idx, std::vector<size_t>(1, col_idx)));
        }
        
        // row set
        typename MapKeyRowSet::iterator rElemFound = _dataRowMap.find(rowRef[col_idx]);
        if (rElemFound != _dataRowMap.end())
        {
          rElemFound->second.insert(row_idx);
        }
        else
        {
          _dataRowMap[rowRef[col_idx]] = RowSet(row_idx);
        }
      }
    }
    result = LOAD_OK;
  }

  return result;
}





// Verify SearchClass is read for 'Command Execution'
template <typename DataType>
bool SearchClass<DataType>::HasData()
{
  return !_container.IsEmpty();
}

// return true if found at least one row that match sequence
template <typename DataType>
RESULT SearchClass<DataType>::SearchSequence(const std::vector<DataType>& target)
{
  START_LAP
  RESULT result = NOT_FOUND;
  // ensure user did not try to call on invalid data
  // ends early if number of element exceed size of row
  if (HasData())
  {
    if (target.empty())
    {
      result = NO_INPUT;
    }
    else if (_container.GetColNum() >= target.size())
    {
      typename MapKeyToRowVec::const_iterator iter = _dataIdxMap.find(target[0]);
      if (iter != _dataIdxMap.end())
      {
        for (size_t idx = 0; idx < iter->second.size(); ++idx)
        {
          // get row references and indexes in current row
          const std::vector<size_t>& rowIndexsRef = iter->second[idx].second;
          const std::vector<DataType>& rowRef = _container.GetRow(iter->second[idx].first);
          
          for(size_t rIIdx = 0; rIIdx < rowIndexsRef.size(); ++ rIIdx)
          {
            if(rowRef.size() - rowIndexsRef[rIIdx] >= target.size())
            {
              size_t begOff = rowIndexsRef[rIIdx];
              size_t endOff = begOff + target.size();
              std::vector<DataType> subRow(rowRef.begin() + begOff, rowRef.begin() + endOff);
              if (HasSequence(subRow, target))
              {
                // print row and indicate result found
                PRINT(PrintRow(iter->second[idx].first);)
                result = FOUND;
                break;
              }
            }
          }
          
          //if (HasSequence(_container.GetRow(iter->second[idx].first), target))
          //{
          //  // print row and indicate result found
          //  PrintRow(iter->second[idx].first);
          //  result = FOUND;
          //}
        }
      }
    }
    else
    {
      result = INPUT_LENGTH_EXCEED;
    }
  }
  else
  {
    result = NO_DATA;
  }
  END_LAP
  return result;
}

// string input to call on vector input
template <typename DataType>
RESULT SearchClass<DataType>::SearchSequence(const std::string& target)
{
  return SearchSequence(splitStringData<DataType>(target));
}

// return true if found at least one row that contain all elements unordered
template <typename DataType>
RESULT SearchClass<DataType>::SearchUnordered(const std::vector<DataType>& target)
{
  START_LAP
  RESULT result = NOT_FOUND;
  // ensure user did not try to call on invalid data
  // ends early if number of element exceed size of row
  if (HasData())
  {
    if (target.empty())
    {
      result = NO_INPUT;
    }
    else if (_container.GetColNum() >= target.size())
    {
      // convert user's input for easier manipulation

      VecDataCounts targetCompressed = countElem<DataType>(target);

      typename MapKeyToRowVec::const_iterator iter = _dataIdxMap.find(targetCompressed[0].first);
      if (iter != _dataIdxMap.end())
      {
        for (size_t idx = 0; idx < iter->second.size(); ++idx)
        {
          if (HasUnordered(_compressSorted[iter->second[idx].first], targetCompressed))
          {
            // print row and indicate result found
            PRINT(PrintRow(iter->second[idx].first);)
            result = FOUND;
          }
        }
      }
    }
    else
    {
      result = INPUT_LENGTH_EXCEED;
    }
  }
  else
  {
    result = NO_DATA;
  }
  END_LAP
  return result;
}

// string input to call on vector input
template <typename DataType>
RESULT SearchClass<DataType>::SearchUnordered(const std::string& target)
{
  return SearchUnordered(splitStringData<DataType>(target));
}

// print closest result to user input
template <typename DataType>
RESULT SearchClass<DataType>::SearchClosest(const std::vector<DataType>& target)
{
  START_LAP
  RESULT result = NO_MATCHES;
  if (HasData())
  {
    if (target.empty())
    {
      result = NO_INPUT;
    }
    else
    {
      size_t maxElements = target.size();
      size_t elements = 0;
      size_t rowIdx = 0;
      VecDataCounts targetCompressed = countElem<DataType>(target);
      std::tr1::unordered_map<size_t,size_t> rowElemMap;
      for (size_t tIdx = 0; tIdx < targetCompressed.size() && maxElements > elements; ++tIdx)
      {
        DataCounts& targValRef = targetCompressed[tIdx];
        typename MapKeyToRowVec::const_iterator iter = _dataIdxMap.find(targValRef.first);
        if(iter != _dataIdxMap.end())
        {
          const std::vector<PairRowColIdxs>& valIdxs = iter->second;
          for(size_t vcIdx = 0; vcIdx < valIdxs.size(); ++vcIdx)
          {
            size_t temp = rowElemMap[valIdxs[vcIdx].first] 
                        = rowElemMap[valIdxs[vcIdx].first] 
                          + std::min(valIdxs[vcIdx].second.size(), targValRef.second);
            if(temp == maxElements)
            {
              rowIdx = valIdxs[vcIdx].first;
              elements = temp;
              break;
            }
            if(temp > elements)
            {
              elements = temp;
              rowIdx = valIdxs[vcIdx].first;
            }
          }
        }
      }
      //for (size_t idx = 0; idx < _container.GetRowNum(); ++idx)
      //{
      //  // get number of elements existing in the row
      //  size_t rowResult = MatchElements(_compressSorted[idx], targetCompressed);
      //  if (rowResult == target.size())
      //  {
      //    // update rowIdx and break from search
      //    rowIdx = idx;//PrintRow(idx);
      //    elements = rowResult;
      //    break;
      //  }
      //  else if (rowResult > elements)
      //  {
      //    // update best row
      //    elements = rowResult;
      //    rowIdx = idx;
      //  }
      //}
      //print the best row found
      rowIdx = rowIdx; // prevent error of rowIdx not used when LESSPRINT is defined
      PRINT(PrintRow(rowIdx);)
      if (elements != 0)
      {
        result = FOUND;
      }
    }
  }
  else
  {
    result = NO_DATA;
  }
  END_LAP
  return result;
}

// string input to call on vector input
template <typename DataType>
RESULT SearchClass<DataType>::SearchClosest(const std::string& target)
{
  return SearchClosest(splitStringData<DataType>(target));
}


// execute commands from user
template <typename DataType>
RESULT SearchClass<DataType>::ExecuteCommand(const std::string& target)
{
  size_t splitter = target.find(" ");
  std::string command = target.substr(0, splitter);
  command = toLowerCase(command);
  RESULT result = NOT_FOUND;
  if (command == "searchsequence")
  {
    PRINT(std::cout << target << std::endl;)
    result = SearchSequence(target.substr(splitter + 1));
  }
  else if (command == "searchunordered")
  {
    PRINT(std::cout << target << std::endl;)
    result = SearchUnordered(target.substr(splitter + 1));
  }
  else if (command == "searchclosest")
  {
    PRINT(std::cout << target << std::endl;)
    result = SearchClosest(target.substr(splitter + 1));
  }
  else if (command == "load")
  {
    PRINT(std::cout << target << std::endl;)
    result = Load(target.substr(splitter + 1));

  }
  else
  {
    PRINT(std::cout << "Command '" << command << "' could not be found\n";)
  }
  // *** Call a function to print result output.
  PRINT(HandleResult(result, target);)

  return result;
}

/*********************************************************************************************/
/*********************************************************************************************/
/******************************* Search class Public functions Ends***************************/
/*********************************************************************************************/
/*********************************************************************************************/



/*********************************************************************************************/
/*********************************************************************************************/
/******************************* Search class Private functions Starts************************/
/*********************************************************************************************/
/*********************************************************************************************/


// search if sequence exist in source
template <typename DataType>
bool SearchClass<DataType>::HasSequence(const std::vector<DataType>& source, const std::vector<DataType>& seq)
{
  // if search is empty, any data will be valid
  if (seq.empty())
    return true;
  // search if seq exist
  size_t elementToMatch = seq.size();
  size_t matchedElems = 0;
  size_t idx = 0;
  size_t validLen = source.size() - seq.size();
  do
  {
    // perform search of sequence starting at idx
    while (matchedElems < elementToMatch && source[idx + matchedElems] == seq[matchedElems])
    {
      ++matchedElems;
    }

    // if loop ended because all element matches, exit
    if (elementToMatch == matchedElems)
    {
      return true;
    }

    // reset matching count and go to next element
    matchedElems = 0;
    ++idx;

  } while (validLen--); // perform search until remaining len is shorter than sequence
  return false;
}

// search if all elements in target exist in source
template <typename DataType>
bool SearchClass<DataType>::HasUnordered(const VecDataCounts& source, const VecDataCounts& target)
{
  // early exit if size does not match
  if (target.size() > source.size())
    return false;

  size_t found = 0;
  size_t tIdx = 0;
  size_t sIdx = 0;
  size_t sEnd = source.size();
  size_t tEnd = target.size();

  // while yet to reach the end and source has at least same amount as target
  while (found != tEnd && tEnd - tIdx <= sEnd - sIdx)
  {
    if (target[tIdx].first == source[sIdx].first)
    {
      // found target element
      if (target[tIdx].second <= source[sIdx].second)
      {
        // sufficient amount of target element exist in source
        ++tIdx;
        ++sIdx;
        ++found;
      }
      else
      {
        // insufficient amount of target element in source
        break;
      }
    }
    else if (target[tIdx].first > source[sIdx].first)
    {
      // source element is greater, go to a smaller element
      ++sIdx;
    }
    else
    {
      // target number can no longer be found, exit search
      break;
    }
  }
  return found == tEnd;
}

// count number of elements that exist from target in source
template <typename DataType>
size_t SearchClass<DataType>::MatchElements(const VecDataCounts& source, const VecDataCounts& target)
{
  size_t tIdx = 0;
  size_t sIdx = 0;
  size_t tEnd = target.size();
  size_t sEnd = source.size();
  size_t result = 0;

  // while yet to reach the end
  while (tIdx != tEnd && sIdx != sEnd)
  {
    // if matches
    if (target[tIdx].first == source[sIdx].first)
    {
      // increment result base on smallest number of this element that exist
      result += target[tIdx].second < source[sIdx].second ? target[tIdx].second : source[sIdx].second;
      // go to next element
      ++tIdx;
      ++sIdx;
    }
    // increment target if target is smaller
    else if (target[tIdx].first < source[sIdx].first)
    {
      // source element is greater, go to a smaller element
      ++tIdx;
    }
    // increment source as source is smaller
    else
    {
      ++sIdx;
    }
  }
  // return found matching number of elements
  return result;
}

// for printing rows
template <typename DataType>
void SearchClass<DataType>::PrintRow(size_t idx)
{
  std::cout << "Row " << idx << ": ";
  for (size_t elem = 0; elem < _container.GetRow(idx).size(); ++elem)
  {
    std::cout << _container.GetRow(idx)[elem] << " ";
  }
  std::cout << std::endl;
}
/*********************************************************************************************/
/*********************************************************************************************/
/******************************* Search class Private functions Ends**************************/
/*********************************************************************************************/
/*********************************************************************************************/
