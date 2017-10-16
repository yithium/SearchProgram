#include "SearchCLass.h"
#include "Cryptography.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
//const std::string KEY ("unlock");

#define KEY "unlock"
#define BUFFERSIZE 1000
#define ASSUMESIZE 10

std::vector<std::string> splitString(const std::string& input, std::string delimiter = "\n")
{
	std::vector<std::string> result;
	result.reserve(ASSUMESIZE);
	size_t sPos = 0;
	size_t ePos = 0;
	while(ePos != std::string::npos)
	{
		ePos = input.find(delimiter,sPos);
		result.push_back(ePos == std::string::npos ? input.substr(sPos) : input.substr(sPos,ePos - sPos));
		sPos = ePos + delimiter.length();
	}
	if(result.back().size() == 0)
		result.pop_back();
	return result;
}

std::vector<int> splitStringInt(const std::string& input)
{
	std::vector<int> result;
	result.reserve(ASSUMESIZE);
	
	std::istringstream isstream(input);
	
	int val = 0;
	if(isstream.good())
	{
		isstream >> val;
	}
	while(isstream.good())
	{
		result.push_back(val);
		isstream >> val;
	}
	return result;
}

bool pairCompare(std::pair<int,size_t> lhs, std::pair<int,size_t> rhs)
{
	return lhs.first < rhs.first;
}

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
	for(size_t elem = 0; elem < input.size(); ++elem)
	{
		if(curElem == input[elem])
		{
			++count;
		}
		else if (curElem != input[elem])
		{
			result.push_back(std::pair<int, size_t>(curElem,count));
			curElem = input[elem];
			count = 1;
		}
	}
	result.push_back(std::pair<int, size_t>(curElem,count));
	return result;
}

void SearchClass::PrintRow(size_t idx)
{
	std::cout << "Row "<< idx << ": ";
	for(size_t elem = 0; elem < _container.GetRow(idx).size(); ++elem)
	{
		std::cout << _container.GetRow(idx)[elem] << " ";
	}
	std::cout << std::endl;
}

SearchClass::SearchClass()
{
}

bool SearchClass::Load(std::string input)
{
	std::ifstream fileStream(input.c_str(), std::ios::binary);
	Cryptography crypto(KEY);
	bool result = false;
	if(fileStream.good())
	{
		fileStream.seekg (0, fileStream.end);
		int length = fileStream.tellg();
		fileStream.seekg (0, fileStream.beg);
		
		std::string fileData;
		while(length > 0 && fileStream.good())
		{
			char buffer[BUFFERSIZE] = {0};
			int readSize = length > BUFFERSIZE ? BUFFERSIZE:length;
			
			fileStream.read(buffer, readSize);
			fileData.append(buffer);
			
			length -= readSize;
		}
		fileStream.close();
		
		
		fileData = crypto.Decrypt(fileData);
		std::vector<std::string> vecStr = splitString(fileData);
		std::vector<std::vector<int> > data2D;
		data2D.reserve(ASSUMESIZE);
		for(size_t idx = 0; idx < vecStr.size(); ++idx)
		{
			data2D.push_back(splitStringInt(vecStr[idx]));
		}
		
		result = Load(data2D);
		
		
	}
	return result;
}

bool SearchClass::Load(const std::vector<std::vector<int> >& input)
{
	bool result = _container.Load(input);
	if (result)
	{
		_compressSorted.reserve(_container.GetRowSize());
		for(size_t idx = 0; idx < _container.GetRowSize(); ++idx)
		{
			_compressSorted.push_back(
				countElem(
					_container.GetRow(idx)
				)
			);
		}
	}
	//std::cout << (result ? "Success":"Fail") << " container Size is now R: " << _container.GetRowSize() << ", C: " << _container.GetColSize() << std::endl;
	return result;
}

bool SearchClass::AddRow(const std::vector<int>& input)
{
	bool result = _container.AddRow(input);
	if(result)
	{
		_compressSorted.push_back( 
			countElem( 
				_container.GetRow(
					_container.GetRowSize()-1
				)
			)
		);
	}
	return result;
}

bool SearchClass::HasData()
{
	return !_container.IsEmpty();
}

bool SearchClass::SearchSequence(const std::vector<int>& target)
{
	bool result = false;
	if(HasData() && _container.GetColSize() >= target.size())
	{
		// return first valid
		for(size_t idx = 0; idx < _container.GetRowSize(); ++idx)
		{
			if(HasSequence(_container.GetRow(idx), target))
			{
				PrintRow(idx);
				result = true;
			}
		}
	}
	return result;
}
bool SearchClass::SearchSequence(const std::string& target)
{
	return SearchSequence(splitStringInt(target));
}


bool SearchClass::SearchUnordered(const std::vector<int>& target)
{
	bool result = false;
	if(HasData() && _container.GetColSize() >= target.size())
	{
		std::vector<std::pair<int, size_t> > targetCompressed = countElem(target);
		for(size_t idx = 0; idx < _container.GetRowSize(); ++idx)
		{
			if(HasUnordered(_compressSorted[idx], targetCompressed))
			{
				PrintRow(idx);
				result = true;
			}
		}
	}
	return result;
}
bool SearchClass::SearchUnordered(const std::string& target)
{
	return SearchUnordered(splitStringInt(target));
}
void SearchClass::SearchClosest(const std::vector<int>& target)
{
	if(HasData())
	{
		size_t elements = 0;
		size_t rowIdx = 0;
		std::vector<std::pair<int, size_t> > targetCompressed = countElem(target);
		for(size_t idx = 0; idx < _container.GetRowSize(); ++idx)
		{
			// get number of elements existing in the row
			size_t rowResult = MatchElements(_compressSorted[idx], targetCompressed);
			if(rowResult == target.size())
			{
				// print row that fully matches and end
				PrintRow(idx);
				return;
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
}
void SearchClass::SearchClosest(const std::string& target)
{
	SearchClosest(splitStringInt(target));
}

bool SearchClass::ExecuteCommand(const std::string& target)
{
	size_t splitter = target.find(" ");
	std::string command = target.substr(0,splitter);
	if(command == "searchSequence")
	{
		return SearchSequence(target.substr(splitter+1));
	}
	else if(command == "searchUnordered")
	{
		return SearchUnordered(target.substr(splitter+1));
	}
	else if(command == "searchClosest")
	{
		SearchClosest(target.substr(splitter+1));
	}
	return false;
}

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
		while(matchedElems < elementToMatch && source[idx + matchedElems] == seq[matchedElems])
		{
			++matchedElems;
		}
		
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

bool SearchClass::HasUnordered(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& target)
{
	if(target.size() > source.size())
		return false;

	size_t found = 0;
	size_t tIdx = 0;
	size_t sIdx = 0;
	size_t sEnd = source.size();
	size_t tEnd = target.size();
	
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

size_t SearchClass::MatchElements(const std::vector<std::pair<int, size_t> >& source, const std::vector<std::pair<int, size_t> >& target)
{
	size_t tIdx = 0;
	size_t sIdx = 0;
	size_t tEnd = target.size();
	size_t sEnd = source.size();
	size_t result = 0;
	while(tIdx != tEnd && sIdx != sEnd)
	{
		if(target[tIdx].first == source[sIdx].first)
		{
			result += target[tIdx].second < source[sIdx].second ? target[tIdx].second : source[sIdx].second;
			++tIdx;
			++sIdx;
		}
		else if( target[tIdx].first < source[sIdx].first)
		{
			// source element is greater, go to a smaller element
			++tIdx;
		}
		else
		{
			++sIdx;
		}
	}
	return result;
}







