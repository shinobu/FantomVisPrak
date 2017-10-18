#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>

using namespace fantom;

namespace
{
    class houseDataAlgorithm
        : public DataAlgorithm
    {

    public:

        struct Options
            : public DataAlgorithm::Options
        {
            Options( Control& control )
                : DataAlgorithm::Options(control)
            {
                // add options here
                add<int>("#houses", "the number of generated buildings", 4, &acceptNumber);
            }

            static int acceptNumber(const int& number){
                return std::max(number, 4);
            }
        };

        struct DataOutputs
            : public DataAlgorithm::DataOutputs
        {
            DataOutputs( Control& control )
                : DataAlgorithm::DataOutputs(control)
            {
                // add outputs here
                add< Grid<3> >("Grid");
            }

        };

        houseDataAlgorithm( InitData & data)
            : DataAlgorithm(data)
        {
            // initialize internal data members
        }

        void execute( const Algorithm::Options& options, const volatile bool& abortFlag ) override
        {
            int house_count = options.get<int>("#houses");
            double coord_1[3] = {-0.5, -0.5, 0.5};
            double coord_2[3] = {0.5, -0.5, 0.5};
            double coord_3[3] = {0.5, -0.5, -0.5};
            double coord_4[3] = {-0.5, -0.5, -0.5};
            double coord_5[3] = {-0.5, 0.5, -0.5};
            double coord_6[3] = {0.5, 0.5, -0.5};
            double coord_7[3] = {0.5, 0.5, 0.5};
            double coord_8[3] = {-0.5, 0.5, 0.5};
            double coord_9[3] = {0, 1, 0};
            Tensor<double, 3> tensor_1(coord_1);
            Tensor<double, 3> tensor_2(coord_2);
            Tensor<double, 3> tensor_3(coord_3);
            Tensor<double, 3> tensor_4(coord_4);
            Tensor<double, 3> tensor_5(coord_5);
            Tensor<double, 3> tensor_6(coord_6);
            Tensor<double, 3> tensor_7(coord_7);
            Tensor<double, 3> tensor_8(coord_8);
            Tensor<double, 3> tensor_9(coord_9);
            std::vector< Tensor<double, 3>  > points;
            points.push_back(tensor_1);
            points.push_back(tensor_2);
            points.push_back(tensor_3);
            points.push_back(tensor_4);
            points.push_back(tensor_5);
            points.push_back(tensor_6);
            points.push_back(tensor_7);
            points.push_back(tensor_8);
            points.push_back(tensor_9);
            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary(points, Precision::FLOAT64);
            // ##### numCellTypes #####
            size_t numCellTypes = (size_t) 2;

            // ##### cellCounts #####
            const std::pair< Cell::Type, size_t >  cellCounts0(Cell::HEXAHEDRON, 1);
            const std::pair< Cell::Type, size_t >  cellCounts1(Cell::PYRAMID, 1);
            const std::pair< Cell::Type, size_t > cellCounts[2] = {cellCounts0, cellCounts1};

            // ##### indices #####
            // Hexahedron
            std::vector<size_t> coords;
            coords.push_back((size_t)0);
            coords.push_back((size_t)1);
            coords.push_back((size_t)2);
            coords.push_back((size_t)3);
            coords.push_back((size_t)4);
            coords.push_back((size_t)5);
            coords.push_back((size_t)6);
            coords.push_back((size_t)7);

            // Pyramid
            coords.push_back((size_t)4);
            coords.push_back((size_t)5);
            coords.push_back((size_t)6);
            coords.push_back((size_t)7);
            coords.push_back((size_t)8);
            //coords.push_back((size_t)1);
            DefaultValueArray< size_t >* indArray = new DefaultValueArray< size_t >( coords, Precision::FLOAT64);
            std::unique_ptr< ValueArray< size_t > > indices((ValueArray<size_t>*) indArray);

            static std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridUnstructured(*mDomain, numCellTypes, cellCounts, std::move(indices));
            setResult("Grid", mGrid);
        }
    };

    AlgorithmRegister<houseDataAlgorithm>
        dummy("praktikum/houseData", "executes houseData");
}
