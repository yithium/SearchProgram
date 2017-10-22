/*
  Container2D is a container created by 2 dimension vector.
  On load, it checks that the 2 dimension vector it is fed is a valid matrix.
  Loading will fail if it is not a valid matrix
  2 dimensional container
*/
#include <cstddef>  //size_t
#include <vector>

template <typename DataType>
class Container2D
{
public:
  Container2D();
  
  
  // For external usage to prevent operation on empty container
  bool IsEmpty();
  
  
  // load data into container, return false if invalid data
  // On failure to load, no changes to current data
  bool Load(const std::vector<std::vector<DataType> >&);
  // Add a row at the back
  bool AddRow(const std::vector<DataType>&);
  
  
  // Get data at row idx, starts at 0
  std::vector<DataType> GetRow(size_t idx);
  
  
  // Get amount of row/col
  size_t GetRowNum();
  size_t GetColNum();
  
  
private:

  // verify all rows has the same size
  bool ValidContainer2D(const std::vector<std::vector<DataType> >&);
  // storage of all data
  std::vector<std::vector<DataType> > _container;
  // current row and column size
  size_t _rows, _cols;
};

#include "Container2D.inl"

