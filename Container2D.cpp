#include "Container2D.h"


Container2D::Container2D()
{
}

// check if data exist in container
bool Container2D::IsEmpty()
{
	// container will be empty if loading failed
	//return _container.empty();
	return !_rows && !_cols;
}

// load data into container
bool Container2D::Load(const std::vector<std::vector<int> >& input)
{
	// ensure input is valid
	bool result = ValidContainer2D(input);
	if(result)
	{
		// load data into container if data is valid.
		_container = input;
		_rows = input.size();
		_cols = input.back().size();
	}
	return result;
}

// add row to end of container
bool Container2D::AddRow(const std::vector<int>& input)
{
	// ensure input has valid number of columns
	if(input.size() == _cols)
	{
		// add row to back and update number of rows
		_container.push_back(input);
		_rows = _container.size();
		return true;
	}
	else
	{
		return false;
	}
}

//bool AddCol(const std::string&);

// Get data at row idx starting from 0
std::vector<int> Container2D::GetRow(size_t idx)
{
	if(idx < _rows)
	{
		return _container[idx];
	}
	else
	{
		return std::vector<int>();
	}
}

// get number of rows
size_t Container2D::GetRowNum()
{
	return _rows;
}

// get number of columns
size_t Container2D::GetColNum()
{
	return _cols;
}

// Check if number of columns is the same in all rows
bool Container2D::ValidContainer2D(const std::vector<std::vector<int> >& input)
{
	if(input.empty())
	{
		return false;
	}
	if (input.back().empty())
	{
		return false;
	}
	
	// get size of input 2D container to deep copy from
	size_t inputRows = input.size();
	size_t inputCols = input.back().size();
	
	for(unsigned idx = 0; idx < inputRows; ++idx)
	{
		if(input[idx].size() != inputCols)
		{
			return false;
		}
	}
	return true;
}






