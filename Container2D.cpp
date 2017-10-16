#include "Container2D.h"


Container2D::Container2D()
{
}

bool Container2D::IsEmpty()
{
	// container will be empty if loading failed
	//return _container.empty();
	return !_rows && !_cols;
}


bool Container2D::Load(const std::vector<std::vector<int> >& input)
{
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

//bool Container2D::Load(std::vector<std::vector<int>>&& input)
//{
//}

bool Container2D::AddRow(const std::vector<int>& input)
{
	if(input.size() == _cols)
	{
		_container.push_back(input);
		_rows = _container.size();
		return true;
	}
	else
	{
		return false;
	}
}
/*
bool Container2D::AddRow(std::vector<int>&&)
{
}
*/

//bool AddCol(const std::string&);
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

size_t Container2D::GetRowSize()
{
	return _rows;
}

size_t Container2D::GetColSize()
{
	return _cols;
}

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






