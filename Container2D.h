#include <string>
#include <vector>



class Container2D
{
public:
	Container2D();
	bool IsEmpty();
	bool Load(const std::vector<std::vector<int> >&);
	//bool Load(std::vector<std::vector<int> >&&);
	bool AddRow(const std::vector<int>&);
	//bool AddRow(std::vector<int>&&);
	//bool AddCol(const std::string&);
	std::vector<int> GetRow(size_t idx);
	size_t GetRowSize();
	size_t GetColSize();
	
	
private:
	bool ValidContainer2D(const std::vector<std::vector<int> >&);
	std::vector<std::vector<int> > _container;
	size_t _rows, _cols;
};



