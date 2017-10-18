#include "SearchCLass.h"   // SearchClass declaration
#include "Cryptography.h"  // Encryption/Decryption operation
#include <iostream>        // std::cout for printing operation results
#include <fstream>         // for reading data file
#include <algorithm>       // sort
#include <vector>          // container
#include <sstream>         // getline from string streams



#define KEY "unlock"       // encryption/decryption key
#define BUFFERSIZE 1000    // assumed buffer size
#define ASSUMESIZE 10      // defaulted vector reserve size, to reduce number of allocation due to no reserve


/***************************/
/*** Help functions Begin***/
/***************************/

// splitting a string by newlines or delimiter string
// for splitting input data into their respective rows
std::vector<std::string> splitString(const std::string& input, std::string delimiter = "\n")
{
	std::vector<std::string> result;
	result.reserve(ASSUMESIZE);
	
	size_t sPos = 0;
	size_t ePos = 0;
	
	// split until end of string
	while(ePos != std::string::npos)
	{
		// search for end point of first string
		ePos = input.find(delimiter,sPos);
		// push string to be split into result vector
		result.push_back(
			ePos == std::string::npos          //
				? input.substr(sPos)             // push back remaining string if reached end of string
				: input.substr(sPos,ePos - sPos) // push back sub string found
		);
		
		sPos = ePos + delimiter.length();    // update start position to after delimiter found
	}
	
	// remove empty string created by potential different input string format
	if(result.back().size() == 0)
		result.pop_back();

	// return split result
	return result;
}

// split string into int
std::vector<int> splitStringData(const std::string& input)
{
	std::vector<int> result;
	result.reserve(ASSUMESIZE);
	
	// create string stream to utilize stl
	std::istringstream isstream(input);
	
	int val = 0;
	// get first value from string
	if(isstream.good())
	{
		// get all valid data from string
		while(isstream >> val)
		{
			// push previously obtained valid data into vector
			result.push_back(val);
		}
	}
	
	return result;
}

// returns sorted element value and counts from unsorted data vector
std::vector<std::pair<int, size_t> > countElem(std::vector<int> input)
{
	std::vector<std::pair<int, size_t> > result;
	result.reserve(input.size());
	
	std::sort(input.begin(), 
		input.end()
	);
	
	// combine sorted data
	int curElem = input[0];
	size_t count = 0;
	// count number of each elements and push into result when element is different from previous
	for(size_t elem = 0; elem < input.size(); ++elem)
	{
		// increase count if same element
		if(curElem == input[elem])
		{
			++count;
		}
		// push if different element and initialize value for next element
		else if (curElem != input[elem])
		{
			result.push_back(std::pair<int, size_t>(curElem,count));
			curElem = input[elem];
			count = 1;
		}
	}
	// push the last element counts that has not been pushed
	result.push_back(std::pair<int, size_t>(curElem,count));
	return result;
}

/***************************/
/**** Help functions End****/
/***************************/


// for printing rows
void SearchClass::PrintRow(size_t idx)
{
	std::cout << "Row "<< idx << ": ";
	for(size_t elem = 0; elem < _container.GetRow(idx).size(); ++elem)
	{
		std::cout << _container.GetRow(idx)[elem] << " ";
	}
	std::cout << std::endl;
}

// constructor
SearchClass::SearchClass()
{
}

// load from data from file
bool SearchClass::Load(std::string input)
{
	// opening file
	std::ifstream fileStream(input.c_str(), std::ios::binary);
	Cryptography crypto(KEY);
	bool result = false;
	// read data if file opened successfully
	if(fileStream.good())
	{
		// get data size
		fileStream.seekg (0, fileStream.end);
		int length = fileStream.tellg();
		fileStream.seekg (0, fileStream.beg);
		
		std::string fileData;
		// read data into string using buffer of BUFFERSIZE 
		while(length > 0 && fileStream.good())
		{
			char buffer[BUFFERSIZE] = {0};
			int readSize = length > BUFFERSIZE ? BUFFERSIZE:length;
			
			fileStream.read(buffer, readSize);
			fileData.append(buffer,readSize);
			
			length -= readSize;
		}
		fileStream.close();
		
		// decrypt data
		fileData = crypto.Decrypt(fileData);
		// split into data into rows
		std::vector<std::string> vecStr = splitString(fileData);
		// split data rows into elements
		std::vector<std::vector<int> > data2D;
		data2D.reserve(ASSUMESIZE);
		for(size_t idx = 0; idx < vecStr.size(); ++idx)
		{
			data2D.push_back(splitStringData(vecStr[idx]));
		}
		// load container using 2d elements data
		result = Load(data2D);
		
		
	}
	return result;
}

// load container 2d using 2d vector
bool SearchClass::Load(const std::vector<std::vector<int> >& input)
{
	// verify number of elements in each row is the same
	bool result = _container.Load(input);
	// create data for searchUnordered and searchClosest optimization
	if (result)
	{
		_compressSorted.reserve(_container.GetRowNum());
		// 
		for(size_t idx = 0; idx < _container.GetRowNum(); ++idx)
		{
			_compressSorted.push_back(
				countElem(
					_container.GetRow(idx)
				)
			);
		}
	}
	
	return result;
}

// Add row to back of container
bool SearchClass::AddRow(const std::vector<int>& input)
{
	bool result = _container.AddRow(input);
	if(result)
	{
		// create data for searchUnordered and searchClosest optimization
		_compressSorted.push_back( 
			countElem( 
				_container.GetRow(
					_container.GetRowNum()-1
				)
			)
		);
	}
	return result;
}

// Verify SearchClass is read for 'Command Execution'
bool SearchClass::HasData()
{
	return !_container.IsEmpty();
}

// return true if found at least one row that match sequence
bool SearchClass::SearchSequence(const std::vector<int>& target)
{
	bool result = false;
	// ensure user did not try to call on invalid data
	// ends early if number of element exceed size of row
	if(HasData())
	{
		if(_container.GetColNum() >= target.size())
		{
			// search for sequence in each data rows
			for(size_t idx = 0; idx < _container.GetRowNum(); ++idx)
			{
				// if sequence exist
				if(HasSequence(_container.GetRow(idx), target))
				{
					// print row and indicate result found
					PrintRow(idx);
					result = true;
				}
			}
		}
		else
		{
			std::cout << "Input length greater than data set\n";
		}
	}
	else
	{
		std::cout << "No valid data for operation\n";
	}
	return result;
}

// string input to call on vector input
bool SearchClass::SearchSequence(const std::string& target)
{
	return SearchSequence(splitStringData(target));
}

// return true if found at least one row that contain all elements unordered
bool SearchClass::SearchUnordered(const std::vector<int>& target)
{
	bool result = false;
	// ensure user did not try to call on invalid data
	// ends early if number of element exceed size of row
	if(HasData()) 
	{
		if(_container.GetColNum() >= target.size())
		{
			// convert user's input for easier manipulation
			std::vector<std::pair<int, size_t> > targetCompressed = countElem(target);
			for(size_t idx = 0; idx < _container.GetRowNum(); ++idx)
			{
				// if all elements exist in row data
				if(HasUnordered(_compressSorted[idx], targetCompressed))
				{
					// print row and indicate result found
					PrintRow(idx);
					result = true;
				}
			}
		}
		else
		{
			std::cout << "Input length greater than data set\n";
		}
	}
	else
	{
		std::cout << "No valid data for operation\n";
	}
	return result;
}

// string input to call on vector input
bool SearchClass::SearchUnordered(const std::string& target)
{
	return SearchUnordered(splitStringData(target));
}

// print closest result to user input
bool SearchClass::SearchClosest(const std::vector<int>& target)
{
	if(HasData())
	{
		size_t elements = 0;
		size_t rowIdx = 0;
		std::vector<std::pair<int, size_t> > targetCompressed = countElem(target);
		for(size_t idx = 0; idx < _container.GetRowNum(); ++idx)
		{
			// get number of elements existing in the row
			size_t rowResult = MatchElements(_compressSorted[idx], targetCompressed);
			if(rowResult == target.size())
			{
				// update rowIdx and break from search
				rowIdx = idx;//PrintRow(idx);
				break;
			}
			else if(rowResult > elements)
			{
				// update best row
				elements = rowResult;
				rowIdx = idx;
			}
		}
		//print the best row found
		PrintRow(rowIdx);
	}
	else
	{
		std::cout << "No valid data for operation\n";
	}
	// for the sake of being similar to other functions.
	return true;
}

// string input to call on vector input
bool SearchClass::SearchClosest(const std::string& target)
{
	return SearchClosest(splitStringData(target));
}

// execute commands from user
bool SearchClass::ExecuteCommand(const std::string& target)
{
	size_t splitter = target.find(" ");
	std::string command = target.substr(0,splitter);
	bool result = false;
	if(command == "searchSequence")
	{
		result = SearchSequence(target.substr(splitter+1));
	}
	else if(command == "searchUnordered")
	{
		result = SearchUnordered(target.substr(splitter+1));
	}
	else if(command == "searchClosest")
	{
		result = SearchClosest(target.substr(splitter+1));
	}
	else
	{
		std::cout << "Command '"<< command << "' could not be found\n";
	}
	if(!result)
	{
		if(target.at(target.size()-1) == '\r')
		{
			std::cout << "'" << target.substr(0,target.size()-1) << "' is not found in any row.\n";
		}
		else
		{
			std::cout << "'" << target << "' is not found in any row.\n";
		}
	}
	return result;
}

// search if sequence exist in source
bool SearchClass::HasSequence(const std::vector<int>& source, const std::vector<int>& seq)
{
	// if search is empty, any data will be valid
	if(seq.empty())
		return true;
	// search if seq exist
	size_t elementToMatch = seq.size();
	size_t matchedElems = 0;
	size_t idx = 0;
	size_t validLen = source.size() - seq.size();
	do
	{
		// perform search of sequence starting at idx
		while(matchedElems < elementToMatch && source[idx + matchedElems] == seq[matchedElems])
		{
			++matchedElems;
		}
		
		// if loop ended because all element matches, exit
		if(elementToMatch == matchedElems)
		{
			return true;
		}
		
		// reset matching count and go to next element
		matchedElems = 0;
		++idx;
		
	}while(validLen--); // perform search until remaining len is shorter than sequence
	return false;
}

// search if all elements in target exist in source
bool SearchClass::HasUnordered(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& target)
{
	// early exit if size does not match
	if(target.size() > source.size())
		return false;

	size_t found = 0;
	size_t tIdx = 0;
	size_t sIdx = 0;
	size_t sEnd = source.size();
	size_t tEnd = target.size();
	
	// while yet to reach the end and source has at least same amount as target
	while (tEnd - tIdx <= sEnd - sIdx)
	{
		if(target[tIdx].first == source[sIdx].first)
		{
			// found target element
			if(target[tIdx].second <= source[sIdx].second)
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
		else if( target[tIdx].first > source[sIdx].first)
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
size_t SearchClass::MatchElements(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& target)
{
	size_t tIdx = 0;
	size_t sIdx = 0;
	size_t tEnd = target.size();
	size_t sEnd = source.size();
	size_t result = 0;
	
	// while yet to reach the end
	while(tIdx != tEnd && sIdx != sEnd)
	{
		// if matches
		if(target[tIdx].first == source[sIdx].first)
		{
			// increment result base on smallest number of this element that exist
			result += target[tIdx].second < source[sIdx].second ? target[tIdx].second : source[sIdx].second;
			// go to next element
			++tIdx;
			++sIdx;
		}
		// increment target if target is smaller
		else if( target[tIdx].first < source[sIdx].first)
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







