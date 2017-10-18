#include <fantom/algorithm.hpp>
#include <fantom/fields.hpp>
#include <fantom/register.hpp>
#include <cmath>

using namespace fantom;

namespace
{
    class houseDataAlgorithm : public DataAlgorithm
    {
      std::vector< Tensor<double, 3>  > pointStack;
      std::vector<size_t> indicesRaw;
      std::vector<std::pair< Cell::Type, size_t >> cellCountsV;
      size_t numCellTypes = (size_t) 0;
    public:
        struct Options : public DataAlgorithm::Options
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

        struct DataOutputs : public DataAlgorithm::DataOutputs
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

        size_t usePoint(std::initializer_list<double> point)
        {
          //hier Suchstruktur für Wiederverwendung einbauen
            pointStack.push_back(*(new Tensor<double, 3>(point)));
            return (size_t)(pointStack.size() -1 ); //nach Suchstruktur verändern
        }

        void createHouse(double offsetX, double offsetY, int height = 0){
            makeHouseBody(offsetX, offsetY, height);
            makeHouseRoof(offsetX, offsetY);
        }

        void makeHouseBody(double offsetX, double offsetY, int height = 0){
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, 0.5 + offsetY}));
          const std::pair< Cell::Type, size_t > cellCounts0(Cell::HEXAHEDRON, 1);
          cellCountsV.push_back(cellCounts0);
          numCellTypes += 1;
        }

        void makeHouseRoof(double offsetX, double offsetY){
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({offsetX, 1, offsetY}));
          const std::pair< Cell::Type, size_t >  cellCounts1(Cell::PYRAMID, 1);
          cellCountsV.push_back(cellCounts1);
          numCellTypes++;
        }

        void execute( const Algorithm::Options& options, const volatile bool& abortFlag ) override
        {
            int house_count = options.get<int>("#houses");

            createHouse(0, 0, 0);
            createHouse(2, 2, 0);

            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainArbitrary(pointStack, Precision::FLOAT64);
            std::pair< Cell::Type, size_t > *cellCounts = new std::pair< Cell::Type, size_t >[cellCountsV.size()];
            std::copy(cellCountsV.begin(), cellCountsV.end(), cellCounts);

            DefaultValueArray< size_t >* indArray = new DefaultValueArray< size_t >( indicesRaw, Precision::FLOAT64);
            std::unique_ptr< ValueArray< size_t > > indices((ValueArray<size_t>*) indArray);

            static std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridUnstructured(*mDomain, numCellTypes, cellCounts, std::move(indices));
            setResult("Grid", mGrid);
        }
    };

    AlgorithmRegister<houseDataAlgorithm>
        dummy("praktikum/houseData", "executes houseData");
}
