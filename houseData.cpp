#include <stdexcept>
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

        struct position
        {
          double x = 0;
          double y = 0;
	  double z = 0;
          size_t index;
          bool init = false;
          bool same(double offsetX, double offsetY, double offsetZ){
            return (((x == offsetX) && (y == offsetY)) && (z==offsetZ) && (init));
          }
          void set(double offsetX, double offsetY, double offsetZ, size_t indexP){
            x = offsetX;
            y = offsetY;
	    z = offsetZ;
            init = true;
            index = indexP;
          }
        };

        struct positionCache{
          position pos[4];
          position* current = pos;
          int currentInd = 0;
          size_t search(double offsetX, double offsetY, double offsetZ){
            int i = 0;
            while (i < 4) {
              if(pos[(currentInd + i) % 4].same(offsetX, offsetY, offsetZ)){
                return pos[(currentInd + i) % 4].index;
              }
              i++;
            }
            return -1;
          }
          size_t addPosition(double offsetX, double offsetY, double offsetZ, size_t indexP){
            current[currentInd].set(offsetX, offsetY, offsetZ, indexP);
            currentInd = (currentInd + 1) % 4;
            return indexP;
          }
        };

        houseDataAlgorithm( InitData & data)
            : DataAlgorithm(data)
        {
            // initialize internal data members
        }
        positionCache cache;

        size_t usePoint(std::initializer_list<double> point)
        {
            double offsetX = point.begin()[0];
	    double offsetY = point.begin()[1];
            double offsetZ = point.begin()[2];
            int exist = cache.search(offsetX,  offsetY, offsetZ);
            if(exist >= 0){
              return exist;
            }else{
              pointStack.push_back(*(new Tensor<double, 3>(point)));
              return cache.addPosition(offsetX, offsetY, offsetZ , (size_t)(pointStack.size() -1 )); //nach Suchstruktur verändern
            }
        }

        void createHouse(double offsetX, double offsetY, int height = 0){
            makeHouseBody(offsetX, offsetY, height);
            makeHouseRoof(offsetX, offsetY, height);
        }

        void makeHouseBody(double offsetX, double offsetY, int height = 0){
	  if(height < 1){
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, 0.5, 0.5 + offsetY}));
	  }else {  
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, -0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, -0.5, -0.5 + offsetY}));

          size_t local[4];
          local[0] = usePoint({-0.5 + offsetX, 0.5, -0.5 + offsetY});
          indicesRaw.push_back(local[0]);
          local[1] = usePoint({0.5 + offsetX, 0.5, -0.5 + offsetY});
          indicesRaw.push_back(local[1]);
          local[2] = usePoint({0.5 + offsetX, 0.5, 0.5 + offsetY});
          indicesRaw.push_back(local[2]);
          local[3] = usePoint({-0.5 + offsetX, 0.5, 0.5 + offsetY});
          indicesRaw.push_back(local[3]);

                for(int i = 0; i < height; i++)
		{
                    indicesRaw.push_back(local[3]);
                    indicesRaw.push_back(local[2]);
                    indicesRaw.push_back(local[1]);
                    indicesRaw.push_back(local[0]);
                    local[0] = usePoint({-0.5 + offsetX, i + 0.5, -0.5 + offsetY});
                    indicesRaw.push_back(local[0]);
                    local[1] = usePoint({0.5 + offsetX, i + 0.5, -0.5 + offsetY});
                    indicesRaw.push_back(local[1]);
                    local[2] = usePoint({0.5 + offsetX, i + 0.5, 0.5 + offsetY});
                    indicesRaw.push_back(local[2]);
                    local[3] = usePoint({-0.5 + offsetX, i + 0.5, 0.5 + offsetY});
                    indicesRaw.push_back(local[3]);
	  	} 
	  }
          const std::pair< Cell::Type, size_t > cellCounts0(Cell::HEXAHEDRON, (1 + height));
          cellCountsV.push_back(cellCounts0);
          numCellTypes += 1;
        }

        void makeHouseRoof(double offsetX, double offsetY, int height = 0){
          if(height > 0){
              height--;
          }
          indicesRaw.push_back(usePoint({-0.5 + offsetX, height + 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, height + 0.5, 0.5 + offsetY}));
          indicesRaw.push_back(usePoint({0.5 + offsetX, height + 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({-0.5 + offsetX, height + 0.5, -0.5 + offsetY}));
          indicesRaw.push_back(usePoint({offsetX, (height + 1), offsetY}));
          const std::pair< Cell::Type, size_t >  cellCounts1(Cell::PYRAMID, 1);
          cellCountsV.push_back(cellCounts1);
          numCellTypes++;
        }

        void buildStarCity(int houses, int height, int spreading)
        {
            int check;
            int x, z, h;
            int max;
            int min;
            int offsetX = 1;
            int offsetZ = 1;

            for (int house = 0; house < houses; ++house)
            {
                check = house % 4;
                x = 0, z = 0, h;
                max = height;
                min = 0;

                if(house == 0)
                {
                    h = 0;
                    createHouse(x, z, h);
                }else{

                    h = rand()%(max-min + 1) + min;

                    if(check == 0 )
                    {
                        createHouse(x-(spreading*offsetX), z, h);
                        offsetX += 1;
                        offsetZ += 1;
                    }else if(check == 1 )
                    {
                        createHouse(x, z+(spreading*offsetZ), h);
                    }else if(check == 2 )
                    {
                        createHouse(x+(spreading*offsetX), z, h);
                    }else if(check == 3 )
                    {
                        createHouse(x, z-(spreading*offsetZ), h);
                    }
                }
            }
        }

        void makePlain(double length, double width){
            length *= 0.5;
            length++;
            width *= 0.5;
            width++;
            indicesRaw.push_back(usePoint({-length, -0.5, -width}));
            indicesRaw.push_back(usePoint({-length, -0.5, width}));
            indicesRaw.push_back(usePoint({length, -0.5, width}));
            indicesRaw.push_back(usePoint({length, -0.5, -width}));
            const std::pair< Cell::Type, size_t >  cellCounts1(Cell::QUAD, 1);
            cellCountsV.push_back(cellCounts1);
            numCellTypes++;
        }

        void execute( const Algorithm::Options& options, const volatile bool& abortFlag ) override
        {
            int house_count = options.get<int>("#houses");

            buildStarCity(house_count, 10, 2);
            makePlain(house_count, house_count);

            //createHouse(0, 0, house_count);
  //          createHouse(2, 2, 0);

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
