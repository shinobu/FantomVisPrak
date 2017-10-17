#include <fantom/algorithm.hpp>
#include <fantom/register.hpp>
#include <fantom/fields.hpp>
using namespace fantom;
namespace
{
    class CommitTutorialAlgorithm : public DataAlgorithm
    {
    public:
        struct Options : public DataAlgorithm::Options
        {
            Options( fantom::Options::Control& control )
                : DataAlgorithm::Options( control )
            {
            }
        };

        //================================================================================================================

        struct DataOutputs : public DataAlgorithm::DataOutputs
        {
            DataOutputs( fantom::DataOutputs::Control& control )
                : DataAlgorithm::DataOutputs( control )
            {
                add< Grid< 3 > >( "grid" );
            }
        };

        //================================================================================================================

        CommitTutorialAlgorithm( InitData& data )
            : DataAlgorithm( data )
        {
        }

        //================================================================================================================

        virtual void execute( const Algorithm::Options& options, const volatile bool& /*abortFlag*/ ) override
        {
          // ##### Variable #####
          size_t numCellTypes = (size_t) 2;
          const std::pair< Cell::Type, size_t >  cellCounts0(Cell::HEXAHEDRON, 1);
          const std::pair< Cell::Type, size_t >  cellCounts1(Cell::TETRAHEDRON, 1);
          const std::pair< Cell::Type, size_t > cellCounts[2] = {cellCounts0, cellCounts1};
          std::vector<size_t> coords;
          coords.push_back((size_t)0);
          coords.push_back((size_t)1);
          DefaultValueArray< size_t >* indArray = new DefaultValueArray< size_t >( coords, Precision::FLOAT64);
          std::unique_ptr< ValueArray< size_t > > indices((ValueArray<size_t>*) indArray);
          //std::unique_ptr< DefaultValueArray< size_t > > indices( indArray );
          // ##### Domain #####
            long nx = 10;
            long ny = 10;
            long nz = 10;
            double dx = 1.0;
            double dy = 1.0;
            double dz = 1.0;
            size_t extent[] = { (size_t)nx,
                                (size_t)ny,
                                (size_t)nz };
            double origin[] = { -0.5 * dx * ( nx - 1 ),
                                -0.5 * dy * ( ny - 1 ),
                                -0.5 * dz * ( nz - 1 ) };
            double spacing[] = { dx, dy, dz };

            std::shared_ptr< const DiscreteDomain< 3 > > mDomain = DomainFactory::makeDomainUniform( extent, origin, spacing );

            //##### Make Object #####
            std::shared_ptr< const Grid< 3 > > mGrid = DomainFactory::makeGridStructured( *mDomain );


            //##### New Object #####
            static std::shared_ptr< const Grid< 3 > > mGridd = DomainFactory::makeGridUnstructured(*mDomain, numCellTypes, cellCounts, std::move(indices));

            setResult( "grid", mGrid );
        }
    };
    AlgorithmRegister< CommitTutorialAlgorithm > dummy( "Tutorial/Commit2", "Generate a simple vector field." );
}
