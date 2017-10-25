
template <typename DataType>
Container2D<DataType>::Container2D() : _rows(0), _cols(0)
{
}

// check if data exist in container
template <typename DataType>
bool Container2D<DataType>::IsEmpty()
{
  // container will be empty if loading failed
  return !_rows && !_cols;
}

// load data into container
template <typename DataType>
bool Container2D<DataType>::Load(const std::vector<std::vector<DataType> >& input)
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
template <typename DataType>
bool Container2D<DataType>::AddRow(const std::vector<DataType>& input)
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


// Get data at row idx starting from 0
template <typename DataType>
std::vector<DataType> Container2D<DataType>::GetRow(size_t idx)
{
  if(idx < _rows)
  {
    return _container[idx];
  }
  else
  {
    return std::vector<DataType>();
  }
}

// get number of rows
template <typename DataType>
size_t Container2D<DataType>::GetRowNum()
{
  return _rows;
}

// get number of columns
template <typename DataType>
size_t Container2D<DataType>::GetColNum()
{
  return _cols;
}

// Check if number of columns is the same in all rows
template <typename DataType>
bool Container2D<DataType>::ValidContainer2D(const std::vector<std::vector<DataType> >& input)
{
  if(input.empty())
  {
    return false;
  }
  if (input.back().empty())
  {
    return false;
  }
  
  // get size of input 2D container to loaded from
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

